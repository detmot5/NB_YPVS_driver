#ifndef RPMMETER_H
#define RPMMETER_H

#define MINUTE 60
#define MAX_RPM 12000
#define MAX_FREQ 200

void rpmMeterIrqHandler(TIM_HandleTypeDef* htim, uint32_t timChannel);
uint16_t getEngineRPM();
uint32_t getEngineFrequency();


#endif // !RPMMETER_H


