#ifndef RPMMETER_H
#define RPMMETER_H

#include "framework.h"

#define MINUTE 60
#define MAX_RPM 12000
#define MAX_FREQ 200

void rpmMeterIrqHandler(TIM_HandleTypeDef* htim, uint32_t timChannel);
void rpmMeterTimPeriodElapsedIrqHandler(TIM_HandleTypeDef* htim);
void rpmMeterInit();
uint32_t getEngineRPM();
uint32_t getEngineFrequency();
void checkRPMstate();
uint8_t getRPMState();

#endif // !RPMMETER_H


