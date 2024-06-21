/*
******************************************************************************
  * AppsGsmTraffic.c
  *
  *  Created on: 27-May-2022
  *      Author: Niranjan
  ******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_hal.h"
#include "variables.h"
#include "AppsGsmTraffic_prv.h"
//#include "App.h"

/* Private variables ---------------------------------------------------------*/
uint8_t *Cell_Nums[TOTAL_CELL]=NULL;
uint8_t *Email_Nums[TOTAL_CELL]=NULL;
uint8_t Cell_Nums_Permission[TOTAL_CELL]={0};

_Bool Sms_Master = 0; 
_Bool Sms_User1 = 0; 
_Bool Sms_User2 = 0 ;
_Bool Sms_User3 = 0 ;
/* Private variables ---------------------------------------------------------*/

_Bool SystemStopedByDeveloper = 0;
/* Private function prototypes -----------------------------------------------*/

void AppsGsmTraffic__Initialization(void)
{
/*
                -----------*-----------
        Every phone permission:
        -----------*----------
        Event permission
        Interval permission
        Miss Calls permission
        Night Mode Permission
        */
	Cell_Nums[REVSED]="9654204232";
	Cell_Nums[DEVELOPER]= "8562988520";//AppsDBManager__ReadDataBase(PG_DEVLOP , PH_NUMB_OFST);
	Cell_Nums[MASTER]="8562988520";//AppsDBManager__ReadDataBase(PG_MASTER , PH_NUMB_OFST);
	Cell_Nums[OWNER]="9654204232";//AppsDBManager__ReadDataBase(PG_OWNER , PH_NUMB_OFST);
	Cell_Nums[OPE1]="9654204232";//AppsDBManager__ReadDataBase(PG_OPERATOR1 , PH_NUMB_OFST);
	Cell_Nums[OPE2]="6387000075";//AppsDBManager__ReadDataBase(PG_OPERATOR2 , PH_NUMB_OFST);
	Cell_Nums[OPE3]="8562988520";//AppsDBManager__ReadDataBase(PG_OPERATOR3 , PH_NUMB_OFST);//

	Cell_Nums_Permission[REVSED] = 0xFF;
        Cell_Nums_Permission[DEVELOPER]=0xFF;//AppsDBManager__ReadDataBase(PG_DEVLOP , PH_STATUS1_OFST);
	Cell_Nums_Permission[MASTER]=(uint8_t)"8562988520";//*AppsDBManager__ReadDataBase(PG_MASTER , PH_STATUS1_OFST);
	Cell_Nums_Permission[OWNER]=(uint8_t)"9654204232";//*AppsDBManager__ReadDataBase(PG_OWNER , PH_STATUS1_OFST);
	Cell_Nums_Permission[OPE1]=(uint8_t)"6387000075";//*AppsDBManager__ReadDataBase(PG_OPERATOR1 , PH_STATUS1_OFST);
	Cell_Nums_Permission[OPE2]=(uint8_t)"6387000075";//*AppsDBManager__ReadDataBase(PG_OPERATOR2 , PH_STATUS1_OFST);
	Cell_Nums_Permission[OPE3]=(uint8_t)"6387000075";//*AppsDBManager__ReadDataBase(PG_OPERATOR3 , PH_STATUS1_OFST);//



#ifdef DEBUG_MODE
	//AppsLog__DebugPrintf((uint8_t *)__FUNCTION__);
	LCD_Line(0,2);
	LCD_Puts((uint8_t *)__FUNCTION__);
	HAL_Delay(InitDelaySeeTime);
	#endif
}

SMSPhTypes  IncommingBox ;
void Mc66GSM__SMSReadCallBack(uint8_t * ph , uint8_t * SmsBuf)
{
        GSM_SERVICES_REQUEST p_command;
        CmdStatus sts = CMD_IDEL;
        IncommingBox.sms_box = SmsBuf;
        IncommingBox.PhoneNum = ph;
        p_command.service_api = MC66;
        p_command.service_opcode = GSM_SMS_SENT;

	
        memcpy(OutgoingBox.Phone,ph,sizeof(OutgoingBox.Phone));// will update exact index later
        
        memcpy(Messagebox,SmsBuf,strlen(SmsBuf));// will update exact index later
        
        // BuzzerTone__SetKeyTonePlay(KEYPAD_PRESSED_TONE,0);
       
       // SMS_Captcher(ph,SmsBuf);
       
       
	/* request to Cmd handler guys */
	if(1)//SystemStopedByDeveloper == 1 && (0 != strncmp("9654204232",ph,10)))
        {
         // EventLoad = 1;//
          //prepare reply vis SMS of command ststus
		/* Ex: SMS sent create request */

			p_command.timeout = 2000;// ms
			//memcpy(Messagebox,"\r\nYour system is out of order. please contact your admin to continue.\r\n",75);
#ifdef SMS_MAP_ON_USB
			//AppsLog__DebugPrintf("SMS Sent:\r");
			//AppsLog__DebugPrintf(OutgoingBox.Phone);// for testing.
			//AppsLog__DebugPrintf(Messagebox);
#else
			Mc66GSM__CreateRequest(p_command);
#endif


#ifdef DEBUG_MODE
      //  AppsLog__DebugPrintf(OutgoingBox.Phone);
		//AppsLog__DebugPrintf(Messagebox);
#endif
		return;

	}

	//sts = AppsCmdExec__RequestToExecutive(SYS_GSM,IncommingBox);
         /**
        Here I m changing in the App.c for this command to execute It's Command Request 
        
        *********/ 
           
	if(sts == CMD_ACCEPTED )
	{
#ifdef DEBUG_MODE
		//AppsLog__DebugPrintf("\rGSM Command Successfully executed");

#endif

	}
	else if(sts == CMD_REJECTED )
	{
		//prepare reply vis SMS of command ststus
/* Ex: SMS sent create request */

			p_command.timeout = 2000;// ms
			memcpy(Messagebox,"ERROR: \nUnauthorized Or\nWrong Command. \nRejected\n",sizeof(Messagebox));
#ifdef SMS_MAP_ON_USB
			//AppsLog__DebugPrintf("SMS Sent:\r");
			//AppsLog__DebugPrintf(OutgoingBox.Phone);// for testing.
			//AppsLog__DebugPrintf(Messagebox);
#else
			Mc66GSM__CreateRequest(p_command);// no reject sms sent
#endif


#ifdef DEBUG_MODE
               // AppsLog__DebugPrintf(OutgoingBox.Phone);
		//AppsLog__DebugPrintf(Messagebox);
#endif

	}


}

uint8_t * AppsGsmTraffic__GetPhoneNumber(PhoneTypes cell)
{
	return Cell_Nums[cell] ;// cell type pointer will return and user can read from this pointer

}


PhoneTypes AppsGsmTraffic__GetPhoneInfo(uint8_t *ph)
{
	PhoneTypes index =  REVSED;

	for(index = REVSED; index < TOTAL_CELL; index++)
	{
		if((0 == strncmp((char *)ph,(char *)Cell_Nums[index],10)))
		{
				break;

		}
	}

	return index; // if not found then index reach at UNKNOWN_NUMBER and return automatically.


}

