/*
 * dbg.c
 *
 *  Created on: May 3, 2020
 *      Author: belkacem
 */

/***********INCLUDES*********/
#include "dbg.h"
#include "stm32g0xx_hal.h"
//#include "stm32g0xx_ll_system.h"


/******EXTEIABLES**********/
extern UART_HandleTypeDef huart2; //uart for serial monitor

/*********Code allowing the use of printf with stm32 uart***/
#ifdef __GNUC__
int __io_putchar(int ch)
{
   uint8_t ch8=ch;
   HAL_UART_Transmit(&huart2,(uint8_t *)&ch8, 1, TIME_OUT);
   return ch;
}
int __io_getchar()
{
   uint8_t ch8;
   HAL_UART_Receive(&huart2,&ch8,1,TIME_IN);
   return (int)ch8;
}
#else
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, TIME_OUT);
  return ch;
}
int __io_getchar()
{
   uint8_t ch8;
   HAL_UART_Receive(&huart2,&ch8, 1, TIME_IN);
   return (int)ch8;
}
#endif /* __GNUC__ */
