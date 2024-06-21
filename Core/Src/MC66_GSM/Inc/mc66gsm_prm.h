/*
******************************************************************************
  * mc66gsm_prm.h
  *
  *  Created on: 27-May-2022
  *      Author: Niranjan
  ******************************************************************************/
/* Includes ------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
#define GSM_PAYLOAD_SIZE_MAX 63// 300
typedef enum{
	GSM_PASSED =0,
	GSM_FAILED,
	GSM_WAITING,
	GSM_IDEL
}GsmServicesStatus;

typedef enum{
	SIM800A =0,
	SIM800C,
	SIM900A,
	MC66,

	/* */
	TOTAL_GSM_TYPES

}GSM_TYPES;

typedef enum {

GSM_IDLE =0,
GSM_POWERUP,
GSM_POWERDOWN,
GSM_SOFT_RESTART,
GSM_MODEM_CHECK,// modem detection
GSM_ATRES_CONFIG,
GSM_ATCMD_CONFIG,// Enable / Disable Command Echo
GSM_SIM_DETECT,// SIM present or not
GSM_IMSI_SIM_CONFIG,
GSM_ICCID_SIM_CONFIG,
GSM_NETWORK_REG_CHECK,// if SIM present then need to check Registration
GSM_NETWORK_REG_START,
GSM_GET_NETWORK_SIGNAL,// if SIM present then need to check signal Qlality Poor,Medium,Good
GSM_GET_SERVICE_SYNC,// service provider name
GSM_NETWORK_TIME_SYNC,
GSM_GET_NETWORK_DATETIME,
GSM_SET_NETWORK_DATETIME,
GSM_SMS_CONFIG,
GSM_GPRS_CONFIG,
GSM_EMAIL_CONFIG,
GSM_GET_LOC,

//GSM_EMAIL_SENT, // required from , to , CC and Message
GSM_SMS_READ,
GSM_SMS_DELETE_ALL,
GSM_SMS_SENT,// mobile and message
GSM_CALL_READ,
//GSM_DIAL_CALL, // mobile num only missed or full ring
//GSM_MISS_CALL, // mobile num only
GSM_MISS_CALL_DIS,

GSM_RUNING_CALLBACK_SMS,
//GSM_RUNING_CALLBACK_CALL,
GSM_RUNING_CALLBACK,// no any command & Expected bytes

///
TOTAL_GSM_SERVICES

}GSM_SERVICES;


typedef struct __attribute__((__packed__)){

    GSM_TYPES service_api; // mc66 0r sim800 etc
    GSM_SERVICES service_opcode; //
    uint16_t timeout; // time out for this request
    uint8_t length; // total payload length
    uint8_t data[GSM_PAYLOAD_SIZE_MAX];

}GSM_SERVICES_REQUEST;

typedef struct {
	uint8_t *PhoneNum;
	uint8_t *sms_box;
}SMSPhTypes;

typedef struct __attribute__((__packed__)){
	uint8_t  Phone[13];
	uint8_t SMSPacketLen;
       // uint8_t Messagebox[];//GSM_PAYLOAD_SIZE_MAX];

}GSM_SMStypes;
extern uint8_t Messagebox[GSM_PAYLOAD_SIZE_MAX];//GSM_PAYLOAD_SIZE_MAX];
extern uint8_t SMSbox[GSM_PAYLOAD_SIZE_MAX];//GSM_PAYLOAD_SIZE_MAX];
typedef struct __attribute__((__packed__)){
	uint8_t * SenderEmail;
	uint8_t * ReceiverEmail;
	uint8_t * ccEmail1;
	uint8_t * ccEmail2;
	uint8_t * ccEmail3;
	uint8_t * ccEmail4;
	uint8_t  MessageboxEmail[];

}GSM_Emailtypes;

extern GSM_SMStypes OutgoingBox;
//extern _Bool SmsAppendMode;//
extern _Bool EmailAppendMode;
extern _Bool SmsLoadedByApps;
/* Private function prototypes -----------------------------------------------*/

void Mc66GSM__Initialize(void);
void Mc66GSM__Handler(void);
void Mc66GSM__AddDataU8(GSM_SERVICES_REQUEST * command, uint8_t u8);
void Mc66GSM__AddDataU8Buffer(GSM_SERVICES_REQUEST command, uint8_t *u8_buffer, uint8_t len);
void Mc66GSM__CreateRequest(GSM_SERVICES_REQUEST g_command);
void Mc66GSM__ClearGsmReceiveBuffer(void);
void MC66GSM__ForceSetService(GSM_SERVICES ser);
GsmServicesStatus M66__GetServiceStatus (void);
GSM_SERVICES MC66GSM__GetServiceRunning(void);
void Mc66GSM__ClearGsmReceiveBuffer(void);
uint8_t MC66__ResponseParser(uint8_t * GsmRx_buffer, uint8_t * expected_string );
void Mc66GSM__ResponseIncomingBuffering(uint8_t data);
__weak void Mc66GSM__SMSReadCallBack(uint8_t * ph , uint8_t * SmsBuf);
__weak void Mc66GSM__CallingServiceCallBack(uint8_t * ph , uint8_t * SmsBuf);
__weak void Mc66GSM__EMAILServiceCallBack(uint8_t * ph , uint8_t * SmsBuf);
uint8_t * Mc66GSM__GetNetworkDateTime(void);
uint8_t Mc66GSM__GetNetworkSignal(void);
uint8_t * Mc66GSM__GetNetworkName(void);

#ifndef APPSPECIFIC_DRIVERS_MC66_GSM_INC_MC66GSM_PRM_H_
#define APPSPECIFIC_DRIVERS_MC66_GSM_INC_MC66GSM_PRM_H_



#endif /* APPSPECIFIC_DRIVERS_MC66_GSM_INC_MC66GSM_PRM_H_ */
