/**
  ******************************************************************************
  * @file           : timer.h
  * @brief          : Header for timer.c file.
  *                   This file contains the common defines of the timer
  *                   interrupts on STM32 board.
  *
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMER_H
#define __TIMER_H

/* Private includes ----------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants & variables --------------------------------------------*/

#define CYCLE_TIM			 TIM2
#define SINGLE_TIM 			 TIM6
#define CYCLE_HANDLER		 htim2
#define SINGLE_HANDLER		 htim6
#define CYCLE_VALUE 6 //seconds for PERIODIC_CYCLE
#define SINGLE_VALUE 10 //seconds pump action duration

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Limits : 1 to 10 Hz => 1000 ms to 100 ms */

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

/**
  * @brief  start a timer handler
  * @param  timer handler
 */
void timer_start(TIM_HandleTypeDef *htim);

/**
  * @brief  stop a timer handler
  * @param  timer handler
 */
void timer_stop(TIM_HandleTypeDef *htim);

/* Private defines -----------------------------------------------------------*/


#endif
