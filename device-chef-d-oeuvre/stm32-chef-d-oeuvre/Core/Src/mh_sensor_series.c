/*
 * mh_sensor_series.c
 *
 *  Created on: Apr 12, 2020
 *      Author: belkacem
 */


/*****INCLUDES*****/
#include "mh_sensor_series.h"
#include "dbg.h"


extern ADC_HandleTypeDef hadc1;//mh sensor serie (moisture) (MHSS) signal must be connected to A1 pin in order to respect rank configuartion
							   //Because in the code the MHSS is the second called

/**FUNCTIONS IMPLEMENTATION****/
const char *moisture_state()
{
	//HAL_ADC_Start(&hadc1);
	//HAL_ADC_PollForConversion(&hadc1, 100);
	uint32_t sensorInputVoltage = HAL_ADC_GetValue(&hadc1);
	float soilMoistureRate = (float)(sensorInputVoltage *100) / 4095 ; //4095 is the ADC 12 bits resolution of the stm nucleo

	//dbg_log("adc SOIL moisture rate %.2f\n", soilMoistureRate);
	const char *moistureState;

	if(soilMoistureRate >= 90. || soilMoistureRate<3.)
		moistureState = "out";
	else if(soilMoistureRate < 90. && soilMoistureRate >= 60. )
		moistureState = "dry";
	else if(soilMoistureRate < 60. && soilMoistureRate >= 37. )
		moistureState = "humid";
	else
		moistureState = "water";
	//HAL_ADC_Stop(&hadc1);

	return moistureState;

}
/*
uint8_t get_moisture_input(ADC_HandleTypeDef hadc1)
{
	return(HAL_ADC_GetValue(hadc1));
}*/
