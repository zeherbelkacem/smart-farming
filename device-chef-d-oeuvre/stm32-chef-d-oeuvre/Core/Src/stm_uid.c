/*
 * uid.c
 *
 *  Created on: May 10, 2020
 *      Author: belkacem
 */


#include <stm_uid.h>
#include "stm32g0xx_ll_system.h"

/**
  * @brief  Get different information available in the MCU (Device)
  * @param  None
  * @retval None
  */
void get_mcu_uid(uint32_t* UIDw0, uint32_t *UIDw1, uint32_t *UIDw2)
{
	*UIDw0 = HAL_GetUIDw0();
	*UIDw1 = HAL_GetUIDw1();
	*UIDw2 = HAL_GetUIDw2();
}
