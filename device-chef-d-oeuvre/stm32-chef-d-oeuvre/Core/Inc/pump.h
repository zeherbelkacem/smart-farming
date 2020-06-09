/*
 * pump.h
 *
 *  Created on: 22 mai 2020
 *      Author: belkacem
 */

#ifndef INC_PUMP_H_
#define INC_PUMP_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_hal_gpio.h"
#include "stdio.h"
#include "string.h"

void pump_action_start ();
void pump_action_stop ();
#endif /* INC_PUMP_H_ */
