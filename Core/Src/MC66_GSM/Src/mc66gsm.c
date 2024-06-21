/*
******************************************************************************
  * MC66.c
  *
  *  Created on: 27-May-2022
  *      Author: Niranjan
  ******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_hal.h"
#include "variables.h"
#include "mc66gsm_prv.h"
/* Private variables ---------------------------------------------------------*/
static uint8_t Outgoing_phase=0;
uint8_t PacketPending = 0;
uint8_t PacketOffset = 0;
uint8_t PowerOnSms = 1;
static _Bool SysOnFirstTime =0;
extern UART_HandleTypeDef huart2;
uint8_t Messagebox[GSM_PAYLOAD_SIZE_MAX];
uint8_t SMSbox[GSM_PAYLOAD_SIZE_MAX];
uint8_t DummyScrach[20] = {0};
uint8_t GsmDateTime[20] = "27-05-2022/14:50:10";
static uint8_t NetworkNameGSM[11]="Wait.....";//{'W','a','i','t'};//'V','o','d','a','p','h','o','n','e','\0'}; // default
uint8_t  SignalQlality = 0;
uint8_t aShowFeedbackDateTime[14];

//_Bool EventLoad = 0 , StsLoad = 0 ;


uint8_t aShowTime[13];            //Time Buffer for RTC
uint8_t aShowDate[13];


uint8_t GSM_SMS_PHONE[10];
uint8_t * GSM_CMD_LISTS[TOTAL_GSM_SERVICES][2];
_Bool StartupSmsdirect = 0;

#ifdef SMS_MAP_ON_USB
	GSM_SERVICES GsmServices = 0;//GSM_POWERUP;
#else
	GSM_SERVICES GsmServices = GSM_POWERUP;//GSM_POWERUP;
#endif

uint8_t GSM_RESPONSE_BUFFER[150];
uint8_t GSM_GET_SMS[70];
static uint8_t ServiceRetry =0, OutServiceRetry =0;;
static uint8_t GSM_ResponseByteCounter;
GsmServicesStatus StartSenderResReceiver =GSM_IDEL;
static _Bool  StartCaptureGSM_Respone =1;// 1 means respose not came.
GsmServicesStatus ServicesStatus =GSM_IDEL;
GSM_SERVICES_REQUEST GsmCommand;
GSM_SMStypes OutgoingBox;
_Bool GsmBus_Busy=0;
_Bool Cmd_Dispatched =0;
_Bool SmsAppendMode = 0;//
_Bool  EmailAppendMode = 0;//
_Bool SmsLoadedByApps = 0;
uint8_t Res_pos=0;
_Bool EventSms = 0 ; 
_Bool SmsloadingAppend = 0 ;


/* Private function prototypes -----------------------------------------------*/


/**
  * @brief Mc66GSM__GetNetworkDateTime
  * @retval None
  */
uint8_t * Mc66GSM__GetNetworkDateTime(void)
{
	return GsmDateTime;

}

/**
  * @brief Mc66GSM__GetNetworkSignal
  * @retval None
  */
uint8_t Mc66GSM__GetNetworkSignal(void)
{
	return SignalQlality;

}

/**
  * @brief M66__GetServiceStatus
  * @retval None
  */
uint8_t * Mc66GSM__GetNetworkName(void)
{
	return NetworkNameGSM;

}

/**
  * @brief M66__GetServiceStatus
  * @retval None
  */
GsmServicesStatus M66__GetServiceStatus (void)
{
  GsmServicesStatus res = ServicesStatus;
  ServicesStatus = GSM_WAITING;/// clear status
  return res;
  
}

/**
  * @brief M66__SendATCommand
  * @retval None
  */
HAL_StatusTypeDef M66__SendATCommand(uint8_t *message)
{
	return HAL_UART_Transmit(&huart1,message,strlen((const char *)message),2000);
}

/**
  * @brief MC66__DispatchCommand
  * @retval None
  */
HAL_StatusTypeDef MC66__DispatchCommand( uint8_t * cmd, uint32_t cmd_timeout)
{
// load command max response timeout
	AppsTimer__Reload(GSM_RESPONSE_TIMER,cmd_timeout);       
        ServicesStatus =GSM_FAILED;
	GSM_ResponseByteCounter =0;//
	StartCaptureGSM_Respone = 1;// receiver start buffering incoming data from GSM
	Mc66GSM__ClearGsmReceiveBuffer();// Clear GSM Response buffer
	return M66__SendATCommand(cmd);// send to GSM
}

/**
  * @brief MC66__ResponseParser
  * @retval None
  */
uint8_t MC66__ResponseParser(uint8_t * GsmRx_buffer, uint8_t * expected_string )
{
	uint8_t i=0;

	uint8_t strl = strlen((const char *) GsmRx_buffer);// actual rx buffer lenght
	uint8_t stre = strlen((const char *) expected_string);// string expected


	for (i=0; i<strl; i++)
	{
		if( 0 == strncmp((const char *)GsmRx_buffer+i, (const char *)expected_string, stre) )
		{
			 return (i+stre);//return buffer index where your response usable data string start.
		}
	}


  return NULL;// invalid
}


/**
  * @brief Mc66GSM__Initialize
  * @retval None
  */
