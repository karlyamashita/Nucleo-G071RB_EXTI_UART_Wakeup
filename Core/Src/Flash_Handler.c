/*
 * Flash_Handler.c
 *
 *  Created on: Jul 16, 2025
 *      Author: karl.yamashita
 */


#include "main.h"


uint8_t flashData[8] = {0};
FlashWriteInit_t flashMsg =
{
	.address = FLASH_EEPROM_START_ADDRESS, // 1F800
	.doubleWordAmount = 1,
	.data = flashData,
	.flashEraseInit.Banks = FLASH_BANK_1,
	.flashEraseInit.TypeErase = FLASH_TYPEERASE_PAGES,
	.flashEraseInit.Page = (FLASH_EEPROM_START_ADDRESS/FLASH_PAGE_SIZE),
	.flashEraseInit.NbPages = 1
};

int GetFlashData(uint32_t address)
{
	return FLASH_DATA_Read(address);
}
