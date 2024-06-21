/*
******************************************************************************
  * mc66gsm_prv.h
  *
  *  Created on: 27-May-2022
  *      Author: Niranjan
  ******************************************************************************/
/* Includes ------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* command & expected return string from GSM */
typedef enum{
	CMD_OFFSET=0,
	RES_OFFSET
}Offsets;

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
void Mc66GSM__ResponseBufferingStatus(void);
void Mc66GSM__SMSOutgoingHandler(void);
void Mc66GSM__MissCallOutgoingHandler(void);
void Mc66GSM__EMAILSOutgoingHandler(void);
HAL_StatusTypeDef M66__SendATCommand(uint8_t *message);
HAL_StatusTypeDef MC66__DispatchCommand( uint8_t * cmd, uint32_t cmd_timeout);
void MC66GSM__ForceSetService(GSM_SERVICES ser);
void Mc66GSM__CommandSenderResReceiverHandler(void);

#ifndef APPSPECIFIC_DRIVERS_MC66_GSM_INC_MC66GSM_PRV_H_
#define APPSPECIFIC_DRIVERS_MC66_GSM_INC_MC66GSM_PRV_H_



#endif /* APPSPECIFIC_DRIVERS_MC66_GSM_INC_MC66GSM_PRV_H_ */
