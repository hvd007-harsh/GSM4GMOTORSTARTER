#include "AppControls.h"

#define TOTAL_LINES_LOGS 22 
#define TOTAL_LINES_SMS 7 
#define TOTAL_LINES_NOTIFICATION_SMS 3 
#define TOTAL_LINES_EVENTS 7

uint8_t Second;
uint8_t Minute; 
uint8_t Hour;

_Bool EventLoad = 0 , StsLoad = 0; 
uint8_t Indexcc = 0; 
uint8_t OWNER_NUM[10]; 

uint8_t UserPhoneGroup = 0;

uint8_t* APPS_CMD_LISTS[TOTAL_CMDS]; 
extern _Bool SmsAppendMode;

int8_t LimitsRow[2];
uint8_t Limitscal[2];
uint8_t DataBuff[50];
uint8_t Phone_Master_Num[10]; 
_Bool NOTIFICATION_FLAG = 0;

_Bool Notification_Load = 0;

uint8_t index = 0;
uint16_t indxxx = 0, j = 0, TotalData = 0;


/*..............
Event Message Template 
................*/
uint8_t * Event_Feedback_Buffer [TOTAL_LINES_SMS][3]={\

{"MPPT SYSTEM"       ,"ALERTS", "\r\n"},
{"SN", " 5/5",  "\r\n"},
{"PUMP", " ON ",  "\r\n"},
{"ADC R", "OKAY",  "\r\n"},
{"ADC Y", "OKAY",  "\r\n"},
{"ADC B","OKAY",  "\r\n"},
{"Date" ,"18/03/24 & 15:03",         "\r\n"}

};

uint8_t * Notification_Feedback_Buffer [TOTAL_LINES_NOTIFICATION_SMS][3]={\
{"MPPT SYSTEM"       ,"NOTIFICATION",         "\r\n"},
{"PUMP" , " OFF" ,  "\r\n"},
{"Date" ,"18/03/24 & 15:03",        "\r\n"}
};




/****
*@Brief:This API responsible to check message has contain expected command or not 
*@Params : True or False 
*
*/
static Cmd_Type AppsCmdExec_CMDparser(uint8_t* message)
{
Cmd_Type res = UNKNOWN_CMD, Index = UNKNOWN_CMD;
for(Index=CMD_REV; Index < UNKNOWN_CMD; Index++)
{
  if(NULL != MC66__ResponseParser(message,APPS_CMD_LISTS[Index]))
  {
    res = Index; 
    break;
  }
  
}
return res;
}
/****
*@Brief: PhoneValidator(Phone); @RetVal: Bool @Params: 
*/

