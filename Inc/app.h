#ifndef APP_H
#define APP_H
#include <TickTimer.h>
#include <UartPrintf.h>
#include <RPMmeter.h>

extern tickTimer ledBuiltinTim;
#if DEBUG_MODE 
extern tickTimer simulateRPMTim;
#endif 



void initPeripherals(void);
void handle_TIM_IC_interrupts(TIM_HandleTypeDef* htim);

#endif // !APP_H