void Mc66GSM__Initialize(void)
{        

      //      //

	GSM_CMD_LISTS[GSM_SOFT_RESTART][CMD_OFFSET] = "AT+QPOWD=1\r\n";
	GSM_CMD_LISTS[GSM_SOFT_RESTART][RES_OFFSET] = "NORMAL POWER DOWN";

	GSM_CMD_LISTS[GSM_MODEM_CHECK][CMD_OFFSET] = "ATE0\r\n";
	GSM_CMD_LISTS[GSM_MODEM_CHECK][RES_OFFSET] = "OK";


	GSM_CMD_LISTS[GSM_ATRES_CONFIG][CMD_OFFSET] = "AT\r\n";
	GSM_CMD_LISTS[GSM_ATRES_CONFIG][RES_OFFSET] = "OK";

	GSM_CMD_LISTS[GSM_ATCMD_CONFIG][CMD_OFFSET] = "ATE0\r\n";
	GSM_CMD_LISTS[GSM_ATCMD_CONFIG][RES_OFFSET] = "OK";

	GSM_CMD_LISTS[GSM_SIM_DETECT][CMD_OFFSET] ="AT+CPIN?\r\n" ;
	GSM_CMD_LISTS[GSM_SIM_DETECT][RES_OFFSET] ="READY" ;

	GSM_CMD_LISTS[GSM_IMSI_SIM_CONFIG][CMD_OFFSET] ="AT+CIMI\r\n" ;
	GSM_CMD_LISTS[GSM_IMSI_SIM_CONFIG][RES_OFFSET] ="OK" ;

	GSM_CMD_LISTS[GSM_ICCID_SIM_CONFIG][CMD_OFFSET] =" AT+QCCID\r\n" ;
	GSM_CMD_LISTS[GSM_ICCID_SIM_CONFIG][RES_OFFSET] ="OK" ;

	GSM_CMD_LISTS[GSM_NETWORK_REG_CHECK][CMD_OFFSET] ="AT+CREG?\r\n" ;
	GSM_CMD_LISTS[GSM_NETWORK_REG_CHECK][RES_OFFSET] ="+CREG:";

	GSM_CMD_LISTS[GSM_NETWORK_REG_START][CMD_OFFSET] ="AT+CREG=1\r\n";
	GSM_CMD_LISTS[GSM_NETWORK_REG_START][RES_OFFSET] ="OK" ;

	GSM_CMD_LISTS[GSM_GET_NETWORK_SIGNAL][CMD_OFFSET] ="AT+CSQ\r\n" ;
	GSM_CMD_LISTS[GSM_GET_NETWORK_SIGNAL][RES_OFFSET] ="+CSQ:" ;

	GSM_CMD_LISTS[GSM_GET_SERVICE_SYNC][CMD_OFFSET] =" AT+COPS?\r\n";
	GSM_CMD_LISTS[GSM_GET_SERVICE_SYNC][RES_OFFSET] ="+COPS:" ;
        
        GSM_CMD_LISTS[GSM_NETWORK_TIME_SYNC][CMD_OFFSET] = "AT+CTZU=1\r\n";//"AT+QNTP=\"time.nist.gov\",123\r\n";//"AT+QNTP=asia.pool.ntp.org,123\r\n";//"AT+QNTP=time.windows.com,22\r\n";//"AT+CTZU=1\r\n";///"AT+QNITZ=1\r\n";
        GSM_CMD_LISTS[GSM_NETWORK_TIME_SYNC][RES_OFFSET] = "OK";

	GSM_CMD_LISTS[GSM_GET_NETWORK_DATETIME][CMD_OFFSET]	 = "AT+CCLK?\r\n";
	GSM_CMD_LISTS[GSM_GET_NETWORK_DATETIME][RES_OFFSET]	 = "+CCLK:";

	GSM_CMD_LISTS[GSM_SET_NETWORK_DATETIME][CMD_OFFSET]="AT+CCLK=";
	GSM_CMD_LISTS[GSM_SET_NETWORK_DATETIME][RES_OFFSET]="OK";

	GSM_CMD_LISTS[GSM_SMS_CONFIG][CMD_OFFSET]	= "AT+CMGF=1\r\n";
	GSM_CMD_LISTS[GSM_SMS_CONFIG][RES_OFFSET]	= "OK";

	GSM_CMD_LISTS[GSM_GPRS_CONFIG][CMD_OFFSET]	 = " ";
	GSM_CMD_LISTS[GSM_GPRS_CONFIG][RES_OFFSET]	 = " ";

	GSM_CMD_LISTS[GSM_EMAIL_CONFIG][CMD_OFFSET]	 = " ";
	GSM_CMD_LISTS[GSM_EMAIL_CONFIG][RES_OFFSET]	 = " ";

	GSM_CMD_LISTS[GSM_GET_LOC][CMD_OFFSET]	 = " ";
	GSM_CMD_LISTS[GSM_GET_LOC][RES_OFFSET]	 = " ";

	GSM_CMD_LISTS[GSM_SMS_READ][CMD_OFFSET]	 = "AT+CMGR=1\r\n";
	GSM_CMD_LISTS[GSM_SMS_READ][RES_OFFSET]	 = "+CMGR:";

	GSM_CMD_LISTS[GSM_SMS_DELETE_ALL][CMD_OFFSET]	 = " AT+CMGD=1,4\r\n";
	GSM_CMD_LISTS[GSM_SMS_DELETE_ALL][RES_OFFSET]	 = "OK";

	GSM_CMD_LISTS[GSM_SMS_SENT][CMD_OFFSET]	 = "AT+CMGF=1\r\n";
	GSM_CMD_LISTS[GSM_SMS_SENT][RES_OFFSET]	 = "OK";

	GSM_CMD_LISTS[GSM_CALL_READ][CMD_OFFSET]	 = " ";
	GSM_CMD_LISTS[GSM_CALL_READ][RES_OFFSET]	 = " ";

	GSM_CMD_LISTS[GSM_MISS_CALL_DIS][CMD_OFFSET] = "ATH\r\n";
	GSM_CMD_LISTS[GSM_MISS_CALL_DIS][RES_OFFSET] = "OK";

	AppsTimer__ReloadSlow(GSM_HEALTH_CHECK_TIMER,300);//
#ifdef DEBUG_MODE
	LCD_Line(0,2);
	LCD_Puts((uint8_t *)__FUNCTION__);
	HAL_Delay(InitDelaySeeTime);
	#endif
}


