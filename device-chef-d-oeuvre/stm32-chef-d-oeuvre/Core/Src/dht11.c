/*
 * dht11.c
 *
 *  Created on: 29 avr. 2020
 *      Author: belkacem
 */
#include "dht11.h"



void dht11_delay_us (uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim1, 0);  // set the counter value a 0
	while (__HAL_TIM_GET_COUNTER(&htim1) < us);  // wait for the counter to reach the us input in the parameter
}

void dht11_set_pin_output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	//Configure GPIO pin to OUTPUT MODE : PA4

	 GPIO_InitStruct.Pin =  DHT11_GPIO;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void dht11_set_pin_input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	//Configure GPIO pin to INPUT MODE : PA4
	GPIO_InitStruct.Pin =  DHT11_GPIO;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void dht11_init()
{
	/* uint32_t t = HAL_GetTick(); //timer test
	dht11_delay_us(10000);
	//uint32_t t2 = HAL_GetTick() - t;
	printf("ticks %lu == 10 ?\r\n", HAL_GetTick() - t);*/
	dht11_set_pin_output(GPIOA,  DHT11_GPIO);
	HAL_GPIO_WritePin(GPIOA,  DHT11_GPIO, GPIO_PIN_RESET);
	dht11_delay_us(18000);
	HAL_GPIO_WritePin(GPIOA,  DHT11_GPIO, GPIO_PIN_SET);   // pull the pin high
	dht11_delay_us(20);   // wait for 20us
	dht11_set_pin_input(GPIOA,  DHT11_GPIO);
}

uint8_t dht11_check_Response ()
{
	uint8_t response = 0;
	dht11_delay_us(40);
	if (!(HAL_GPIO_ReadPin(GPIOA,  DHT11_GPIO)))
	{
		dht11_delay_us(80);
		response = 1;
		if ((HAL_GPIO_ReadPin(GPIOA,  DHT11_GPIO)))
			response = 2;
		else
			response = -1;
	}
	while ((HAL_GPIO_ReadPin(GPIOA,  DHT11_GPIO)));   // wait for the pin to go low

	return response;
}

uint8_t dht11_read_data ()
{
	uint8_t i,j;
	for (j=0;j<8;j++)
	{
		while (!(HAL_GPIO_ReadPin (GPIOA,  DHT11_GPIO)));   // wait for the pin to go high
		dht11_delay_us(40);   // wait for 40 us
		if (!(HAL_GPIO_ReadPin (GPIOA,  DHT11_GPIO)))   // if the pin is low
		{
			i&= ~(1<<(7-j));   // write 0
		}
		else i|= (1<<(7-j));  // if the pin is high, write 1
		while ((HAL_GPIO_ReadPin (GPIOA,  DHT11_GPIO)));  // wait for the pin to go low
	}
	return i;
}


void dht11_get_AirHumidity_Temperature(float *airHumidity, float *temperature) //data = AirHumidity or Temperature
{
	uint8_t dht11Bytes[5];//dht11Bytes[0]: first humidity byte
						  //dht11Bytes[1]: second humidity byte
						  //dht11Bytes[2]: first temperature byte
						  //dht11Bytes[3]: second temperature byte
						  //dht11Bytes[4]: bytes sum
	uint16_t airHumid = 0, temperat = 0;
	/*
	 * Before the 2 following functions, don't forget to call HAL_TIM_Base_Start(&htim1)
	 * in the main or in this function itself
	 */
	HAL_TIM_Base_Start(&htim1);

	dht11_init();
	uint8_t response = dht11_check_Response();
	if(response == 2)
	{
		for(int i = 0; i<5; i++)
			dht11Bytes[i] = dht11_read_data();
		////////////////////////////
		airHumid = dht11Bytes[0]; //first humidity byte
		temperat = dht11Bytes[2]; //first temperature byte
		///////////////////////////
		*airHumidity = (float)airHumid;
		*temperature = (float)temperat;
	}
	else
		printf("DHT11 did not respond, try to get response to 2 \n");


}
