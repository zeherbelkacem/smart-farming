/*
 * mh-water-sensor.c
 *
 *  Created on: May 1, 2020
 *      Author: belkacem
 *      Physical warning: Don't hold the sensor form the that lines part it may DESTROY THE SENSOR
 */
#include "mh-water-sensor.h"
#include "dbg.h"

extern ADC_HandleTypeDef hadc1; //water sensor level (WSL) signal must be connected to A0 pin in order to respect rank configuartion
								//Because in the code the WSL is the first called

const char* mh_water_get_value()
{
	HAL_ADC_PollForConversion(&hadc1, 100);
	uint32_t adcWaterLevel = HAL_ADC_GetValue(&hadc1);
	//dbg_log("adc Water level %lu\n", adcWaterLevel);
	const char* waterLevel;

	if(adcWaterLevel <= 100)//from 0 to 4095
		waterLevel = "Empty";
	else if(adcWaterLevel >100 && adcWaterLevel <=1500)
		waterLevel = "Low";
	else if(adcWaterLevel >1500 && adcWaterLevel <= 3250)
		waterLevel = "Medium";
	else
		waterLevel = "Full";
	return waterLevel;
}