static _Bool Phone_Validator(uint8_t * Num) 
{
    _Bool Checker_Flag=0; 
  uint8_t Num_Length = 0 ;
  
  Num_Length = strlen(Num);
  if(Num_Length  < 10  )
  {
    Checker_Flag = 0;
    return Checker_Flag ; 
  }
  
  if( Num[0] == ('0'||'1'))
  {
    Checker_Flag  = 0 ; 
    return Checker_Flag;
  }
   
 
  for(int8_t i = 1 ; i <= 10; i++)
  { 
    if( Num[i] > '0' || Num[i] < '9')
    {
      Checker_Flag = 1;
    }
    else
    {
      Checker_Flag =0;
      return Checker_Flag;
    }
  }
  return Checker_Flag;
}
/*********
*** @Breif : AppsEventOrganizer__Handler
**  @params: Void 
*  @RetVal : void 
@Desc : It will send message till all message didn't send to MS , OPE1 , OPE2, OPE3
*******/
void AppsEventOrganizer__Handler(void) 
{
  _Bool Validation_Flag = 0; 
  if(Sent_Completed == 1)
  {
    if(MC66GSM__GetServiceRunning() > GSM_SMS_CONFIG)
      if(MC66GSM__GetServiceRunning() == GSM_RUNING_CALLBACK) 
         switch (UserPhoneGroup)
         {
         case 0: 
           //For Master 
           Validation_Flag = Phone_Validator(Phone_Master_Num);
           if(Validation_Flag ==1)
           {
             if(NOTIFICATION_FLAG == 1) 
             {
               Notification_Load =1;
               Mc66GSM__SMSReadCallBack(Phone_Master_Num,"STATUS");
             }
             else
             {
               Mc66GSM__SMSReadCallBack(Phone_Master_Num,"STATUS");
             }
           }
          break; 
          
         case 1:
           //For OPE1 
           Validation_Flag = Phone_Validator(Phone_Master_Num);
           if(Validation_Flag ==1)
           {
             if(NOTIFICATION_FLAG == 1) 
             {
               Notification_Load =1;
               Mc66GSM__SMSReadCallBack(Phone_Master_Num,"STATUS");
             }
             else
             {
               Mc66GSM__SMSReadCallBack(Phone_Master_Num,"STATUS");
             }
           }
          break; 
         case 2: 
           //For OPE2
           Validation_Flag = Phone_Validator(Phone_Master_Num);
           if(Validation_Flag ==1)
           {
             if(NOTIFICATION_FLAG == 1) 
             {
               Notification_Load =1;
               Mc66GSM__SMSReadCallBack(Phone_Master_Num,"STATUS");
             }
             else
             {
               Mc66GSM__SMSReadCallBack(Phone_Master_Num,"STATUS");
             }
           }
          break; 
         case 3: 
                      //For OPE3
           Validation_Flag = Phone_Validator(Phone_Master_Num);
           if(Validation_Flag ==1)
           {
             if(NOTIFICATION_FLAG == 1) 
             {
               Notification_Load =1;
               Mc66GSM__SMSReadCallBack(Phone_Master_Num,"STATUS");
             }
             else
             {
               Mc66GSM__SMSReadCallBack(Phone_Master_Num,"STATUS");
             }
           }
          break; 
         }
  }
}




/****
***@Brief: AppsCmd_GSMSMSAppending 
**@Params: void 
*@Return: None 
***/
void  AppsCmd_GsmSMSAppending(void) 
{
  if(SmsloadingAppend ==1) 
  {
    if(EventLoad == 1)
    {
      if(Notification_Load == 1)
      {
        //Notification Command 
        if( indxxx < TOTAL_LINES_NOTIFICATION_SMS)
        {
          for(int i=0; i <3; i++)
          {
            memcpy(&DataBuff[TotalData],Notification_Feedback_Buffer[indxxx][j], strlen(Notification_Feedback_Buffer[indxxx][j]));
            TotalData += strlen(Notification_Feedback_Buffer[indxxx][j]);
            if(j = 0)
            {
              memcpy(&DataBuff[TotalData],":",1);
            }
          }
            MC66__DispatchCommand(DataBuff,1000);
            #ifdef DEBUG_MODE
            //AppsLog__DebugPrintf(DataBuff);
            #endif
            memset(DataBuff,0,TotalData+2);
            TotalData = 0;
            indxxx++;
        }
        else 
        {
          EventLoad = 0; 
          SmsloadingAppend = 0 ; 
          SmsAppendMode = 0;
          EmailAppendMode = 0 ; 
          indxxx = 0 ; 
          j= 0;
        }
      }
      else
      {
        if(indxxx < TOTAL_LINES_EVENTS) 
        {
          for(j = 0 ; j < 3; j++)
          {
            memcpy(&DataBuff[TotalData],Event_Feedback_Buffer[indxxx][j], strlen(Event_Feedback_Buffer[indxxx][j]));
            TotalData += strlen(Event_Feedback_Buffer[indxxx][j]);
            
            if(j == 0) 
            {
              memcpy(&DataBuff[TotalData],":",1);
              TotalData += 1; 
            }
          }
          while(HAL_OK != MC66__DispatchCommand(DataBuff,1000));
          memset(DataBuff, 0 , TotalData +2 );
          TotalData = 0;
          indxxx++; 
          
        }
        else
        {
          EventLoad = 0; 
          SmsloadingAppend = 0 ; 
          SmsAppendMode = 0; 
          EmailAppendMode = 0; 
          indxxx = 0; 
          j = 0;
        }
      }
    }
    else if(StsLoad == 1) 
    {
      if(indxxx < TOTAL_LINES_SMS)
      {
        for(j = 0; j<3; j++)
        {
          if(j == 0)
          {
            memcpy(&DataBuff[TotalData],":",1);
            TotalData += 1;
          }
        }
        while (HAL_OK != MC66__DispatchCommand(DataBuff, 1000));
        #ifdef DEBUG_MODE
        // AppsLog__DebugPrintf(DataBuff);
        #endif
        memset(DataBuff, 0, TotalData + 2);
        TotalData = 0;
        indxxx++;
        
      }
      else {
          SmsloadingAppend = 0; 
          SmsAppendMode = 0; 
          indxxx = 0; 
          j = 0; 
          StsLoad = 0; 
          EmailAppendMode = 0; 
      }
      
    }
    else {
      while (HAL_OK != MC66__DispatchCommand(Messagebox,1000));
      SmsloadingAppend = 0; 
      SmsAppendMode = 0;
      indxxx =0; 
      EmailAppendMode = 0;
      j =0; 
    }

  } 
   
}