/**
  * @brief Mc66GSM__AddDataU8Buffer
  * @retval None
  */
void Mc66GSM__AddDataU8Buffer(GSM_SERVICES_REQUEST command, uint8_t *u8_buffer, uint8_t len)
{
    if(len > (GSM_PAYLOAD_SIZE_MAX - command.length))
    {
            len = (uint8_t)(GSM_PAYLOAD_SIZE_MAX - command.length);
    }
    memcpy(&command.data[command.length],u8_buffer,len);
    command.length += len;
}


/**
  * @brief Mc66GSM__AddDataU8
  * @retval None
  */
void Mc66GSM__AddDataU8(GSM_SERVICES_REQUEST * command, uint8_t u8)
{
    if (command != (GSM_SERVICES_REQUEST *)NULL)
    {
        if (command->length < GSM_PAYLOAD_SIZE_MAX)
        {
            command->data[command->length ] = u8;
            command->length += 1;
        }
    }
}


/**
  * @brief Mc66GSM__CreateRequest
  * @retval None
  */
void Mc66GSM__CreateRequest(GSM_SERVICES_REQUEST g_command)
{
	GsmCommand = g_command; // this will use by GSM handler

	if(GsmCommand.service_api == MC66)
	{
		if(GsmCommand.service_opcode < GSM_MODEM_CHECK)
		{
			MC66GSM__ForceSetService(GsmCommand.service_opcode);

		}
		else if(GsmCommand.service_opcode == GSM_SMS_SENT)
		{
                  /*
			memcpy(OutgoingBox.Phone, GsmCommand->data,10);

			// here require to fill as per feedback requirement 
			memcpy(Messagebox, &GsmCommand->data[10],10);
                        memcpy(OutgoingBox.Phone, &GsmCommand->data[10],10);
*/

			MC66GSM__ForceSetService(GsmCommand.service_opcode);
			SmsAppendMode = 0;//
			SmsLoadedByApps = 0;
			SmsloadingAppend = 1;
		}
	}

}

/**
  * @brief MC66GSM__GetServiceRunning
  * @retval None
  */

GSM_SERVICES MC66GSM__GetServiceRunning(void)
{
	return GsmServices;
}

/**
  * @brief MC66GSM__ForceSetService
  * @retval None
  */
void MC66GSM__ForceSetService(GSM_SERVICES ser)
{
	if(ser > GSM_POWERDOWN)
	{
		StartSenderResReceiver = GSM_WAITING;
	}
	else
	{
		StartSenderResReceiver = GSM_PASSED;// i.e relpy from mc66 not expected

	}
	GsmServices = ser;
	ServicesStatus = GSM_FAILED;// failed
	ServiceRetry =0;
        GsmBus_Busy = 0;
        Cmd_Dispatched = 0;
        Outgoing_phase =0;
}

/**
  * @brief Mc66GSM__ClearGsmReceiveBuffer
  * @retval None
  */

void Mc66GSM__ClearGsmReceiveBuffer(void)
{
	
	uint8_t index=0;
	for(index=0; index<100; index++)
	{
		GSM_RESPONSE_BUFFER[index] = NULL;// assigned NULL as clear
	}
	GSM_ResponseByteCounter = 0;

}

/**
  * @brief Mc66GSM__CommandSenderResReceiverHandler
  * @retval None
  */

void Mc66GSM__CommandSenderResReceiverHandler(void)
{
	if(StartSenderResReceiver == GSM_WAITING)
	{
		if(GsmBus_Busy == 0)// wait untill  GSM Bus free
			{
				if(Cmd_Dispatched == 0)
				{
					GsmCommand.timeout = 25000;//500 ms for all

					if(HAL_OK == MC66__DispatchCommand(GSM_CMD_LISTS[GsmServices][CMD_OFFSET],\
							GsmCommand.timeout))
					{
						Cmd_Dispatched = 1;
					}
				}
				else
				{
					if(AppsTimer__Expired(GSM_RESPONSE_TIMER) || StartCaptureGSM_Respone == 0)
					{
						Cmd_Dispatched = 0;
						// parse data here
						Res_pos = MC66__ResponseParser(GSM_RESPONSE_BUFFER,GSM_CMD_LISTS[GsmServices][RES_OFFSET]);
						if(NULL != Res_pos)
						{
							ServiceRetry =0;
							StartSenderResReceiver =GSM_PASSED;// response rx done
						}
						else
						{
							if(ServiceRetry++ < 3)
							{

							}
							else
							{
								ServiceRetry =0;
								StartSenderResReceiver = GSM_FAILED;// response rx failed
							}
						}
					}
				}
			}

	}

}



