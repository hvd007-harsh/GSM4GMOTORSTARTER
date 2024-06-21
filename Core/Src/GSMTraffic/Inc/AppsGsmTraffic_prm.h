/*
******************************************************************************
  * AppsGsmTraffic_prm.h
  *
  *  Created on: 27-May-2022
  *      Author: Niranjan
  ******************************************************************************/
/* Includes ------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
 
//Developer Firewall 
  
    
    
    
    
    
    
    
    
    
    
    
typedef enum{
	PH_NUMB_OFST=0,
	PH_STATUS1_OFST=11,
	PH_STATUS2_OFST=12,


}PhonePgOffset;

typedef enum{
	PHONE_ENABLE =0,// all operation disabled
	EVENT_SMS_ENABLE, // Events sms go or not.
	INTERVAL_SMS_ENABLE, // Events sms go or not.
	MISS_CALL_ENABLE, // dial miss call or not.
	REPEATE_SMS,// if SMS_ENABLED then need to check repeate enable or disables.
	NIGHT_MODE_ENABLE, // night mode ebanle or disabled.
        SETTING_ENABLE// Setting permission active or deactive.

}CellPermissionTypes;

typedef enum{
	REVSED=0,
	DEVELOPER,
	MASTER,
	OWNER, // admin
	OPE1,
	OPE2,
	OPE3,
	UNKNOWN_NUMBER,

	/* Total Phones */
	TOTAL_CELL


}PhoneTypes;

typedef enum{
	EMAIL_REVSED=0,
	UNKNOWN_EMAIL,

	/* Total Phones */
	TOTAL_EMAIL


}EmailTypes;

/* Private variables ---------------------------------------------------------*/
extern uint8_t Cell_Nums_Permission[TOTAL_CELL];
extern uint8_t *Email_Nums[TOTAL_CELL];
extern uint8_t *Cell_Nums[TOTAL_CELL];

/* Private function prototypes -----------------------------------------------*/
void AppsGsmTraffic__Initialization(void);
uint8_t * AppsGsmTraffic__GetPhoneNumber(PhoneTypes cell);
PhoneTypes AppsGsmTraffic__GetPhoneInfo(uint8_t *ph);

#ifndef APPSPECIFIC_APPLICATIONS_GSMTRAFFIC_INC_APPSGSMTRAFFIC_PRM_H_
#define APPSPECIFIC_APPLICATIONS_GSMTRAFFIC_INC_APPSGSMTRAFFIC_PRM_H_



#endif /* APPSPECIFIC_APPLICATIONS_GSMTRAFFIC_INC_APPSGSMTRAFFIC_PRM_H_ */
