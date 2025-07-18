/*
 * PollingRoutine.h
 *
 *  Created on: Oct 24, 2023
 *      Author: karl.yamashita
 *
 *
 *      Template
 */

#ifndef INC_POLLINGROUTINE_H_
#define INC_POLLINGROUTINE_H_


/*

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <ctype.h>
#define Nop() asm(" NOP ")

//#include "RingBuffer.h"
//#include "TimerCallback.h"

#include "PollingRoutine.h"

*/
#ifndef __weak
#define __weak __attribute__((weak))
#endif


#define FLASH_EEPROM_START_ADDRESS 0x0801F800

enum
{
	sleepmode,
	stopmode,
	standbymode,
	shutdownmode
};

typedef struct
{
	uint8_t pwr_state;
	uint32_t time;
}Button_t;


void PollingInit(void);
void PollingRoutine(void);

void UART_ParseCommands(UART_DMA_Struct_t *msg);
void STM32_Ready(UART_DMA_Struct_t *msg);


void ButtonDebounced(void);
void ButtonModeChange(void);
void ButtonPwrStateInit(Button_t *button);



/* copy these 2 to main.h
extern UART_HandleTypeDef huart2;
extern TimerCallbackStruct timerCallback;
*/

#endif /* INC_POLLINGROUTINE_H_ */
