#include "framework.h"
#include "RPMmeter.h"

#define MINUTE 60
#define MAX_RPM 12000
#define RPM_OFFSET 250


static volatile uint16_t engineRPM;
static volatile uint32_t engineFrequency;           // in Hz
static volatile uint8_t rpmState;


void rpmMeterIrqHandler(TIM_HandleTypeDef* htim, uint32_t timChannel){
	uint32_t capturedValue = HAL_TIM_ReadCapturedValue(htim, timChannel);
	uint32_t timPrescaler = htim->Init.Prescaler + 1;
  uint32_t tempFrequency, tempRPM;
  
	if(capturedValue != 0) {	
		tempFrequency = (SystemCoreClock / timPrescaler) / capturedValue;
		tempRPM = tempFrequency * MINUTE;

		engineFrequency = tempFrequency;
    engineRPM = tempRPM;
	} 
	else {
		engineFrequency = 0;
		engineRPM = 0;
	}
}


void checkRPMstate(){
  static uint16_t prevRPM = 0;
  uint16_t actualRPM = getEngineRPM();

  if( (int16_t)(actualRPM - prevRPM) >= 250 ){
    rpmState = RPM_STATE_INCREASING;
    prevRPM = actualRPM;
  }
  else if( (int16_t)(prevRPM - actualRPM) >= 250 ){
    rpmState = RPM_STATE_DECREASING;
    prevRPM = actualRPM;
  }
  else rpmState = RPM_STATE_CONSTANT;
}


uint8_t getRPMState(){
  return rpmState;
}


uint16_t getEngineRPM(){
	return engineRPM;
}

uint32_t getEngineFrequency(){
	return engineFrequency;
}




