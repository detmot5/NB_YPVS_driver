#include "RPMmeter.h"
#include "../CircularBuffer/circularBuffer.h"

#define MINUTE 60
#define MAX_RPM 12000
#define RPM_BUFFER_SIZE 25
#define RPM_OFFSET 250

static volatile uint16_t rpmBufferArray[RPM_BUFFER_SIZE] ;
static volatile CircularBuffer_t rpmBuffer;
static volatile uint32_t engineRPM;
static volatile uint32_t engineFrequency;           // in Hz
static volatile uint8_t rpmState;


void rpmMeterIrqHandler(TIM_HandleTypeDef* htim, uint32_t timChannel) {
	uint32_t capturedValue = HAL_TIM_ReadCapturedValue(htim, timChannel);
	uint32_t timPrescaler = htim->Init.Prescaler + 1;


  
	if(capturedValue != 0) {	
		engineFrequency = (SystemCoreClock / timPrescaler) / capturedValue;
		engineRPM = engineFrequency * MINUTE;
		CircularBuffer_PushBack(&rpmBuffer, engineRPM);
	} 
	else {
		engineFrequency = 0;
		engineRPM = 0;
	}
}

void rpmMeterInit() {
  CircularBuffer_Init(&rpmBuffer, rpmBufferArray, RPM_BUFFER_SIZE);
}



void checkRPMstate(){
  static uint16_t prevRPM = 0;
  uint32_t actualRPM = getEngineRPM();

  if( (int32_t)(actualRPM - prevRPM) >= 250 ){
    rpmState = RPM_STATE_INCREASING;
    prevRPM = actualRPM;
  }
  else if( (int32_t)(prevRPM - actualRPM) >= 250 ){
    rpmState = RPM_STATE_DECREASING;
    prevRPM = actualRPM;
  }
  else rpmState = RPM_STATE_CONSTANT;
}


uint8_t getRPMState(){
  return rpmState;
}


uint32_t getEngineRPM(){
	return CircularBuffer_GetAverageValue(&rpmBuffer);
}

uint32_t getEngineFrequency(){
	return engineFrequency;
}




