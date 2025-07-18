/*
 * PollingRoutine.c
 *
 *  Created on: Oct 24, 2023
 *      Author: karl.yamashita
 *
 *
 *      Template for projects.
 *
 *      The object of this PollingRoutine.c/h files is to not have to write code in main.c which already has a lot of generated code.
 *      It is cumbersome having to scroll through all the generated code for your own code and having to find a USER CODE section so your code is not erased when CubeMX re-generates code.
 *      
 *      Direction: Call PollingInit before the main while loop. Call PollingRoutine from within the main while loop
 * 
 *      Example;
        // USER CODE BEGIN WHILE
        PollingInit();
        while (1)
        {
            PollingRoutine();
            // USER CODE END WHILE

            // USER CODE BEGIN 3
        }
        // USER CODE END 3

 */


#include "main.h"


const char version[] = "v1.0.1";

#define UART2_DMA_RX_QUEUE_SIZE 10 // queue size
#define UART2_DMA_TX_QUEUE_SIZE 4
UART_DMA_Data uart2_dmaDataRxQueue[UART2_DMA_RX_QUEUE_SIZE] = {0};
UART_DMA_Data uart2_dmaDataTxQueue[UART2_DMA_TX_QUEUE_SIZE] = {0};

UART_DMA_Struct_t uart2_msg =
{
	.huart = &huart2,
	.rx.queueSize = UART2_DMA_RX_QUEUE_SIZE,
	.rx.msgQueue = uart2_dmaDataRxQueue,
	.tx.queueSize = UART2_DMA_TX_QUEUE_SIZE,
	.tx.msgQueue = uart2_dmaDataTxQueue,
	.dma.dmaPtr.SkipOverFlow = true
};

char msg_copy_command[UART_DMA_QUEUE_DATA_SIZE] = {0};



Button_t button =
{
	// 0 = sleep, 1 = stop, 2 = standby, 3 = shutdown
	.pwr_state = 1
};

void PollingInit(void)
{
    UART_DMA_EnableRxInterruptIdle(&uart2_msg);

    // toggle led so we know STM32 is running
    TimerCallbackRegisterOnly(&timerCallback, LED_Toggle);
    TimerCallbackTimerStart(&timerCallback, LED_Toggle, 200, TIMER_REPEAT);

    // register callbacks
    TimerCallbackRegisterOnly(&timerCallback, EnterSleepMode);
    TimerCallbackRegisterOnly(&timerCallback, EnterStopMode);
    TimerCallbackRegisterOnly(&timerCallback, EnterStandbyMode);
    TimerCallbackRegisterOnly(&timerCallback, EnterShutdownMode);
    TimerCallbackRegisterOnly(&timerCallback, ButtonDebounced);
    TimerCallbackRegisterOnly(&timerCallback, ButtonModeChange);

    ButtonPwrStateInit(&button);

    // init led based off pwr_state
    LED_Set(button.pwr_state);

    // let user know STM32 is ready and running
    STM32_Ready(&uart2_msg);
}

void PollingRoutine(void)
{
    TimerCallbackPoll(&timerCallback);

	UART_DMA_ParseCircularBuffer(&uart2_msg);

	UART_ParseCommands(&uart2_msg);
}

