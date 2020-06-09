/*
 * stm_uid.h
 *
 *  Created on: May 10, 2020
 *      Author: belkacem
 */


#ifndef INC_STM_UID_H_
#define INC_STM_UID_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"
#include "stm32g0xx_ll_system.h"



void get_mcu_uid(uint32_t* UIDw0, uint32_t *UIDw1, uint32_t *UIDw2);


#endif /* INC_STM_UID_H_ */
