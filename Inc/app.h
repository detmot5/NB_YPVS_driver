#ifndef APP_H
#define APP_H
#include <TickTimer.h>
#include <UartPrintf.h>
#include <RPMmeter.h>
#include <LCD44780_I2C.h>



#define DEBUG_MODE 0





extern tickTimer ledBuiltinTim;
#if DEBUG_MODE
extern tickTimer simulateRPMTim;
#endif


void initPeripherals(void);
void mainLoop(void);
void handle_TIM_IC_interrupts(TIM_HandleTypeDef* htim);
void handle_TIM_PeriodElapsed_interrupts(TIM_HandleTypeDef* htim);
#endif // !APP_H