/**
 * @brief : Adding the App Execution Command
 * @Params: Ph=> Phone Number in uint8_t *
 *           Data => This will be Data for the of the Message which was recieved from SMS
 * @Variables: In used of variables Indexcc;
 * @RetVal: CmdStatus which will tell it is approved or rejected
 ***/

uint8_t CmdOffset = 0;
CmdStatus AppCmdExec_RequestToExecutive(CmdRequesterTypes Requestertype, SMSPhTypes smsbox)
{
  GSM_SERVICES_REQUEST p_command;
  p_command.service_api = MC66;
  p_command.service_opcode = GSM_SMS_SENT;
  Indexcc = 0;

  uint8_t *RawData = smsbox.sms_box;
  /*
   * Here page is required but we are not saving the data
   */
  SysExecAuthTypes ExecAuthCmd = NO_AUTH_EXEC;
  /// here there is command for the GSM Health Check
  AppsTimer__ReloadSlow(GSM_HEALTH_CHECK_TIMER, 300); // 5 min
  ////
  uint8_t DataLen = 0;
  CmdOffset = 0;
  CmdStatus res = CMD_REJECTED;
  Cmd_Type CmdType = UNKNOWN_CMD;
  CmdType = AppsCmdExec__CMDparser(RawData);
  memset(DataBuff, 0, sizeof(DataBuff));
  memset(LimitsRow, 0, sizeof(LimitsRow));

  if (CmdType != UNKNOWN_CMD)
  {
    CmdOffset = MC66__ResponseParser(RawData, APPS_CMD_LISTS[CmdType]);
    // CmdOffset --> store cmd found position
  }
}
/*
*  Define By Author : Harsh Dixit 
*
*
*@Brief: SMSCaptcher 
*@Params: void  Ph -> Phone Sms-> Sms 
*@RetVal: void
*@Description: messagebox
*/


