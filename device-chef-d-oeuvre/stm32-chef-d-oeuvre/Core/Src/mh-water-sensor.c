/*
 * mh-water-sensor.c
 *
 *  Created on: May 1, 2020
 *      Author: belkacem
 *      Physical warning: Don't hold the sensor form the that lines part it may DESTROY THE SENSOR
 */
#include "mh-water-sensor.h"

const char* mh_water_get_value()
{
	uint32_t adcWaterLevel = HAL_ADC_GetValue(&hadc1);
	const char* waterLevel;

	if(adcWaterLevel <= 100)
		waterLevel = "Empty";
	else if(adcWaterLevel >100 && adcWaterLevel <=1500)
		waterLevel = "Low";
	else if(adcWaterLevel >1500 && adcWaterLevel <= 3250)
		waterLevel = "Medium";
	else
		waterLevel = "Full";
	return waterLevel;
}


