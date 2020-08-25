/*
 * millis.c
 *
 *  Created on: Aug 7, 2020
 *      Author: norbe
 */


#include "../../Inc/main.h" 
#include "uartprintf.h"

#include  <errno.h>
#include  <sys/unistd.h> // STDOUT_FILENO, STDERR_FILENO
#include  <string.h>



static UART_HandleTypeDef* printf_uart = NULL;

void uart_init_printf(UART_HandleTypeDef* huart){
	printf_uart = huart;
}



int _write(int file, char *data, int len)
{
   if ((file != STDOUT_FILENO) && (file != STDERR_FILENO))
   {
      errno = EBADF;
      return -1;
   }

   if(NULL == printf_uart) return HAL_ERROR;

   // arbitrary timeout 1000
   HAL_StatusTypeDef status =
      HAL_UART_Transmit(printf_uart, (uint8_t*)data, len, 1000);
   	 	HAL_UART_Transmit(printf_uart, (uint8_t*)"\r", 1, 1000);

   // return # of bytes written - as best we can tell
   return (status == HAL_OK ? len : 0);
}












