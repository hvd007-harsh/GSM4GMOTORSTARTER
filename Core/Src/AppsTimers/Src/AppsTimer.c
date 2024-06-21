/*
******************************************************************************
  * AppsTimer.c
  *
  *  Created on: 27-May-2022
  *      Author: Niranjan
  ******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_hal.h"
#include "variables.h"
#include "AppsTimer_prv.h"
/* Private variables ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


static uint32_t TIMER_EXEC[TOTAL_TIMERS];
static uint32_t SLOW_TIMER_EXEC[TOTAL_SLOW_TIMERS];



void AppsTimer__Initialize(void)
{
	uint8_t i =0;
	for (i=0; i < TOTAL_TIMERS; i++)
	{
		TIMER_EXEC[i] =0 ;
	}
	for (i=0;i < TOTAL_SLOW_TIMERS;i++)
	{
		SLOW_TIMER_EXEC[i] = 0;
	}
#ifdef DEBUG_MODE
	LCD_Line(0,2);
	LCD_Puts((uint8_t *)__FUNCTION__);
	HAL_Delay(InitDelaySeeTime);
	#endif
}

/* Need to call 10 millisecond
 * So its depend on timer requirements.
 * 10ms ----- i.e max 10 min delay can be supported
 * 100ms------i.e max 100 min delay can be supported
 * so on......
 */
static uint32_t SlowCounter = 0;
void AppsTimer__UpdateHandler(void)
{
	uint8_t i =0;
	for (i=0;i < TOTAL_TIMERS;i++)
	{
		if(TIMER_EXEC[i])
		{
			--TIMER_EXEC[i];
		}
	}
	if(SlowCounter++ >= 4410)//5000 TOTAL_SLOW_TIMERS 1Sec
	{
		SlowCounter = 0;
		for (i=0;i < TOTAL_SLOW_TIMERS;i++)
		{
			if(SLOW_TIMER_EXEC[i])
			{
				--SLOW_TIMER_EXEC[i];
			}
		}
	}
}

void AppsTimer__ReloadSlow(uint8_t timer_name,uint32_t count)
{
	SLOW_TIMER_EXEC[timer_name] = count;

}

void AppsTimer__Reload(uint8_t timer_name,uint32_t count)
{
	TIMER_EXEC[timer_name] = count;

}

_Bool AppsTimer__Expired(uint8_t timer_name)
{
	if(AppsTimer__RemainingCounter(timer_name) == 0 )
	{
		return EXPIRED;
	}
	else return RUNING;

}

_Bool AppsTimer__ExpiredSlow(uint8_t timer_name)
{
	if(AppsTimer__RemainingCounterSlow(timer_name) == 0 )
	{
		return EXPIRED;
	}
	else return RUNING;

}

uint16_t AppsTimer__RemainingCounterSlow(uint8_t timer_name)
{
	return SLOW_TIMER_EXEC[timer_name];
}

uint16_t AppsTimer__RemainingCounter(uint8_t timer_name)
{
	return TIMER_EXEC[timer_name];
}


