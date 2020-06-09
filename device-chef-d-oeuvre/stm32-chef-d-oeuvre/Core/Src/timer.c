/*
 * timer.c
 *
 *  Created on: May 25, 2020
 *      Author: belkacem
 */


/**
 ******************************************************************************
 * @file           : timer.c
 * @brief          : timer.c file.
 *                   This file contains the common defines of the timer
 *                   interrupts
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "timer.h"
#include "main.h"

/* Private includes ----------------------------------------------------------*/
#include "dbg.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/**
 *-----------------------------------------------------------------------------
 * see system_stm32g0xx.c
 *-----------------------------------------------------------------------------
 *	System Clock source  | HSI
 *	SYSCLK(Hz)           | 16000000
 *	HCLK(Hz)             | 16000000
 *-----------------------------------------------------------------------------
 *	LSI_VALUE			32 Khz
 *	LSE_VALUE			32.768 Khz
 *	HSE_VALUE  	 		8 MHz
 *	HSI_VALUE  			16 MHz
 *	SystemCoreClock		16 MHz
 *-----------------------------------------------------------------------------
 *  internal clock		2 MHz
 *-----------------------------------------------------------------------------
 *	CLK		| 2 MHz		| 8 MHz		| 16 MHz	| 32 MHz
 *	PSC		| 4			| 16		| 32		| 64
 *	ARR		| 50000		| 50000		| 50000		| 50000
 *	RCR		| 1			| 1			| 1			| 1
 *	event	| 100 ms	| 100 ms	| 100 ms	| 100 ms
 *-----------------------------------------------------------------------------
 *	if you change the prescaler PSC in ioc => change CLK value !!!
 *	Take care to NOT write 2*10^6 but 2e6 !!!
 *-----------------------------------------------------------------------------
 */
//#define CLK	(HSI_VALUE)
#define CLK	(64e6)
#define PSC	(CYCLE_HANDLER.Init.Prescaler+1)
#define ARR (CYCLE_HANDLER.Init.Period+1)
#define RCR (CYCLE_HANDLER.Init.RepetitionCounter+1)

// convert HZ in MS
#define HZ_MS			1000

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern REQUEST_TYPE FLAG;
extern TIM_HandleTypeDef 	CYCLE_HANDLER;  	// address of timer structure
static float 				event_ms;		// event frequency on timer
static uint16_t 			cycle_trigger;	// counter limit
static uint16_t 			single_trigger;	// counter limit
static uint16_t 			cycle_threshold;	// counter
static uint32_t				cycle_tick;
static uint32_t				single_tick;
static uint16_t				single_threshold;



/**
  * @brief  start timer
  * @param	timer handler
  * @retval nothing
  */
void timer_start(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == CYCLE_TIM) {
		// counter
		cycle_threshold = 0;
		// formula to have the event frequency on timer
		// objective on timer is to have an interrupt every 100 ms
		event_ms 	= HZ_MS;
		event_ms 	*= PSC*ARR*RCR;
		event_ms 	/= CLK;
		// counter limit for LED blink
		cycle_trigger =  (CYCLE_VALUE * HZ_MS) / event_ms;

		if (cycle_trigger == 0) {
			//dbg_log("invalid timer %hu", cycle_trigger);
			Error_Handler();
		} else {  // led_trigger should be 5
			//dbg_log("start timer %hu", cycle_trigger);
			cycle_tick = HAL_GetTick();
			HAL_TIM_Base_Start_IT(htim);
		}

	}else if (htim->Instance == SINGLE_TIM) {
		event_ms 	= HZ_MS;
		event_ms 	*= PSC*ARR*RCR;
		event_ms 	/= CLK;
		single_trigger =  (SINGLE_VALUE * HZ_MS) / event_ms ;
		if (single_trigger == 0) {
			Error_Handler();

		}else {  // led_trigger should be 5
			HAL_TIM_Base_Start_IT(htim);
		}
	}else {

	//	dbg_log("error start timer");
		Error_Handler();
	}
}

/**
  * @brief  stop timer
  * @param	timer handler
  * @retval nothing
  */
void timer_stop(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == CYCLE_TIM) {
		//dbg_log("stop led timer");
		HAL_TIM_Base_Stop_IT(htim);
	}
	else if (htim->Instance == SINGLE_TIM) {
		HAL_TIM_Base_Stop_IT(htim);
	}
	else {
		//dbg_log("error stop timer");
		Error_Handler();
	}
}

/**
  * @brief  timer interrupt callback
  * @param	timer handler
  * @retval nothing
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == CYCLE_TIM) {
		++cycle_threshold;
		//dbg_printf("timer IT %u",cycle_threshold);

		if (cycle_threshold == cycle_trigger) { // +2 = +200 ms
			cycle_tick = HAL_GetTick();
			FLAG = PERIODIC_DATA;
			cycle_threshold = 0;
		}
	}
	else if (htim->Instance == SINGLE_TIM) {
		++single_threshold;
		if (single_threshold == single_trigger) { // +2 = +200 ms
			single_tick = HAL_GetTick();
			FLAG = STOP_PUMP;
			single_threshold = 0;
		}
		}
}
