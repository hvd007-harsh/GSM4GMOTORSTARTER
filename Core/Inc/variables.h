/*
 * variables.h
 *
 *  Created on: 20-Mar-2024
 *      Author: Lenovo
 */

#ifndef INC_VARIABLES_H_
#define INC_VARIABLES_H_

#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "stdint.h"
#include "flash.h"
//#include "led.h"
//#include "rtc1.h"
//#include "ext_eeprom.h"

#include "mc66gsm_prm.h"

#include "AppsGsmTraffic_prm.h"
#include "AppsTimer_prm.h"
   
   //#include "App.h"







extern _Bool EventLoad, StsLoad;
#define Calib_Setting 14
#define Phone_Developer 22
#define Phone_Master 22
#define Phone_OPE1   32
#define Phone_OPE2   42
#define Phone_OPE3   52
   
   
#define First_Addr  0x0003

#define Log_Addr 0x000A
 

#define INIT_CHAR '#'
 

typedef enum{
	CMD_IDEL =0,
	CMD_ACCEPTED,
	CMD_REJECTED,
}CmdStatus;




////////////////////////////


typedef struct {
uint16_t RPhase;
uint16_t YPhase;
uint16_t BPhase; 


}ADC_TYPES;


/*
typedef struct __attribute__((__packed__)){

	float AbsValue;// this will update from ADC
	uint8_t NAME[AIN_NAME_SIZE];
        uint8_t LcdSttausEv[4];
	int8_t min;
	int8_t max;
	uint8_t status_b;// enabled or disabled
	uint8_t EVENT_NAME[AIN_EVENT_NAME_SIZE];
	uint8_t clb_factor;

}AINStoreTypes;
*/

/*
typedef struct __attribute__((__packed__)){
	AINStoreTypes AIN1;
	AINStoreTypes AIN2;
	AINStoreTypes AIN3;
	AINStoreTypes AINH;

}AINx;
*/
typedef enum {
	UNKNOWN_REQUESTER=0,
	SYS_GSM
}CmdRequesterTypes;


/* This for operation name which will run due to above comand request */
typedef enum{
SETTINGS=0,
STS_RUN,


	// do all possible execution due command */
	NO_AUTH_EXEC

}SysExecAuthTypes;
        
typedef enum{

	CMD_REV=0,
	CMD_AIN,
	CMD_GSM, // APN
	CMD_INTERVALS,// logger, hooter, repaate SMS, relay ON
	CMD_SYSTEM,// date & time status & LC setting Download
	CMD_SMS, // all mobiles
	CMD_COMMON,
	SYS_RESET,
        ACK_FROM_USER,
	UNKNOWN_CMD,
	 ///
	 TOTAL_CMDS


}Cmd_Type;




typedef enum{
  MASTER_USER= 0,
  OPE1_USER ,
  OPE2_USER, 
  OPE3_USER,
}AllUsers;


 


typedef struct __attribute__((__packed__)){

  uint8_t Notification_Type[10];
  uint8_t Notification_Status[20];
} Notification_Message_Handler_t;



/*
typedef struct __attribute__((__packed__)){

	uint8_t CmdStr[8];
	uint8_t DeviceID[9];
	DownloadReqTypes Dtyp;
	uint8_t txn_id[5];
	uint8_t AIN1_V[4];
	uint8_t AIN1_S[4];
	uint8_t AIN2_V[4];
	uint8_t AIN2_S[4];
	uint8_t AIN3_V[4];
	uint8_t AIN3_S[4];
	uint8_t Hum[4];
	uint8_t HumS[4];


}FeedbackTypes;
*/
/*
extern Parameter_t Params; 
extern Calib_Parameter_t CParams;
*/

extern uint8_t Mode;
extern uint8_t Second;
extern uint8_t Minute; 
extern uint8_t Hour;
extern uint16_t DataIn[200];
extern uint8_t OWNER_NUM[10];
extern uint8_t Phone_Master_Num[10]; 
extern uint8_t Phone_OP1_Num[10]; 
extern uint8_t Phone_OP2_Num[10];
extern uint8_t Phone_OP3_Num[10];

extern _Bool NOTIFICATION_FLAG;


extern uint16_t EE_buffer[150];
extern uint8_t buf[100];


//extern float YPH_DG, BPH_DG, RPH_DG, RPH_MAINS, YPH_MAINS, BPH_MAINS, BATT_DG,
//		BATT_BTS, BATT_CS, TEMP_LM35_CS;
//extern Lcd_HandleTypeDef lcd;
//extern Fault_Detection_t fault; 
//extern Setting_Mode_t setting;
//extern Fault_Message_Handler_t Fault_Messages;
//extern Notification_Message_Handler_t Notification_Messages;
//extern uint8_t Pass_Counter;*/
extern uint8_t UserPhoneGroup; 
extern uint8_t First_Addr_Read_EEPROM[4];

/////////////////////////////// NOTIFICATIOn
extern _Bool NOTIFICATION_FLAG;


///GSM PART 
extern _Bool EventSms ;
extern _Bool SmsloadingAppend ; 
extern _Bool SmsAppendMode;
extern _Bool Notification_Load;

///////////////////SMS HANDLER PARAM 
extern _Bool Sent_Completed;


//////////////////////////////
extern _Bool Sms_Master; 
extern _Bool Sms_User1; 
extern _Bool Sms_User2;
extern _Bool Sms_User3;

extern uint8_t Log_Timeout;
extern _Bool Log_Reception_Started; 


//Developer firewall 
//extern _Bool SystemStopedByDeveloper = 0 ;  



///DISPLAY PART 
extern uint8_t aShowTime[13];            //Time Buffer for RTC
extern uint8_t aShowDate[13];
extern uint8_t aShowFeedbackDateTime[14];


#endif /* INC_VARIABLES_H_ */
