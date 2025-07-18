/*
 * PowerMode.c
 *
 *  Created on: Jul 16, 2025
 *      Author: karl.yamashita
 */


#include "main.h"

/*
 * We can test the 4-5 possible Power states that we can enter.
 */

/*
 * Description: Any interrupt will exit Sleep mode
 * 				Be sure NVIC for GPIO EXTI is enabled
 */
void EnterSleepMode(void)
{
	HAL_SuspendTick();

	LED_Off();

	HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);

	HAL_ResumeTick();
}

/*
 * Description: Any interrupt will exit Stop mode.
 * 				Certain peripherals will exit Stop mode
 * 				Be sure NVIC for GPIO EXTI is enabled
 * 				Wake up system clock is HSISYS
 */
void EnterStopMode(void)
{
	UART_WakeUpTypeDef WakeUpSelection = {0};

	HAL_SuspendTick();

	LED_Off();

	// configure UART for Wakeup
	WakeUpSelection.WakeUpEvent = UART_WAKEUP_ON_STARTBIT;
	if (HAL_UARTEx_StopModeWakeUpSourceConfig(&huart2, WakeUpSelection) != HAL_OK)
	{
		Error_Handler();
	}
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_WUF);
	if(HAL_UARTEx_EnableStopMode(&huart2) != HAL_OK)
	{
		Error_Handler();
	}

	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

	HAL_UARTEx_DisableStopMode(&huart2);

	RunClockConfig(); // calls SystemClock_Config()

	HAL_ResumeTick();
}

/*
 * Description: WKUP will exit Standby mode. Not EXTI
 * 				RTC event
 * 				TAMP event
 * 				NRST pin, external
 * 				IWDG reset
 *
 * 			Wake up with HSISYS
 *
 * 			All clocks OFF except LSI and LSE
 *
 *
 */
void EnterStandbyMode(void)
{
	HAL_SuspendTick();

	LED_Off();

	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF2); // PWR_FLAG_WUF2
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN2_LOW); // PWR_WAKEUP_PIN2_LOW

	HAL_PWR_EnterSTANDBYMode();

	RunClockConfig(); // calls SystemClock_Config()

	HAL_ResumeTick();
}

/*
 * Description: WKUP will exit Shutdown mode. Not EXTI
 * 				RTC event
 * 				TAMP event
 * 				NRST pin, external
 * 				No IWDG
 *
 * 			Wake up with HSISYS
 *
 * 			All clocks OFF except LSE
 *
 * 			MR Off, LPR Off
 */
void EnterShutdownMode(void)
{
	HAL_SuspendTick();

	LED_Off();

	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF2); // PWR_FLAG_WUF2
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN2_LOW); // PWR_WAKEUP_PIN2_LOW

	HAL_PWREx_EnterSHUTDOWNMode();

	RunClockConfig(); // calls SystemClock_Config()

	HAL_ResumeTick();
}

/*
 * System clock frequency has to be decreased below 2 MHz before entering
  *        low power run mode
 */
void EnterLowPowerRunMode(void)
{
	HAL_PWREx_EnableLowPowerRunMode();
}
