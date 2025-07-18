/*
 * GPIO_Handler.c
 *
 *  Created on: Jul 16, 2025
 *      Author: karl.yamashita
 */

#include "main.h"


/*
 * Description: Called from HAL_GPIO_EXTI_Rising_Callback or HAL_GPIO_EXTI_Falling_Callback
 */
void GPIO_Handler(uint16_t GPIO_Pin)
{
	GPIO_PinState pinState;

	if(GPIO_Pin == Button_Pin)
	{
		pinState = HAL_GPIO_ReadPin(Button_GPIO_Port, Button_Pin);
		if(!pinState) // pressed
		{
			// start 1000ms debounce before calling ButtonDebounced
			TimerCallbackTimerStart(&timerCallback, ButtonDebounced, 1000, TIMER_NO_REPEAT);
		}
		else // released
		{
			// Disable the TimerCallback so ButtonDebounced is not called
			TimerCallbackDisable(&timerCallback, ButtonDebounced);
			ButtonPressed(0);
		}
	}
	else if(GPIO_Pin == ModeChange_Pin)
	{
		pinState = HAL_GPIO_ReadPin(ModeChange_GPIO_Port, ModeChange_Pin);
		if(!pinState) // pressed
		{
			// start 1000ms debounce before calling ButtonDebounced
			TimerCallbackTimerStart(&timerCallback, ButtonModeChange, 10, TIMER_NO_REPEAT);
		}
		else // released
		{
			// Disable the TimerCallback so ButtonDebounced is not called
			TimerCallbackDisable(&timerCallback, ButtonModeChange);
			ButtonPressed(0);
		}
	}
}


/*
 * Nucleo green SOH led.
 */
void LED_Toggle(void)
{
	HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
}

/*
 * turn off SOH LED
 */
void LED_Off(void)
{
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
}

/*
 * Indicates what pwr mode is selected
 */
void LED_Set(int mode)
{
	switch(mode)
	{
	case 0:
		HAL_GPIO_WritePin(LED_Sleep_GPIO_Port, LED_Sleep_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_Stop_GPIO_Port, LED_Stop_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_Standby_GPIO_Port, LED_Standby_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_Shutdown_GPIO_Port, LED_Shutdown_Pin, GPIO_PIN_RESET);
		break;
	case 1:
		HAL_GPIO_WritePin(LED_Sleep_GPIO_Port, LED_Sleep_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_Stop_GPIO_Port, LED_Stop_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_Standby_GPIO_Port, LED_Standby_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_Shutdown_GPIO_Port, LED_Shutdown_Pin, GPIO_PIN_RESET);
		break;
	case 2:
		HAL_GPIO_WritePin(LED_Sleep_GPIO_Port, LED_Sleep_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_Stop_GPIO_Port, LED_Stop_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_Standby_GPIO_Port, LED_Standby_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_Shutdown_GPIO_Port, LED_Shutdown_Pin, GPIO_PIN_RESET);
		break;
	case 3:
		HAL_GPIO_WritePin(LED_Sleep_GPIO_Port, LED_Sleep_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_Stop_GPIO_Port, LED_Stop_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_Standby_GPIO_Port, LED_Standby_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_Shutdown_GPIO_Port, LED_Shutdown_Pin, GPIO_PIN_SET);
		break;
	}
}

/*
 * Indicator for button pressed/released state
 */
void ButtonPressed(int mode)
{
	HAL_GPIO_WritePin(ButtonPressed_GPIO_Port, ButtonPressed_Pin, mode);
}


