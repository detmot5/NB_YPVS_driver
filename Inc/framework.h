// HAL drivers
#include "main.h"
#include "tim.h"
#include "usart.h"
// Major project files
#include "RPMmeter.h"
#include "atcmd.h"

#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>



// Libraries
#include <Uartprintf.h>
#include <TickTimer.h>
#include <HardwareServo.h>
#include <ATCommandsService.h>
#include <UartDma.h>

#define DEBUG_MODE 1



long mapValue(long x, long in_min, long in_max, long out_min, long out_max);
#endif //  HAL_H
