/*
 * pump.c
 *
 *  Created on: 22 mai 2020
 *      Author: belkacem
 */

#include "pump.h"
#include "stdio.h"
#include "dbg.h"


void pump_action (void)
{
	 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0 ); //put GPIO_PIN_3 to 5V because inverse logic
	 dbg_log("||--------------WATER PUMPING--------------||\n");
	 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	 HAL_Delay(10000);
	 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 1 );////put GPIO_PIN_3 to 0V because inverse logic
	 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
}
