// HAL drivers
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "adc.h"
// Major project files
#include "RPMmeter/RPMmeter.h"

// Libraries
#include "UartPrintf/uartprintf.h"
#include "TickTimer/tickTimer.h"
#include "../lib/HardwareServo/HardwareServo.h"
#include "LCD44780_I2C/LCD44780_I2C.h"



#ifndef FRAMEWORK_H
#define FRAMEWORK_H


long mapValue(long x, long in_min, long in_max, long out_min, long out_max);



#endif //  HAL_H
