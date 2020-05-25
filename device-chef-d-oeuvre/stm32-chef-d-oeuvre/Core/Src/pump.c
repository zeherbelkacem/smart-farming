/*
 * pump.c
 *
 *  Created on: 22 mai 2020
 *      Author: belkacem
 */

#include "pump.h"

void pump_action (void)
{
	HAL_GPIO_WritePin(GPIOA,  GPIO_PIN_7, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

	HAL_Delay(5000);
	HAL_GPIO_WritePin(GPIOA,  GPIO_PIN_7, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

	HAL_Delay(5000);

}
