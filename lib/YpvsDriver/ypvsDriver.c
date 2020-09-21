//
// Created by norbe on 18/09/2020.
//
#include "Maps/ypvsMap.h"
#include "Ypvs.h"
#include "framework.h"
#include "ypvsDriver.h"

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

uint16_t getCurrentServoPulseWidth(ypvsMap* hmap) {
  ypvsMapStep currentStep = getCurrentStep(hmap, getEngineRPM());
  uint8_t servoPulseInPercent = currentStep.ypvsOpenPercentage;
  uint16_t servoPulseWidth = (uint16_t) mapValue(servoPulseInPercent, 0, 100,1300,2400);

  return servoPulseWidth;
}



static void selfTest(void) {
  hServo_Write_us(&hservo, 2400);
  HAL_Delay(400);
  hServo_Write_us(&hservo, 1300);
  HAL_Delay(500);
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

//  printf("%u %u\n", getCurrentStep(&map).ypvsOpenPercentage, getEngineRPM());
}

