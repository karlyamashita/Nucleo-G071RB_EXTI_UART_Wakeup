/*
 * GPIO_Handler.h
 *
 *  Created on: Jul 16, 2025
 *      Author: karl.yamashita
 */

#ifndef INC_GPIO_HANDLER_H_
#define INC_GPIO_HANDLER_H_

void GPIO_Handler(uint16_t GPIO_Pin);

void LED_Toggle(void);
void LED_Off(void);

void LED_Set(int mode);
void ButtonPressed(int mode);

#endif /* INC_GPIO_HANDLER_H_ */
