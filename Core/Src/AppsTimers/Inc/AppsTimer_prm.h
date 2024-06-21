/*
******************************************************************************
  * AppsTimer_prm.h
  *
  *  Created on: 27-May-2022
  *      Author: Niranjan
  ******************************************************************************/
/* Includes ------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


#ifndef APPSPECIFIC_DRIVERS_APPSTIMERS_INC_APPSTIMER_PRM_H_
#define APPSPECIFIC_DRIVERS_APPSTIMERS_INC_APPSTIMER_PRM_H_


void AppsTimer__UpdateHandler(void);
void AppsTimer__Reload(uint8_t timer_name,uint32_t count);
_Bool AppsTimer__Expired(uint8_t timer_name);
uint16_t AppsTimer__RemainingCounter(uint8_t timer_name);
void AppsTimer__ReloadSlow(uint8_t timer_name,uint32_t count);
_Bool AppsTimer__ExpiredSlow(uint8_t timer_name);
uint16_t AppsTimer__RemainingCounterSlow(uint8_t timer_name);
void AppsTimer__Initialize(void);

enum
{
  RUNING = 0,
  EXPIRED
};

typedef enum
{
	GP_TIMER1=0,
	GP_TIMER2,
	GP_TIMER3,
	GP_TIMER4,
	FAST_SLOT_TIMER,
	SLOW_SLOT_TIMER,
	COMMON_SLOT_TIMER,
	GSM_RESPONSE_TIMER,
	GSM_HANDLER_TIMER,
	SYSTEM_CONTROLTIMER,
	DISPLAY_TIMER,
        DISPLAY_TIMER_NEXT,
	GSM_BUS_CHAR_TIMER,
	GUI_BUS_CHAR_TIMER,
	KEYPAD_TIMER,
        KEYPAD_HANDLER,
	BUTTON_FAILED_TIMER,
	APS_GSM_PROCESS_TIMER,
	GSM_POWER_UP_STABILITY_TIMER,
	MISS_CALL_TIMEOUT,

/* Total timers do not remove this */
	TOTAL_TIMERS


}TimerTypes;

typedef enum
{

	REPEATE_SMS_TIMER_T1,
	REPEATE_SMS_TIMER_T2,
	REPEATE_SMS_TIMER_T3,
	REPEATE_SMS_TIMER_H,
	HOOTER_ON_TIMEOUT,
	RELAY_ON_TIMEOUT,
	LOG_INTERVAL_TIMER,
	GSM_HEALTH_CHECK_TIMER,
        WAIT_TO_RUN_EVENT_PROCESS,

/* Total timers do not remove this */
	TOTAL_SLOW_TIMERS


}TimerSlowTypes;


#endif /* APPSPECIFIC_DRIVERS_APPSTIMERS_INC_APPSTIMER_PRM_H_ */
