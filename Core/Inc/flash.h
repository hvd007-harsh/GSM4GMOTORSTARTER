/*
 * flash.h
 *
 *  Created on: 21-Mar-2024
 *      Author: Lenovo
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_


#include "main.h"
#include "variables.h"

 void FlashEEPROM__WriteDataInToFlash(uint16_t *data, uint16_t size );
 void FlashEEPROM__ReadFromFlash (void);
uint16_t FlashEEPROM__ReadSingleAddress(uint32_t Adrs);


#endif /* INC_FLASH_H_ */
