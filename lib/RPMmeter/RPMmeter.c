#include "../../Inc/main.h"
#include "RPMmeter.h"

#define MINUTE 60


static volatile uint16_t engineRPM;
static volatile uint32_t engineFrequency;           // in Hz

void rpmMeterIrqHandler(TIM_HandleTypeDef* htim, uint32_t timChannel){
	uint16_t capturedValue = HAL_TIM_ReadCapturedValue(htim, timChannel);
	uint32_t timPrescaler = htim->Init.Prescaler + 1;

	if(capturedValue != 0) {	
		engineFrequency = (SystemCoreClock / timPrescaler) / capturedValue;
		engineRPM = engineFrequency * MINUTE;
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




