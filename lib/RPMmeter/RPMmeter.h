#ifndef RPMMETER_H
#define RPMMETER_H


void rpmMeterIrqHandler(TIM_HandleTypeDef* htim, uint32_t timChannel);
uint16_t getEngineRPM();
uint32_t getEngineFrequency();


#endif // !RPMMETER_H


