/*
 * flash.c
 *
 *  Created on: 21-Mar-2024
 *      Author: Lenovo
 */


#include "flash.h"

#include "string.h"
#include "stdio.h"


uint16_t EE_buffer[150];
FLASH_EraseInitTypeDef EraseInitStruct; 

#define FLASH_USER_START_ADDR   0x0800F800   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     FLASH_USER_START_ADDR + FLASH_PAGE_SIZE   /* End @ of user Flash area */


uint32_t Address, data32 , PageError = 0 ; 
uint16_t DataBaseMirror[100],DataBase[100];


/**
* @brief FlashEEPROM__ReadSingleAddress
*/

uint16_t FlashEEPROM__ReadSingleAddress(uint32_t Adrs)
{
  Address =  Adrs+FLASH_USER_START_ADDR;
  data32 = *(__IO uint16_t *)Address;
  return data32;
}


/**
* @brief FlashEEPROM__ReadFromFlash
*/

 void FlashEEPROM__ReadFromFlash (void)
{

   uint16_t size=0;
   HAL_Delay(10);
   HAL_FLASH_Unlock();
   EE_buffer[size]= FlashEEPROM__ReadSingleAddress((size*2));
   size = 1;
   while(size < (uint16_t)(Phone_OPE3 + 9))
   {

	   EE_buffer[size]  = FlashEEPROM__ReadSingleAddress((size*2));
	   size = size+1;

   }
   HAL_FLASH_Lock();
   memcpy(DataBaseMirror,EE_buffer,sizeof(DataBaseMirror));
   memcpy(DataBase,DataBaseMirror,sizeof(DataBase));

}

/**
   * @brief FlashEEPROM__WriteDataInToFlash
   */

 void FlashEEPROM__WriteDataInToFlash(uint16_t *data, uint16_t size )
 {
 	uint16_t Lenght=0;
         HAL_FLASH_Unlock();

 	  /* Erase the user Flash area
 	    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

 	  /* Fill EraseInit structure*/
 	  EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
 	  EraseInitStruct.PageAddress = FLASH_USER_START_ADDR;
 	  EraseInitStruct.NbPages = (FLASH_USER_END_ADDR - FLASH_USER_START_ADDR) / FLASH_PAGE_SIZE;
 	  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
 	  {
 	    HAL_NVIC_SystemReset();
 	    /*
 	      Error occurred while page erase.
 	      User can add here some code to deal with this error.
 	      PageError will contain the faulty page and then to know the code error on this page,
 	      user can call function 'HAL_FLASH_GetError()'*/
 	    while(1);
 	  }

 	  Address = FLASH_USER_START_ADDR;

 	  while (Address < FLASH_USER_END_ADDR && Lenght <  size)
 	  {
 	    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, Address, data[Lenght]) == HAL_OK)
 	    {
 	      Address = Address + 2;
 	      Lenght++;
 	    }
 	    else
 	    {
 	      /* Error occurred while writing data in Flash memory.
 	         User can add here some code to deal with this error */
 	      HAL_NVIC_SystemReset();
 	      while (1)
 	      {
 	    	  ;
 	      }
 	    }
 	  }
 	  /* Lock the Flash to disable the flash control register access (recommended
 	     to protect the FLASH memory against possible unwanted operation) *********/
 	  HAL_FLASH_Lock();

 }
