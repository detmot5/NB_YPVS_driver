#include "../../Inc/framework.h"
#include "RPMmeter.h"

#define MINUTE 60
#define MAX_RPM 12000
#define MAX_FREQ 200

static volatile uint16_t engineRPM;
static volatile uint32_t engineFrequency;           // in Hz

void rpmMeterIrqHandler(TIM_HandleTypeDef* htim, uint32_t timChannel){
	uint16_t capturedValue = HAL_TIM_ReadCapturedValue(htim, timChannel);
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


uint16_t getEngineRPM(){
	return engineRPM;
}

uint32_t getEngineFrequency(){
	return engineFrequency;
}