char p;
void SMS_Captcher(uint8_t *ph, uint8_t *sms)
{

  p = strstr(sms, "#");
  index = (strlen(sms) - strlen(p));
  // sms saving
  if (p)
  {
    p = 0x00;
    // Saving Command
    p = strstr(sms, "AD");
    if (p)
    {
      if (!(strcmp(ph, OWNER_NUM)))
      {
        memcpy(Messagebox, "\r\nADMIN ADDED ", 14);
        Messagebox[15] = DataIn[Phone_Master] = sms[index + 1];
        Messagebox[16] = DataIn[Phone_Master + 1] = sms[index + 2];
        Messagebox[17] = DataIn[Phone_Master + 2] = sms[index + 3];
        Messagebox[18] = DataIn[Phone_Master + 3] = sms[index + 4];
        Messagebox[19] = DataIn[Phone_Master + 4] = sms[index + 5];
        Messagebox[20] = DataIn[Phone_Master + 5] = sms[index + 6];
        Messagebox[21] = DataIn[Phone_Master + 6] = sms[index + 7];
        Messagebox[22] = DataIn[Phone_Master + 7] = sms[index + 8];
        Messagebox[23] = DataIn[Phone_Master + 8] = sms[index + 9];
        Messagebox[24] = DataIn[Phone_Master + 9] = sms[index + 10];
        memcpy(Messagebox + 25, "\r\n", 2);
        memcpy(Messagebox + 27, aShowFeedbackDateTime, 14);
        memcpy(Messagebox + 41, "\r\n", 2);
        memcpy(SMSbox, Messagebox, sizeof(Messagebox));
        Sms_Master = 1;
      }
      else
      {
        // UNAUTHORIZED
      }
    }

    p = 0x00;
    p = strstr(sms, "US1");
    if (p)
    {
      memcpy(Messagebox, "\r\nADDED USER 1", 14);
      Messagebox[15] = DataIn[Phone_OPE1] = sms[index + 1];
      Messagebox[16] = DataIn[Phone_OPE1 + 1] = sms[index + 2];
      Messagebox[17] = DataIn[Phone_OPE1 + 2] = sms[index + 3];
      Messagebox[18] = DataIn[Phone_OPE1 + 3] = sms[index + 4];
      Messagebox[19] = DataIn[Phone_OPE1 + 4] = sms[index + 5];
      Messagebox[20] = DataIn[Phone_OPE1 + 5] = sms[index + 6];
      Messagebox[21] = DataIn[Phone_OPE1 + 6] = sms[index + 7];
      Messagebox[22] = DataIn[Phone_OPE1 + 7] = sms[index + 8];
      Messagebox[23] = DataIn[Phone_OPE1 + 8] = sms[index + 9];
      Messagebox[24] = DataIn[Phone_OPE1 + 9] = sms[index + 10];
      memcpy(Messagebox + 25, "\r\n", 2);
      memcpy(Messagebox + 27, aShowFeedbackDateTime, 14);
      memcpy(Messagebox + 41, "\r\n", 2);
      memcpy(SMSbox, Messagebox, sizeof(Messagebox));
      Sms_User1 = 1;
    }
    p = 0x00;
    p = strstr(sms, "US2");
    if (p)
    {
      memcpy(Messagebox, "\r\nADDED USER 2", 14);
      Messagebox[15] = DataIn[Phone_OPE2] = sms[index + 1];
      Messagebox[16] = DataIn[Phone_OPE2 + 1] = sms[index + 2];
      Messagebox[17] = DataIn[Phone_OPE2 + 2] = sms[index + 3];
      Messagebox[18] = DataIn[Phone_OPE2 + 3] = sms[index + 4];
      Messagebox[19] = DataIn[Phone_OPE2 + 4] = sms[index + 5];
      Messagebox[20] = DataIn[Phone_OPE2 + 5] = sms[index + 6];
      Messagebox[21] = DataIn[Phone_OPE2 + 6] = sms[index + 7];
      Messagebox[22] = DataIn[Phone_OPE2 + 7] = sms[index + 8];
      Messagebox[23] = DataIn[Phone_OPE2 + 8] = sms[index + 9];
      Messagebox[24] = DataIn[Phone_OPE2 + 9] = sms[index + 10];
      memcpy(Messagebox + 25, "\r\n", 2);
      memcpy(Messagebox + 27, aShowFeedbackDateTime, 14);
      memcpy(Messagebox + 41, "\r\n", 2);
      memcpy(SMSbox, Messagebox, sizeof(Messagebox));
      Sms_User2 = 1;
    }
    p = 0x00;
    p = strstr(sms, "US3");
    if (p)
    {
      memcpy(Messagebox, "\r\nADDED USER 3", 14);
      Messagebox[15] = DataIn[Phone_OPE3] = sms[index + 1];
      Messagebox[16] = DataIn[Phone_OPE3 + 1] = sms[index + 2];
      Messagebox[17] = DataIn[Phone_OPE3 + 2] = sms[index + 3];
      Messagebox[18] = DataIn[Phone_OPE3 + 3] = sms[index + 4];
      Messagebox[19] = DataIn[Phone_OPE3 + 4] = sms[index + 5];
      Messagebox[20] = DataIn[Phone_OPE3 + 5] = sms[index + 6];
      Messagebox[21] = DataIn[Phone_OPE3 + 6] = sms[index + 7];
      Messagebox[22] = DataIn[Phone_OPE3 + 7] = sms[index + 8];
      Messagebox[23] = DataIn[Phone_OPE3 + 8] = sms[index + 9];
      Messagebox[24] = DataIn[Phone_OPE3 + 9] = sms[index + 10];
      memcpy(Messagebox + 25, "\r\n", 2);
      memcpy(Messagebox + 27, aShowFeedbackDateTime, 14);
      memcpy(Messagebox + 41, "\r\n", 2);
      memcpy(SMSbox, Messagebox, sizeof(Messagebox));
      Sms_User3 = 1;
    }
  }
  else
  {
    p = 0x00;
    // Asking Command
    p = strstr(sms, "?");
    index = (strlen(sms) - strlen(p));
    if (p)
    {
      // Saving Command
      p = 0x00;
      p = strstr(sms, "MS");
      if (p)
      {
        if (!(strcmp(ph, OWNER_NUM)))
        {
          memcpy(&Messagebox[0], "\r\n", 2);
          memcpy(&Messagebox[2], Phone_Master_Num, 10);
          memcpy(&Messagebox[12], "\r\n", 2);
        }
      }
      p = 0x00;
      p = strstr(sms, "US1");
      if (p)
      {

        memcpy(&Messagebox[0], "\r\n", 2);
        memcpy(&Messagebox[2], Phone_OP1_Num, 10);
        memcpy(&Messagebox[12], "\r\n", 2);
      }
      p = 0x00;
      p = strstr(sms, "US2");
      if (p)
      {

        memcpy(&Messagebox[0], "\r\n", 2);
        memcpy(&Messagebox[2], Phone_OP2_Num, 10);
        memcpy(&Messagebox[12], "\r\n", 2);
      }
      p = 0x00;
      p = strstr(sms, "US3");
      if (p)
      {

        memcpy(&Messagebox[0], "\r\n", 2);
        memcpy(&Messagebox[2], Phone_OP3_Num, 10);
        memcpy(&Messagebox[12], "\r\n", 2);
      }
    }

    p = 0x00;
    p = strstr(sms, "DELALL");
    if (p)
    {
      if (!(strcmp(ph, Phone_Master_Num)))
      {
        memcpy(&DataIn[Phone_OPE1], "1111111111", 9);
        memcpy(&DataIn[Phone_OPE2], "1111111111", 9);
        memcpy(&DataIn[Phone_OPE3], "1111111111", 9);
      }
      memcpy(Messagebox, "\r\n All Operator Deleted.\r\n", 30);
    }
    p = 0x00;
    p = strstr(sms, "DELUS1");
    if (p)
    {
      if (!(strcmp(ph, Phone_Master_Num)))
      {
        memcpy(&DataIn[Phone_OPE1], "1111111111", 9);

        memcpy(Messagebox, "\r\n Operator 1 Deleted.\r\n", 28);
      }
      p = 0x00;
      p = strstr(sms, "DELUS2");
      if (p)
      {
        if (!(strcmp(ph, Phone_Master_Num)))
        {
          memcpy(&DataIn[Phone_OPE2], "1111111111", 9);
        }
        memcpy(Messagebox, "\r\n Operator 2 Deleted.\r\n", 28);
      }
      p = 0x00;
      p = strstr(sms, "DELUS3");
      if (p)
      {
        if (!(strcmp(ph, Phone_Master_Num)))
        {
          memcpy(&DataIn[Phone_OPE3], "1111111111", 9);
        }
        memcpy(Messagebox, "\r\n Operator 3 Deleted.\r\n", 28);
      }
    }
    FlashEEPROM__WriteDataInToFlash(DataIn, (Phone_OPE3 + 9));
   // Parameter_Feeder();
  }
}

