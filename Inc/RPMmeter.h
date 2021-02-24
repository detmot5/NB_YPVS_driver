#ifndef INC_RPMMETER_H_
#define INC_RPMMETER_H_

#include "framework.h"

#define MINUTE 60
#define MAX_RPM 12000
#define MAX_FREQ 200


typedef enum e_EngineRPMState {
  RPM_STATE_INCREASING,
  RPM_STATE_DECREASING,
  RPM_STATE_CONSTANT,
} EngineRPMState_t;

void rpmMeterIrqHandler(TIM_HandleTypeDef* htim, uint32_t timChannel);
void rpmMeterInit();
uint32_t getEngineRPM();
uint32_t getEngineFrequency();
void checkIsEngineRunning();
void checkRPMstate();
uint8_t getRPMState();

#endif  // INC_!RPMMETER_H_


