/*
 * mh-water-sensor.h
 *
 *  Created on: May 1, 2020
 *      Author: belkacem
 */

#ifndef INC_MH_WATER_SENSOR_H_
#define INC_MH_WATER_SENSOR_H_

#include "stm32g0xx_hal.h"


extern ADC_HandleTypeDef hadc1;

const char* mh_water_get_value();


#endif /* INC_MH_WATER_SENSOR_H_ */
