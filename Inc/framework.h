// HAL drivers
#include "main.h"
#include "tim.h"
#include "usart.h"
// Major project files
#include "../Core/RPMmeter/RPMmeter.h"
#include "../Core/ATCommands/atcmd.h"
#ifndef FRAMEWORK_H
#define FRAMEWORK_H

// Libraries
#include "../lib/UartPrintf/uartprintf.h"
#include "../lib/TickTimer/tickTimer.h"
#include "../lib/HardwareServo/HardwareServo.h"
#include "../lib/ATCommandsService/ATCommandsService.h"
#include "../lib/UartDMA/uartDma.h"

#define DEBUG_MODE 1

long mapValue(long x, long in_min, long in_max, long out_min, long out_max);



#endif //  HAL_H