void UART_ParseCommands(UART_DMA_Struct_t *msg)
{
	int status = 0;
	char *ptr;
	char retStr[UART_DMA_QUEUE_DATA_SIZE] = "OK";

	if(UART_DMA_RxMsgRdy(msg))
	{
		memset(&msg_copy_command, 0, sizeof(msg_copy_command));// clear
		memcpy(&msg_copy_command, msg->rx.msgToParse->data, strlen((char*)msg->rx.msgToParse->data) - 2); // remove CR/LF

		ptr = (char*)msg->rx.msgToParse->data;
		ToLower(ptr);

		if(strncmp(ptr, "version", strlen("version"))== 0)
		{
			sprintf(retStr, "%s", version);
		}
		else if(strncmp(ptr, "sleepmode", strlen("sleepmode"))== 0)
		{
			TimerCallbackTimerStart(&timerCallback, EnterSleepMode, 1000, TIMER_NO_REPEAT);
			button.pwr_state = sleepmode;
			LED_Set(button.pwr_state);
			flashData[0] = button.pwr_state;
			FLASH_WriteDoubleWord(&flashMsg);
		}
		else if(strncmp(ptr, "stopmode", strlen("stopmode"))== 0)
		{
			TimerCallbackTimerStart(&timerCallback, EnterStopMode, 1000, TIMER_NO_REPEAT);
			button.pwr_state = stopmode;
			LED_Set(button.pwr_state);
			flashData[0] = button.pwr_state;
			FLASH_WriteDoubleWord(&flashMsg);
		}
		else if(strncmp(ptr, "standbymode", strlen("standbymode"))== 0)
		{
			TimerCallbackTimerStart(&timerCallback, EnterStandbyMode, 1000, TIMER_NO_REPEAT);
			button.pwr_state = standbymode;
			LED_Set(button.pwr_state);
			flashData[0] = button.pwr_state;
			FLASH_WriteDoubleWord(&flashMsg);
		}
		else if(strncmp(ptr, "shutdownmode", strlen("shutdownmode"))== 0)
		{
			TimerCallbackTimerStart(&timerCallback, EnterShutdownMode, 1000, TIMER_NO_REPEAT);
			button.pwr_state = shutdownmode;
			LED_Set(button.pwr_state);
			flashData[0] = button.pwr_state;
			FLASH_WriteDoubleWord(&flashMsg);
		}

		else if(strncmp(ptr, "buttonsleep", strlen("buttonsleep"))== 0)
		{
			button.pwr_state = sleepmode;
			LED_Set(button.pwr_state);
			flashData[0] = button.pwr_state;
			FLASH_WriteDoubleWord(&flashMsg);
		}
		else if(strncmp(ptr, "buttonstop", strlen("buttonstop"))== 0)
		{
			button.pwr_state = stopmode;
			LED_Set(button.pwr_state);
			flashData[0] = button.pwr_state;
			FLASH_WriteDoubleWord(&flashMsg);
		}
		else if(strncmp(ptr, "buttonstandby", strlen("buttonstandby"))== 0)
		{
			button.pwr_state = standbymode;
			LED_Set(button.pwr_state);
			flashData[0] = button.pwr_state;
			FLASH_WriteDoubleWord(&flashMsg);
		}
		else if(strncmp(ptr, "buttonshutdown", strlen("buttonshutdown"))== 0)
		{
			button.pwr_state = shutdownmode;
			LED_Set(button.pwr_state);
			flashData[0] = button.pwr_state;
			FLASH_WriteDoubleWord(&flashMsg);
		}
		else
		{
			status = COMMAND_UNKNOWN;
		}

		// check return status
		if(status == NO_ACK)
		{
			return;
		}
		else if(status != 0) // other return status other than NO_ACK or NO_ERROR
		{
			UART_DMA_PrintError(msg, msg_copy_command, status);
		}
		else // NO_ERROR
		{
			UART_DMA_PrintReply(msg, msg_copy_command, retStr);
		}

		memset(&msg->rx.msgToParse->data, 0, UART_DMA_QUEUE_DATA_SIZE); // clear current buffer index

	}
}

void STM32_Ready(UART_DMA_Struct_t *msg)
{
	char str[UART_DMA_QUEUE_DATA_SIZE] = {0};

	sprintf(str, "STM32 Ready. Wake From S*** mode - %s", version);

	UART_DMA_NotifyUser(msg, str, strlen(str), true);
}


void ButtonDebounced(void)
{
	char str[32] = {0};

	switch(button.pwr_state)
	{
	case sleepmode:
		TimerCallbackTimerStart(&timerCallback, EnterSleepMode, 1000, TIMER_NO_REPEAT);
		break;
	case stopmode:
		TimerCallbackTimerStart(&timerCallback, EnterStopMode, 1000, TIMER_NO_REPEAT);
		break;
	case standbymode:
		TimerCallbackTimerStart(&timerCallback, EnterStandbyMode, 1000, TIMER_NO_REPEAT);
		break;
	case shutdownmode:
		TimerCallbackTimerStart(&timerCallback, EnterShutdownMode, 1000, TIMER_NO_REPEAT);
		break;
	default:
		TimerCallbackTimerStart(&timerCallback, EnterSleepMode, 1000, TIMER_NO_REPEAT);
		break;
	}

	ButtonPressed(1);

	sprintf(str, "EnteringS***Mode in 1 second");
	UART_DMA_NotifyUser(&uart2_msg, str, strlen(str), true);
}

/*
 * Description: Sequence through and change modes
 */
void ButtonModeChange(void)
{
	button.pwr_state += 1;
	if(button.pwr_state >= 4)
	{
		button.pwr_state = 0;
	}

	LED_Set(button.pwr_state);
	flashData[0] = button.pwr_state;
	FLASH_WriteDoubleWord(&flashMsg);
}

/*
 * Description: After a POR, we need to get pwr_state that was save in flash memory
 */
void ButtonPwrStateInit(Button_t *button)
{
	button->pwr_state = GetFlashData(FLASH_EEPROM_START_ADDRESS);
}