/**
  * @brief Mc66GSM__Handler
  * @retval None
  */
_Bool SysInitPase = 0;
uint8_t PoWerOn_Off_retry = 0;
void Mc66GSM__Handler(void)
{
	Mc66GSM__ResponseBufferingStatus();
	Mc66GSM__CommandSenderResReceiverHandler();

	switch(GsmServices)
	{
	case GSM_POWERUP:
		SysInitPase = 1;
#ifdef LED_PROD_MODE
		LED_SYS_OK(GPIO_PIN_RESET);
#endif

		HAL_GPIO_WritePin(GSM_PWR_GPIO_Port, GSM_PWR_Pin, GPIO_PIN_SET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(GSM_PWR_GPIO_Port, GSM_PWR_Pin, GPIO_PIN_RESET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(GSM_PWR_GPIO_Port, GSM_PWR_Pin, GPIO_PIN_SET);
		AppsTimer__Reload(GSM_POWER_UP_STABILITY_TIMER,10000);
		ServicesStatus = GSM_PASSED;// passed
		MC66GSM__ForceSetService(GSM_POWERDOWN);


		break;

	case GSM_POWERDOWN:
		if(AppsTimer__Expired(GSM_POWER_UP_STABILITY_TIMER) == EXPIRED)
		{
			GsmServices = GSM_POWERUP;// same sequence
			ServicesStatus = GSM_PASSED;// passed
			MC66GSM__ForceSetService(GSM_MODEM_CHECK);
		}

		break;

	case GSM_SOFT_RESTART:
		if(StartSenderResReceiver == GSM_PASSED)
		{

			ServicesStatus = GSM_PASSED;// passed
			MC66GSM__ForceSetService(GSM_IDLE);

		}
		else if(StartSenderResReceiver == GSM_FAILED)
		{

			MC66GSM__ForceSetService(GSM_IDLE);
			ServicesStatus = GSM_FAILED;// failed
			ServiceRetry =0;
		}



		break;

	case GSM_ATCMD_CONFIG:
		if(StartSenderResReceiver == GSM_PASSED)
		{
			ServicesStatus = GSM_PASSED;// passed
			MC66GSM__ForceSetService(GSM_SIM_DETECT);

		}
		else if(StartSenderResReceiver == GSM_FAILED)
		{

			MC66GSM__ForceSetService(GSM_IDLE);
			ServicesStatus = GSM_FAILED;// failed
			ServiceRetry =0;
		}


		break;

	case GSM_MODEM_CHECK:
		if(StartSenderResReceiver == GSM_PASSED)
		{

			ServicesStatus = GSM_PASSED;// passed
 			MC66GSM__ForceSetService(GSM_ATCMD_CONFIG);

		}
		else if(StartSenderResReceiver == GSM_FAILED)
		{

			MC66GSM__ForceSetService(GSM_MODEM_CHECK);// wait for modem
			ServicesStatus = GSM_FAILED;// failed
			ServiceRetry =0;
		}


		break;

	case GSM_SIM_DETECT:
#ifdef LED_PROD_MODE
	LED_SYS_OK(GPIO_PIN_RESET);
#endif
		if(StartSenderResReceiver == GSM_PASSED)
		{

			/*
			 *
			 * Check here SIM detected or not
			 * if not detected then restart the modem( Normal Power Down) and wait for 20-30 Second
			 * then check again SIM status if still same then repeat once more and
			 * again still same then Give error to display as:
			 * "Insert activated SIM properly then ON/OFF the system" in regular interval
			 *
			 * and GSM Init failed and user not able use their services.
			 */

			ServicesStatus = GSM_PASSED;// passed
			MC66GSM__ForceSetService(GSM_IMSI_SIM_CONFIG);
                        PoWerOn_Off_retry = 0;

		}
		else if(StartSenderResReceiver == GSM_FAILED)
		{
                  memset(NetworkNameGSM,0,sizeof(NetworkNameGSM));
                if(PoWerOn_Off_retry++ > 2)
                {
                        PoWerOn_Off_retry = 0;
			MC66GSM__ForceSetService(GSM_IDLE);
			ServicesStatus = GSM_FAILED;// failed
                }
                else MC66GSM__ForceSetService(GSM_POWERUP);
                
                ServiceRetry =0;
                memcpy(NetworkNameGSM,"NOT SIM ",7);
		}

		break;
        case  GSM_IMSI_SIM_CONFIG:
		if(StartSenderResReceiver == GSM_PASSED)
		{
			ServicesStatus = GSM_PASSED;// passed
			MC66GSM__ForceSetService(GSM_ICCID_SIM_CONFIG);

		}
		else if(StartSenderResReceiver == GSM_FAILED)
		{

			MC66GSM__ForceSetService(GSM_IDLE);// wait for modem
			ServicesStatus = GSM_FAILED;// failed
			ServiceRetry =0;
		}

          
          break;
          
         case  GSM_ICCID_SIM_CONFIG:
		if(StartSenderResReceiver == GSM_PASSED)
		{
			ServicesStatus = GSM_PASSED;// passed
			MC66GSM__ForceSetService(GSM_NETWORK_REG_CHECK);
                      AppsTimer__Reload(GSM_POWER_UP_STABILITY_TIMER,600000);

		}
		else if(StartSenderResReceiver == GSM_FAILED)
		{

			MC66GSM__ForceSetService(GSM_IDLE);// wait for modem
			ServicesStatus = GSM_FAILED;// failed
			ServiceRetry =0;
		}

          
          break;

	case GSM_NETWORK_REG_CHECK:
		if(StartSenderResReceiver == GSM_PASSED)
			{
			/*
			 * Here need to check Registration done or not
			 * if not execute registration process.
			 *
			 * if yes then go for next case.
			 *
			 */

				if(GSM_RESPONSE_BUFFER[Res_pos+1] >= '0' && GSM_RESPONSE_BUFFER[Res_pos+1] <= '5')
				{
					if((GSM_RESPONSE_BUFFER[Res_pos+1+2] =='5') || (GSM_RESPONSE_BUFFER[Res_pos+1+2] =='1' ))
					{
						ServicesStatus = GSM_PASSED;// passed
						MC66GSM__ForceSetService(GSM_GET_NETWORK_SIGNAL);
						 //BuzzerTone__SetKeyTonePlay(WARNINGALARM_TONE,0);
                                                 memcpy(NetworkNameGSM,"NET REG",7);
					}
					else
					{
                                          memcpy(NetworkNameGSM,"Search...",10);
						if( GSM_RESPONSE_BUFFER[Res_pos+1+2] == '0' )
						{

						}
						else if( GSM_RESPONSE_BUFFER[Res_pos+1+2] == '2' )
						{

						}
						else if( GSM_RESPONSE_BUFFER[Res_pos+1+2] == '3' )
						{

						}
						else if( GSM_RESPONSE_BUFFER[Res_pos+1+2] == '4' )
						{

						}
                                                MC66GSM__ForceSetService(GSM_NETWORK_REG_CHECK);
					}
                                        if(AppsTimer__Expired(GSM_POWER_UP_STABILITY_TIMER) == EXPIRED)
                                        {
                                          memset(NetworkNameGSM,0,sizeof(NetworkNameGSM));                                         
                                          GsmServices = GSM_POWERUP;// same sequence 
                                        }
				}
				else
				{
					StartSenderResReceiver = GSM_FAILED;
                                         MC66GSM__ForceSetService(GSM_NETWORK_REG_CHECK);
                                         memcpy(NetworkNameGSM,"REG FAIL",8);
				}
			}
			else if(StartSenderResReceiver == GSM_FAILED)
			{

				MC66GSM__ForceSetService(GSM_IDLE);
				ServicesStatus = GSM_FAILED;// failed
				ServiceRetry =0;
			}

		break;

	case GSM_NETWORK_REG_START:
		if(StartSenderResReceiver == GSM_PASSED)
		{


			ServicesStatus = GSM_PASSED;// passed
			MC66GSM__ForceSetService(GSM_NETWORK_REG_CHECK);

		}
		else if(StartSenderResReceiver == GSM_FAILED)
		{

			MC66GSM__ForceSetService(GSM_IDLE);
			ServicesStatus = GSM_FAILED;// failed
			ServiceRetry =0;
		}

		break;

	case GSM_GET_NETWORK_SIGNAL:
		if(StartSenderResReceiver == GSM_PASSED)
		{

			/*
			*
			* Here check signal from GSM_RESPONSE_BUFFER[Res_pos+]
			*
			*
			*/if((GSM_RESPONSE_BUFFER[Res_pos+2] != ',')&& (GSM_RESPONSE_BUFFER[Res_pos+3] == ','))
			{
				SignalQlality = (((GSM_RESPONSE_BUFFER[Res_pos+1]-48) * 10)\
				+ GSM_RESPONSE_BUFFER[Res_pos+2]-48)*3;
			}
			else SignalQlality = ((GSM_RESPONSE_BUFFER[Res_pos+1]-48)*3);

			ServicesStatus = GSM_PASSED;// passed
			MC66GSM__ForceSetService(GSM_GET_SERVICE_SYNC);

		}
		else if(StartSenderResReceiver == GSM_FAILED)
		{

			MC66GSM__ForceSetService(GSM_IDLE);
			ServicesStatus = GSM_FAILED;// failed
			ServiceRetry =0;
		}


		break;

	case GSM_GET_SERVICE_SYNC: // operator name
		if(StartSenderResReceiver == GSM_PASSED)
		{


			/*
			* Here network name from GSM_RESPONSE_BUFFER[Res_pos+]
			*/
			 uint8_t ntework_len = 0;
                         memset(NetworkNameGSM,0,sizeof(NetworkNameGSM));
                         if(GSM_RESPONSE_BUFFER[Res_pos+5] == '"')
                         {
                           while(GSM_RESPONSE_BUFFER[Res_pos+6+ntework_len] != '"')
                           {
                             if(ntework_len++ > (sizeof(NetworkNameGSM)-2))
                             {
                               break;
                             }
                             
                           }                           
                         }
						 if(ntework_len > 10)
						 ntework_len = 10;
						 memset(NetworkNameGSM,0,sizeof(NetworkNameGSM));
						memcpy(NetworkNameGSM,&GSM_RESPONSE_BUFFER[Res_pos+6],ntework_len);
						NetworkNameGSM[ntework_len] = '\0';
						NetworkNameGSM[10] = '\0';// fixed due to display show
						ntework_len = 0;

			ServicesStatus = GSM_PASSED;// passed
			MC66GSM__ForceSetService(GSM_NETWORK_TIME_SYNC);//GSM_GET_NETWORK_DATETIME);
                        

		}
		else if(StartSenderResReceiver == GSM_FAILED)
		{

			MC66GSM__ForceSetService(GSM_IDLE);
			ServicesStatus = GSM_FAILED;// failed
			ServiceRetry =0;
		}


		break;
                
        case GSM_NETWORK_TIME_SYNC:
          
          if(StartSenderResReceiver == GSM_PASSED)
		{

			ServicesStatus = GSM_PASSED;// passed
			MC66GSM__ForceSetService(GSM_GET_NETWORK_DATETIME);

		}
		else if(StartSenderResReceiver == GSM_FAILED)
		{

			MC66GSM__ForceSetService(GSM_NETWORK_TIME_SYNC);// wait for modem
			ServicesStatus = GSM_FAILED;// failed
			ServiceRetry =0;
		}

          break;

	case GSM_GET_NETWORK_DATETIME:
		if(StartSenderResReceiver == GSM_PASSED)
		{


			/*
			* Here date&time from GSM_RESPONSE_BUFFER[Res_pos+]
			*/
			 GSM_RESPONSE_BUFFER[Res_pos+1+20] = '\0';
             memcpy(GsmDateTime,&GSM_RESPONSE_BUFFER[Res_pos+2],18);
             
             memcpy(aShowDate ,GsmDateTime ,8);
               memcpy(aShowTime ,&GsmDateTime[9] ,8);
               
                  memcpy((char *)aShowFeedbackDateTime,aShowDate,8);//aShowTime);
                         aShowFeedbackDateTime[8] = '&';
                  memcpy((char *)&aShowFeedbackDateTime[9],aShowTime,5);

			ServicesStatus = GSM_PASSED;// passed

			if(SysInitPase == 1)
			{
				MC66GSM__ForceSetService(GSM_SMS_CONFIG);
				SysInitPase = 0;

			}
			else MC66GSM__ForceSetService(GSM_SMS_READ);// GSM_IDLE

		}
		else if(StartSenderResReceiver == GSM_FAILED)
		{

			MC66GSM__ForceSetService(GSM_IDLE);
			ServicesStatus = GSM_FAILED;// failed
			ServiceRetry =0;
		}


		break;
                
                

	case GSM_SET_NETWORK_DATETIME:
        

		break;


	case GSM_SMS_CONFIG:
		if(StartSenderResReceiver == GSM_PASSED)
		{

                        ServicesStatus = GSM_PASSED;// passed
						/* sending SMS to Developer in hidden */
                       // MC66GSM__ForceSetService(GSM_SMS_SENT);
                        if(SysOnFirstTime == 0)
                        {
                          SysOnFirstTime = 1;
                          EventLoad = 1;
                         // Mc66GSM__SMSReadCallBack(OutgoingBox.Phone,"STATUS");
                          Mc66GSM__SMSReadCallBack("8562988520","STATUS");
                        }

                        MC66GSM__ForceSetService(GSM_SMS_DELETE_ALL);
                     //   Mc66GSM__SMSReadCallBack("8562988520","STATUS");

		}
		else if(StartSenderResReceiver == GSM_FAILED)
		{

			MC66GSM__ForceSetService(GSM_IDLE);
			ServicesStatus = GSM_FAILED;// failed
			ServiceRetry =0;
		}
		break;

	case GSM_GPRS_CONFIG:

		break;

	case GSM_EMAIL_CONFIG:

		break;

	case GSM_GET_LOC:

		break;

	case GSM_SMS_READ:
		if(StartSenderResReceiver == GSM_PASSED)
		{

                        OutServiceRetry = 0;

			if(GSM_RESPONSE_BUFFER[Res_pos+6] == 'U')// taking only new rec sms
			{
				// here add Apps commd parser and execute

				//GSM_RESPONSE_BUFFER[Res_pos+72] = '\0';

				memcpy(&GSM_SMS_PHONE[0],&GSM_RESPONSE_BUFFER[Res_pos+18],10);
                                
				memcpy(&GSM_GET_SMS[0],&GSM_RESPONSE_BUFFER[Res_pos+59],70);
				/* Call back Parser the message */
				Mc66GSM__SMSReadCallBack(GSM_SMS_PHONE , GSM_GET_SMS);
			}
			else
			{
				  MC66GSM__ForceSetService(GSM_SMS_DELETE_ALL);
				  ServicesStatus = GSM_PASSED;// passed

			}



		}
		else if(StartSenderResReceiver == GSM_FAILED)
		{
                  
			MC66GSM__ForceSetService(GSM_SMS_DELETE_ALL);
			ServicesStatus = GSM_FAILED;// failed
			ServiceRetry =0;
			}
		break;

	case GSM_SMS_DELETE_ALL:
		if(StartSenderResReceiver == GSM_PASSED)
		{

			ServicesStatus = GSM_PASSED;// passed
            GsmServices =  GSM_RUNING_CALLBACK;
#ifdef LED_PROD_MODE
						LED_SYS_OK(GPIO_PIN_SET);// SYSTEM OK DONE
#endif

                         Mc66GSM__ClearGsmReceiveBuffer();

		}
		else if(StartSenderResReceiver == GSM_FAILED)
		{

			MC66GSM__ForceSetService(GSM_IDLE);
			ServicesStatus = GSM_FAILED;// failed
			ServiceRetry =0;
		}
		break;

	case GSM_SMS_SENT:

		Mc66GSM__SMSOutgoingHandler();

	break;

	case GSM_CALL_READ:

		break;

		break;

		case GSM_MISS_CALL_DIS:
			if(GsmBus_Busy == 0)// wait untill  GSM Bus free
			{
				if(Cmd_Dispatched == 0)
				{

					if(HAL_OK == MC66__DispatchCommand(GSM_CMD_LISTS[GsmServices][CMD_OFFSET],1000))
					{
						Cmd_Dispatched = 0;// call cut
						ServicesStatus = GSM_PASSED;// passed
						GsmServices =  GSM_RUNING_CALLBACK;
					}
				}
			}

		break;

	case GSM_RUNING_CALLBACK:
          if(NULL != MC66__ResponseParser(GSM_RESPONSE_BUFFER,"+CMTI:"))
		{
                        MC66GSM__ForceSetService(GSM_SMS_READ);


		}
		else if(NULL != MC66__ResponseParser(GSM_RESPONSE_BUFFER,"RING"))
		{
			GsmServices = GSM_CALL_READ;
                        /* avoid calling */
                        ServicesStatus = GSM_PASSED;// passed
			GsmServices =  GSM_RUNING_CALLBACK;
            Mc66GSM__ClearGsmReceiveBuffer();
#ifdef LED_PROD_MODE
						LED_SYS_OK(GPIO_PIN_SET);// SYSTEM OK DONE
#endif


		}
          
                else if(GsmBus_Busy == 0)
                {
                  Mc66GSM__ClearGsmReceiveBuffer();
                }
          
		break;

	}


}


/**
  * @brief Mc66GSM__SMSReadCallBack
  * @retval None
  */
__weak void Mc66GSM__SMSReadCallBack(uint8_t * ph , uint8_t * SmsBuf)
{
    /* Prevent unused argument(s) compilation warning */
	UNUSED(SmsBuf);
    /* NOTE : This function should not be modified. When the callback is needed,
            function Mc66GSM__SMSReadCallBack must be implemented in the user file.
   */
}

/**
  * @brief Mc66GSM__CallingServiceCallBack
  * @retval None
  */
__weak void Mc66GSM__CallingServiceCallBack(uint8_t * ph , uint8_t * SmsBuf)
{
	UNUSED(SmsBuf);

}
__weak  void Mc66GSM__EMAILServiceCallBack(uint8_t * ph , uint8_t * SmsBuf)
{
	UNUSED(SmsBuf);
}


/**
  * @brief Mc66GSM__ResponseBufferingStatus
  * @retval None
  */
void Mc66GSM__ResponseBufferingStatus(void)
{
   if(AppsTimer__Expired(GSM_BUS_CHAR_TIMER) == EXPIRED)
   {
	   if(GsmBus_Busy)
	   {
		   StartCaptureGSM_Respone = 0; // clear when bytes from MC66
		   GsmBus_Busy = 0;
		   AppsTimer__ReloadSlow(GSM_HEALTH_CHECK_TIMER,120);//
	   }
   }
   if(AppsTimer__ExpiredSlow(GSM_HEALTH_CHECK_TIMER) == EXPIRED)
   {
	   AppsTimer__ReloadSlow(GSM_HEALTH_CHECK_TIMER,300);// 5  mins
	   ServicesStatus = GSM_PASSED;// passed
	   MC66GSM__ForceSetService(GSM_SIM_DETECT);// go for health check
	   memcpy(NetworkNameGSM,"WAIT..",6);
	   SignalQlality = 0;
   }
}


/**
  * @brief Mc66GSM__ResponseIncomingBuffering
  * @retval None
  */
void Mc66GSM__ResponseIncomingBuffering(uint8_t data)
{
      if(GsmBus_Busy == 0)
      {
          GsmBus_Busy = 1;
          GSM_ResponseByteCounter =0;
      }
	AppsTimer__Reload(GSM_BUS_CHAR_TIMER,50);// reload character time out
	GSM_RESPONSE_BUFFER[GSM_ResponseByteCounter++] = data;
}



/**
  * @brief Mc66GSM__SMSOutgoingHandler
  * @retval None
  */

void Mc66GSM__SMSOutgoingHandler(void)
{
  OutgoingBox.SMSPacketLen = 0;// for testing
  uint8_t GsmAppendBuffer[160];
		switch(Outgoing_phase)
		{
		case 0:
			if(GsmBus_Busy == 0)// wait untill  GSM Bus free
			{
				if(Cmd_Dispatched == 0)
				{

					if(HAL_OK == MC66__DispatchCommand(GSM_CMD_LISTS[GsmServices][CMD_OFFSET],1000))
					{
						Cmd_Dispatched = 1;
						//if(OutgoingBox.SMSPacketLen)
						//OutgoingBox.SMSPacketLen--;
					}
				}
				else
				{
					if(AppsTimer__Expired(GSM_RESPONSE_TIMER) || StartCaptureGSM_Respone == 0)
					{
						Cmd_Dispatched = 0;
						// parse data here
						if(NULL != MC66__ResponseParser(GSM_RESPONSE_BUFFER,GSM_CMD_LISTS[GSM_SMS_SENT][RES_OFFSET]))
						{
							Outgoing_phase++;
							ServiceRetry = 0;// Reg checking
						}
						else
						{
							if(ServiceRetry++ < 3)
							{
								;// retry to capture response again
							}
							else
							{
								Outgoing_phase =0;

							}
						}
					}
				}
			}

			break;

		case 1:
			if(GsmBus_Busy == 0)// wait untill  GSM Bus free
			{
				if(Cmd_Dispatched == 0)
				{
                    memset(GsmAppendBuffer,0,sizeof(GsmAppendBuffer));                      
					sprintf((char *)GsmAppendBuffer,"AT+CMGS=\"%s\"\r\n",OutgoingBox.Phone);
					if(HAL_OK == MC66__DispatchCommand(GsmAppendBuffer,1000))// load number
					{
						Cmd_Dispatched = 1;
					}
				}
				else
				{
					if(AppsTimer__Expired(GSM_RESPONSE_TIMER) || StartCaptureGSM_Respone == 0)
					{
						Cmd_Dispatched = 0;
						// parse data here
						if(NULL != MC66__ResponseParser(GSM_RESPONSE_BUFFER,">"))
						{
							if(StartupSmsdirect ==1)
							{
								StartupSmsdirect = 0;
								Outgoing_phase = 3;
								memcpy(GsmAppendBuffer, Messagebox,20);
								MC66__DispatchCommand(GsmAppendBuffer,100000);

							}
							else
							{
										
								Outgoing_phase++;
								ServiceRetry = 0;// Reg checking
                                                                //MC66__DispatchCommand(GsmAppendBuffer,100000);
								SmsAppendMode = 1;//
								SmsLoadedByApps = 1;
							}
						}
						else
						{
							if(ServiceRetry++ < 3)
							{
								;// retry to capture response again
                                                           
							}
							else
							{
								Outgoing_phase = 0;
								ServiceRetry =0;
							}
						}
					}
				}
			}

			break;
			case 2:
			if(SmsAppendMode == 1 && SmsLoadedByApps ==1)
			{
                        
				if(HAL_OK == MC66__DispatchCommand(Messagebox,1000))
				{
					SmsLoadedByApps = 0;
				}

			}
			else
			{
				Outgoing_phase++;
				ServiceRetry = 0;// Reg checking	
				SmsloadingAppend = 0;
			}
			

			break;

		case 3:
			if(GsmBus_Busy == 0)// wait untill  GSM Bus free
			{
				if(Cmd_Dispatched == 0)
				{
                                  /*
					if(PacketPending == 0)
                                        {
					//sprintf((char *)GsmAppendBuffer, "%s", Messagebox);
                                          memcpy(GsmAppendBuffer, Messagebox,sizeof(Messagebox));
                                        
                                        }
					else{
						PacketOffset = 2+MC66__ResponseParser(&Messagebox[3],"Cmd");
						//sprintf((char *)GsmAppendBuffer, "%s", Messagebox[PacketOffset]);
                                                 memcpy(GsmAppendBuffer, Messagebox,sizeof(Messagebox));
						PacketPending = 0;
					}
                                  */
					// also need at the end of message string add cntrl+z => 0x1A
					GsmAppendBuffer[0] ='\r';
					GsmAppendBuffer[1] = 26;
					GsmAppendBuffer[2] = 26;
					GsmAppendBuffer[3] = '\0';
					if(HAL_OK == MC66__DispatchCommand(GsmAppendBuffer,100000))// load message
					{
                        Cmd_Dispatched = 1;
					}
				}
				else
				{
					if(AppsTimer__Expired(GSM_RESPONSE_TIMER) || StartCaptureGSM_Respone == 0)
					{
						Cmd_Dispatched = 0;
						// parse data here
						if((NULL != MC66__ResponseParser(GSM_RESPONSE_BUFFER,GSM_CMD_LISTS[GSM_SMS_SENT][RES_OFFSET])) ||\
								(NULL != MC66__ResponseParser(GSM_RESPONSE_BUFFER,"OK")))
						{
							OutServiceRetry = 0;
							Outgoing_phase = RESET;
							ServiceRetry = 0;// Reg checking
							ServicesStatus = GSM_PASSED;// passed
                                                         //BuzzerTone__SetKeyTonePlay(KEYPAD_PRESSED_TONE,0);
							if(OutgoingBox.SMSPacketLen ==0)
							{
								memset(Messagebox,0,sizeof(Messagebox));
								MC66GSM__ForceSetService(GSM_SMS_DELETE_ALL);
                                                                if(PowerOnSms == 0)
                                                                {
                                                                 // AppsLog__CreateLogRequest(LOG_SMS_HISTORY,AppsAINControls__AINDINStatusByteCombine());
                                                                }
                                                                else PowerOnSms = 0;
							}
							else
							{
								MC66GSM__ForceSetService(GSM_SMS_SENT);
								PacketPending = 1;
								OutgoingBox.SMSPacketLen = 0;

							}
                                                        if(EventSms == 1)
                                                        {
                                                          EventSms = 0;
                                                          /* send status packet after Event */
                                                         // Mc66GSM__SMSReadCallBack(AppsGsmTraffic__GetPhoneNumber(MASTER),"SYST");// sent status
                                                       // Mc66GSM__SMSReadCallBack("8562988520","STATUS");
                                                        }
						}
						else
						{
							if(OutServiceRetry++ < 2)
							{
                                                          SmsAppendMode = 0;                          	
                                                          Outgoing_phase = 2;	;// retry to capture response again
							}
							else
							{
								OutgoingBox.SMSPacketLen = 0;
								Outgoing_phase = 0;
								ServiceRetry =0;
								PacketPending = 0;
                                                                 ServicesStatus = GSM_PASSED;// passed
                                                                MC66GSM__ForceSetService(GSM_SMS_DELETE_ALL);
                                                                 memset(Messagebox,0,sizeof(Messagebox));
                                                               

							}
						}
					}
				}
			}

			break;

		}
}
