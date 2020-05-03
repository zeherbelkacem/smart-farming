/*
 * tcs34725.h
 *
 *  Created on: 16 févr. 2020
 *      Author: belkacem
 *
 *      Pins Connection: VIN (tcs) to 5V (Nucleo)
 *      				 GND(tcs) to GND (Nucleo)
 *      				 SDA (tcs) to I2C/SDA/PB7 (Nucleo)
 *      				 SCL (tcs) to I2C/SCL/PB6 (Nucleo)
 */

#ifndef INC_TCS34725_H_
#define INC_TCS34725_H_
/*
 *
 */


#include "stdint.h"
#include "stm32g0xx_hal.h"

extern TIM_HandleTypeDef htim1;
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;

static const uint8_t TCS34725_ENABLE = 0x00; //W/R enable states and interrupts
static const uint8_t TCS34725_ENABLE_AEN = 0x02; /* RGBC Enable - Writing 1 actives the ADC, 0 disables it */
static const uint8_t TCS34725_ENABLE_PON = 0x01; /* Power on - Writing 1 activates the internal oscillator, 0 disables it */
static const uint8_t TCS34725_ADDR = 0x29 << 1; // Use 8-bit address
static const uint8_t TCS34725_ID = 0x12 ;//TCS34725 (device) ID
static const uint8_t TCS34725_ATIME = 0x01; // TIME integration
static const uint8_t TCS34725_CONTROL = 0x0F; //Control register (gain)
static const uint8_t TCS34725_COMMAND_BYTE = 0x80; // Command byte in i2c protocol (to specify???)
static const uint8_t REG_CDATAL = 0x14; //CLEAR data low byte
static const uint8_t REG_CDATAH = 0x15; //CLEAR data high byte
static const uint8_t REG_RDATAL = 0x16; //REDTCS34725_ATIME data low byte
static const uint8_t REG_RDATAH = 0x17; //RED data high byte
static const uint8_t REG_GDATAL = 0x18; //GREEN data low byte
static const uint8_t REG_GDATAH = 0x19; //GREEN data HIGH byte
static const uint8_t REG_BDATAL = 0x1A; //BLUE data low byte
static const uint8_t REG_BDATAH = 0x1B; //BLUE data HIGH byte
/*
 *
 */
typedef enum
{
  TCS34725_INTEGRATIONTIME_2_4MS  = 0xFF,   /**<  2.4ms - 1 cycle    - Max Count: 1024  */
  TCS34725_INTEGRATIONTIME_24MS   = 0xF6,   /**<  24ms  - 10 cycles  - Max Count: 10240 */
  TCS34725_INTEGRATIONTIME_50MS   = 0xEB,   /**<  50ms  - 20 cycles  - Max Count: 20480 */
  TCS34725_INTEGRATIONTIME_101MS  = 0xD5,   /**<  101ms - 42 cycles  - Max Count: 43008 */
  TCS34725_INTEGRATIONTIME_154MS  = 0xC0,   /**<  154ms - 64 cycles  - Max Count: 65535 */
  TCS34725_INTEGRATIONTIME_700MS  = 0x00    /**<  700ms - 256 cycles - Max Count: 65535 */
}tcs34725IntegrationTime_t;

typedef enum
{
  TCS34725_GAIN_1X                = 0x00,   /**<  No gain  */
  TCS34725_GAIN_4X                = 0x01,   /**<  4x gain  */
  TCS34725_GAIN_16X               = 0x02,   /**<  16x gain */
  TCS34725_GAIN_60X               = 0x03    /**<  60x gain */
}tcs34725Gain_t;

/*
 *
 */
void tcs34725_enable(void);
void tcs34725_disable(void);
uint8_t tcs34725_i2c_read8(uint8_t reg);// read 1 data byte
void tcs32725_init();// TCS34725 initialization
void tcs34725_i2c_write8(uint8_t reg, uint32_t val);//
uint16_t tcs34725_i2c_read16(uint8_t reg); //read 2 data byte
void tcs34725_set_time_integration(tcs34725IntegrationTime_t intTime);
void tcs34725_set_gain(tcs34725Gain_t gain);
void tcs34725_get_RGBC_Data(uint16_t *rValue, uint16_t *gValue, uint16_t *bValue, uint16_t *cValue);//for example: COLOR_DATA_ADDR = REG_CDATAL = 0x14
void tcs34725_get_RGB_Values(float *redColor, float *greenColor, float *blueColor);
void tcs34725_see_rgbLED(uint16_t redColor, uint16_t greenColor, uint16_t blueColor);

#endif /* INC_TCS34725_H_ */