/////////////////////////////////////PARAMETER INIT 
/*
 * @Brief : Parameter will get Initialize through it
 * @RetVal : Return Will be Void  One Time use Only.
 */

void Parameter_Init()
{


  DataIn[0] = 1;
  DataIn[1] = 1;
  DataIn[2] = 1;
  DataIn[3] = 1;
  DataIn[4] = 1;
  DataIn[5] = 1;
  DataIn[6] = 1;
  DataIn[7] = 1;
  DataIn[8] = 1;
  DataIn[9] = 1;
  DataIn[10] =1;
  DataIn[11] =1;
  DataIn[12] =1;
 
  DataIn[13] = 0x01;
  // Caliberation Setting
  DataIn[Calib_Setting + 0] = 0;
  DataIn[Calib_Setting + 1] = 0;
  DataIn[Calib_Setting + 2] = 0;
  DataIn[Calib_Setting + 3] = 0;
  DataIn[Calib_Setting + 4] = 0;
  DataIn[Calib_Setting + 5] = 0;
  DataIn[Calib_Setting + 6] = 0;
  DataIn[Calib_Setting + 7] = 0;
  DataIn[Calib_Setting + 8] = 0;

  /////Master
  DataIn[Phone_Master] = 1;
  DataIn[Phone_Master + 1] = 1;
  DataIn[Phone_Master + 2] = 1;
  DataIn[Phone_Master + 3] = 1;
  DataIn[Phone_Master + 4] = 1;
  DataIn[Phone_Master + 5] = 1;
  DataIn[Phone_Master + 6] = 1;
  DataIn[Phone_Master + 7] = 1;
  DataIn[Phone_Master + 8] = 1;
  DataIn[Phone_Master + 9] = 1;

  /////////////Phone OPE1
  DataIn[Phone_OPE1] = 1;
  DataIn[Phone_OPE1 + 1] = 1;
  DataIn[Phone_OPE1 + 2] = 1;
  DataIn[Phone_OPE1 + 3] = 1;
  DataIn[Phone_OPE1 + 4] = 1;
  DataIn[Phone_OPE1 + 5] = 1;
  DataIn[Phone_OPE1 + 6] = 1;
  DataIn[Phone_OPE1 + 7] = 1;
  DataIn[Phone_OPE1 + 8] = 1;
  DataIn[Phone_OPE1 + 9] = 1;

  ////////////Phone OPE2
  DataIn[Phone_OPE2] = 1;
  DataIn[Phone_OPE2 + 1] = 1;
  DataIn[Phone_OPE2 + 2] = 1;
  DataIn[Phone_OPE1 + 3] = 1;
  DataIn[Phone_OPE1 + 4] = 1;
  DataIn[Phone_OPE1 + 5] = 1;
  DataIn[Phone_OPE1 + 6] = 1;
  DataIn[Phone_OPE1 + 7] = 1;
  DataIn[Phone_OPE1 + 8] = 1;
  DataIn[Phone_OPE2 + 9] = 1;

  ////////////////Phone OPE2
  DataIn[Phone_OPE3] = 1;
  DataIn[Phone_OPE3 + 1] = 1;
  DataIn[Phone_OPE3 + 2] = 1;
  DataIn[Phone_OPE3 + 3] = 1;
  DataIn[Phone_OPE3 + 4] = 1;
  DataIn[Phone_OPE3 + 5] = 1;
  DataIn[Phone_OPE3 + 6] = 1;
  DataIn[Phone_OPE3 + 7] = 1;
  DataIn[Phone_OPE3 + 8] = 1;
  DataIn[Phone_OPE3 + 9] = 1;

  FlashEEPROM__WriteDataInToFlash(DataIn, (Phone_OPE3 + 9));
}



