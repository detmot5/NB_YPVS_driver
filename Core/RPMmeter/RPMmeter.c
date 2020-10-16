#include "RPMmeter.h"
#include "../CircularBuffer/circularBuffer.h"

#define MINUTE 60
#define MAX_RPM 12000
#define RPM_BUFFER_SIZE 25
#define RPM_OFFSET 250

static volatile uint16_t rpmBufferArray[RPM_BUFFER_SIZE] = {0};
static volatile CircularBuffer_t rpmBuffer = {0};
static volatile uint32_t engineRPM = 0;
static volatile uint32_t engineFrequency = 0;           // in Hz
static volatile uint8_t rpmState = 0;



static volatile bool _isEngineRunning = false;
static volatile uint16_t engineRunningCounter = 0;

static inline void refreshEngineRunningCounter(){
  engineRunningCounter = 1000;
}

void rpmMeterIrqHandler(TIM_HandleTypeDef* htim, uint32_t timChannel) {
	uint32_t capturedValue = HAL_TIM_ReadCapturedValue(htim, timChannel);
	const uint32_t timPrescaler = htim->Init.Prescaler + 1;

	if(capturedValue != 0) {
		engineFrequency = (SystemCoreClock / timPrescaler) / capturedValue;
		engineRPM = engineFrequency * MINUTE;
	} else {
		engineFrequency = 0;
		engineRPM = 0;
	}
  CircularBuffer_PushBack(&rpmBuffer, engineRPM);
	refreshEngineRunningCounter();
}





void rpmMeterInit() {
  CircularBuffer_Init(&rpmBuffer, rpmBufferArray, RPM_BUFFER_SIZE);
}

void checkIsEngineRunning(){
  if(engineRunningCounter > 0) {
    --engineRunningCounter;
    _isEngineRunning = true;
  } else {
    _isEngineRunning = false;
    CircularBuffer_Fill(&rpmBuffer, 0); //set RPM to 0
  }
}


void checkRPMstate(){
  static uint16_t prevRPM = 0;
  uint32_t actualRPM = getEngineRPM();

  if( (int32_t)(actualRPM - prevRPM) >= RPM_OFFSET ){
    rpmState = RPM_STATE_INCREASING;
    prevRPM = actualRPM;
  }
  else if( (int32_t)(prevRPM - actualRPM) >= RPM_OFFSET ){
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




