#ifndef APP_H
#define APP_H
#include <RPMmeter.h>

#ifdef __cplusplus
extern "C"{
#endif


void initPeripherals(void);

void mainLoop(void);

void handle_TIM_IC_interrupts(TIM_HandleTypeDef *htim);

void handle_TIM_PeriodElapsed_interrupts(TIM_HandleTypeDef *htim);

#ifdef __cplusplus
}
#endif
#endif // !APP_H



