/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <ctype.h>
#define Nop() asm(" NOP ")

#include "ErrorStatus.h"
#include "RingBuffer.h"
#include "UART_DMA_Idle_Circular_Drv_STM32.h"
#include "Flash_ReadWrite_Drv_STM32.h"
#include "Flash_Handler.h"
#include "GPIO_Handler.h"
#include <HSI_ClockConfig.h>
#include "PowerMode.h"
#include "StringManip.h"
#include "TimerCallback.h"

#include "PollingRoutine.h"


extern UART_HandleTypeDef huart2;

extern UART_DMA_Struct_t uart2_msg;
extern TimerCallbackStruct timerCallback;
extern FlashWriteInit_t flashMsg;
extern uint8_t flashData[];

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void RunClockConfig(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ButtonPressed_Pin GPIO_PIN_12
#define ButtonPressed_GPIO_Port GPIOC
#define Button_Pin GPIO_PIN_13
#define Button_GPIO_Port GPIOC
#define Button_EXTI_IRQn EXTI4_15_IRQn
#define USART2_TX_Pin GPIO_PIN_2
#define USART2_TX_GPIO_Port GPIOA
#define USART2_RX_Pin GPIO_PIN_3
#define USART2_RX_GPIO_Port GPIOA
#define LED_GREEN_Pin GPIO_PIN_5
#define LED_GREEN_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define LED_Shutdown_Pin GPIO_PIN_15
#define LED_Shutdown_GPIO_Port GPIOA
#define LED_Sleep_Pin GPIO_PIN_0
#define LED_Sleep_GPIO_Port GPIOD
#define LED_Stop_Pin GPIO_PIN_3
#define LED_Stop_GPIO_Port GPIOD
#define LED_Standby_Pin GPIO_PIN_4
#define LED_Standby_GPIO_Port GPIOD
#define ModeChange_Pin GPIO_PIN_6
#define ModeChange_GPIO_Port GPIOB
#define ModeChange_EXTI_IRQn EXTI4_15_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
