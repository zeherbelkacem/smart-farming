/*
 * tcs34725.c
 *
 *  Created on: 16 févr. 2020
 *      Author: belkacem
 */
/*includes*/
#include "tcs34725.h"




//////////////////
/*
 *
 */
tcs34725Gain_t _tcs34725Gain;
tcs34725IntegrationTime_t _tcs34725IntegrationTime;
/***enable and disable the tcs34725 sensor
  */
void tcs34725_enable(void)
{
  tcs34725_i2c_write8(TCS34725_ENABLE, TCS34725_ENABLE_PON);
  tcs34725_i2c_write8(TCS34725_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);
}

void tcs34725_disable(void)
{
/* Turn the device off to save power */
	uint8_t reg = 0;
	reg = tcs34725_i2c_read8(TCS34725_ENABLE);
	tcs34725_i2c_write8(TCS34725_ENABLE, reg & ~(TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN));
}
//Read with 8 bits using I2C protocol
uint8_t tcs34725_i2c_read8(uint8_t reg)
{
	uint8_t buf[2];
	buf[0] = (TCS34725_COMMAND_BYTE | reg);
	HAL_I2C_Master_Transmit(&hi2c1, TCS34725_ADDR, buf, 1, HAL_MAX_DELAY);
  	HAL_I2C_Master_Receive(&hi2c1, TCS34725_ADDR, buf, 1, HAL_MAX_DELAY);
	return buf[0];
}
/*SENSOR init*/
void tcs32725_init()
{
/* Make sure we're actually connected */
	uint8_t x = tcs34725_i2c_read8(TCS34725_ID);
	//printf("sensor ID %d \n", x);
	if ((x != 0x44) && (x != 0x10))
		HAL_UART_Transmit(&huart2, (uint8_t *)"sensor not identified \n", 24, HAL_MAX_DELAY);
		//printf("sensor not identified \n");
	//_tcs34725Initialized = true;
/* Set default integration time and gain */
	tcs34725_set_time_integration(TCS34725_INTEGRATIONTIME_2_4MS);
	tcs34725_set_gain(TCS34725_GAIN_1X);
/* Note: by default, the device is in power down mode on bootup */
	tcs34725_enable();
	//return true;
}
//////////////////////////////////

// Read with 16 bits using i2c protocol
uint16_t tcs34725_i2c_read16(uint8_t reg)
{
	uint16_t x;	uint8_t TxBuf[2],RxBuf[2];
	TxBuf[0] = (TCS34725_COMMAND_BYTE | reg);
	HAL_I2C_Master_Transmit(&hi2c1, TCS34725_ADDR, TxBuf, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&hi2c1, TCS34725_ADDR, RxBuf, 2, HAL_MAX_DELAY);
	x = RxBuf[0];
	x <<= 8;
	x |= RxBuf[1];
	return x;
}
//Write with 8 bits for I2C protocol
void tcs34725_i2c_write8(uint8_t reg, uint32_t val)
{
	uint8_t buf[2];
	buf[0] = (TCS34725_COMMAND_BYTE | reg);
	buf[1] = (val & 0xFF);
	HAL_I2C_Master_Transmit(&hi2c1, TCS34725_ADDR, buf, 2, HAL_MAX_DELAY);
}
// SET TCS34725 TIME INTEGRATION
void tcs34725_set_time_integration(tcs34725IntegrationTime_t intTime)
{
	//if (!_tcs34725Initialized) tcs32725_init();
	 /* Update the timing register */
	tcs34725_i2c_write8(TCS34725_ATIME, intTime);
	 /* Update value placeholders */
	_tcs34725IntegrationTime = intTime;
}

void tcs34725_set_gain(tcs34725Gain_t gain)
{
	//if (!_tcs34725Initialized) tcs32725_init();
/* Update the timing register */
	tcs34725_i2c_write8(TCS34725_CONTROL, gain);
/* Update value placeholders */
	_tcs34725Gain = gain;
}
///
/*
 * GET (or read) RGBC data values (get with to bytes)
 */
void tcs34725_get_RGBC_Data(uint16_t *rValue, uint16_t *gValue, uint16_t *bValue, uint16_t *cValue)
{
 // if (!_tcs34725Initialized) tcs32725_init();
  *rValue = tcs34725_i2c_read16(REG_RDATAL);
  *gValue = tcs34725_i2c_read16(REG_GDATAL);
  *bValue = tcs34725_i2c_read16(REG_BDATAL);
  *cValue = tcs34725_i2c_read16(REG_CDATAL);
  /* Set a delay for the integration time */
  switch (_tcs34725IntegrationTime)
  {
    case TCS34725_INTEGRATIONTIME_2_4MS:
      HAL_Delay(3);
      break;
    case TCS34725_INTEGRATIONTIME_24MS:
      HAL_Delay(24);
      break;
    case TCS34725_INTEGRATIONTIME_50MS:
      HAL_Delay(50);
      break;
    case TCS34725_INTEGRATIONTIME_101MS:
      HAL_Delay(101);
      break;
    case TCS34725_INTEGRATIONTIME_154MS:
      HAL_Delay(154);
      break;
    case TCS34725_INTEGRATIONTIME_700MS:
      HAL_Delay(700);
      break;
  }
}
//GET R or G or B real VALUE
void tcs34725_get_RGB_Values(float *redColor, float *greenColor, float *blueColor)
{
	uint16_t redValue = 0, greenValue = 0, blueValue = 0, clearValue = 0;
	tcs34725_get_RGBC_Data(&redValue, &greenValue, &blueValue, &clearValue);

	// Division by zero
	if (clearValue == 0)
	{
		*redColor = 0;
		*greenColor = 0;
		*blueColor = 0;
	}
	else
	{
		*redColor = (float)redValue / clearValue * 255.0;
		*greenColor = (float)greenValue / clearValue * 255.0;
		*blueColor = (float)blueValue / clearValue * 255.0;
	}

}
/*
 * Use timer TIM1 to control an RGB LED
 */
void tcs34725_see_rgbLED(uint16_t redColor, uint16_t greenColor, uint16_t blueColor)
{
	HAL_TIM_PWM_Start (&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start (&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start (&htim1, TIM_CHANNEL_3);
	////////
	__HAL_TIM_SET_COMPARE( &htim1, TIM_CHANNEL_1, redColor );//Red LED "ON"
	__HAL_TIM_SET_COMPARE( &htim1, TIM_CHANNEL_2, greenColor );//Green LED "ON"
	__HAL_TIM_SET_COMPARE( &htim1, TIM_CHANNEL_3, blueColor );//Blue LED "ON"
	  HAL_Delay(3000);
	__HAL_TIM_SET_COMPARE( &htim1, TIM_CHANNEL_1, 0 );//Red LED "OFF"
	__HAL_TIM_SET_COMPARE( &htim1, TIM_CHANNEL_2, 0 );
	__HAL_TIM_SET_COMPARE( &htim1, TIM_CHANNEL_3, 0 );

}
