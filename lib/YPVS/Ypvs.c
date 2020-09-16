#include "Ypvs.h"
#include "framework.h"

#include "Maps/ypvsMap.h"

static HardwareServo_t hservo;
static ypvsMap map;

static void (*ypvsErrorHandler)(void);


ypvsMapStep steps[MAX_STEPS] = {
        {3000, 0},
        {3300, 10},
        {3600, 20},
        {3900, 30},
        {4200, 40},
        {4500, 50},
        {4800, 60},
        {5100, 70},
        {5400, 80},
        {5700, 90},
        {6000, 100},
};

ypvsMapStep  steps2[MAX_STEPS] = {
        {3000, 10},
        {3300, 10},
        {3600, 10},
        {3900, 10},
        {4200, 10},
        {4500, 10},
        {4800, 50},
        {5100, 50},
        {5400, 50},
        {5700, 50},
        {6000, 100},
};


static inline uint8_t calculateOffset(uint16_t actualRpm, uint16_t prevRpm) {
  static uint8_t offset = 0;
  if(actualRpm < prevRpm) offset = 100;
  else if(actualRpm > prevRpm) offset = 0;
  return offset;
}




ypvsMapStep getCurrentStep(ypvsMap* hmap) {
  static ypvsMapStep prevStep = {0,0};
  static uint8_t offset;
  ypvsMapStep actualStep = hmap->steps[0];
  uint16_t currentRpm = getEngineRPM();


  for(int i = 1; i < MAX_STEPS; i++) {
    if(currentRpm < hmap->steps[i].rpm - offset){
      actualStep = hmap->steps[i-1];
      break;
    }
  }

  if(currentRpm >= map.steps[MAX_STEPS-1].rpm - 100) {
    actualStep = map.steps[MAX_STEPS-1];
  }

  offset = calculateOffset(actualStep.rpm, prevStep.rpm);
  prevStep = actualStep;

  return actualStep;
}


uint16_t getCurrentServoPulseWidth(ypvsMap* hmap) {
  ypvsMapStep currentStep = getCurrentStep(hmap);
  uint8_t servoPulseInPercent = currentStep.ypvsOpenPercentage;
  uint16_t servoPulseWidth = (uint16_t) mapValue(servoPulseInPercent, 0, 100,1300,2400);

  return servoPulseWidth;
}

static void selfTest(void) {
  hServo_Write_us(&hservo, 2400);
  HAL_Delay(400);
  hServo_Write_us(&hservo, 1000);
  HAL_Delay(500);
}


uint8_t getYpvsCurrentPos() {
  return getCurrentStep(&map).ypvsOpenPercentage;
}


void ypvsInit(void (*errorHandler)()) {
  if(errorHandler) ypvsErrorHandler = errorHandler;

  hservo = *hServo_Init(&hservo, &servoTim, servoTimChannel, 400, 2400, 0);
  map = *ypvsMap_Init(&map, steps, 3000, 6000);
  selfTest();
}


void ypvsRun(void){

  if (ypvsMap_IsInitialized(&map)) {
    hServo_Write_us(&hservo, getCurrentServoPulseWidth(&map));
  }
  else if (ypvsErrorHandler) {
    ypvsErrorHandler();
  }

  printf("%u %u\n", getCurrentStep(&map).ypvsOpenPercentage, getEngineRPM());
}









