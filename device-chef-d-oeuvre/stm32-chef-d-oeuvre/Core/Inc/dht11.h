/*
 * dht11.h
 *
 *  Created on: 29 avr. 2020
 *      Author: belkacem
 */

#ifndef INC_DHT11_H_
#define INC_DHT11_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_hal_gpio.h"
#include "stdio.h"
#include "string.h"

#define DHT11_GPIO GPIO_PIN_4

extern TIM_HandleTypeDef htim1;
/*****Get delay (for dht11) in microseconds***/
void dht11_delay_us (uint16_t us);

/**********DHT11 INITIALIZATION*********/
void dht11_init();
/*********CHECK DHT11 RESPONSE*********/
uint8_t dht11_check_Response ();
/***********DHT11 READ DATA************/
uint8_t dht11_read_data ();
/*****SET DHT11 PIN CONNECTION TO INPUT AND TO OUTPUT*****/
void dht11_set_pin_output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void dht11_set_pin_input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void dht11_get_AirHumidity_Temperature(float *airHumidity, float *temperature); //data = AirHumidity or Temperature

#endif /* INC_DHT11_H_ */
