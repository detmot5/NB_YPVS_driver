#include "Ypvs.h"
#include "../../Inc/framework.h"

#include "Maps/ypvsMap.h"

static HardwareServo_t hservo;
static ypvsMap map;
static inline long mapValue(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


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





ypvsMapStep getCurrentStep(ypvsMap* hmap){
  ypvsMapStep biggest = hmap->steps[0];
  uint16_t currentRpm = getEngineRPM();
  for(int i = 0; i < MAX_STEPS; i++){

    if(currentRpm < hmap->steps[i].rpm){
      biggest = hmap->steps[i];
      break;
    }
  }
 // printf("%d %d %d", map.steps[MAX_STEPS-1].rpm, biggest.ypvsOpenPercentage);
  if(currentRpm >= map.steps[MAX_STEPS-1].rpm) {
    biggest = map.steps[MAX_STEPS-1];
  }

  return biggest;
}


uint16_t getCurrentServoPulseWidth(ypvsMap* hmap){
  uint8_t servoPulseInPercent = getCurrentStep(hmap).ypvsOpenPercentage;
  /*printf("%u %u\n",servoPulseInPercent, hservo.actualPulseWidth);*/
  uint16_t servoPulseWidth = (uint16_t) mapValue(servoPulseInPercent, 0, 100,1000,2200);
  return servoPulseWidth;
}

static void selfTest(void){
  hServo_Write_us(&hservo, 1500);
  HAL_Delay(1000);
  hServo_Write_us(&hservo, 1000);
  HAL_Delay(1000);
}


uint8_t getYpvsCurrentPos(){
  return getCurrentStep(&map).ypvsOpenPercentage;
}


void ypvsInit(void){
  hservo = *hServo_Init(&hservo, &servoTim, servoTimChannel, 800, 2200, 0);
  map = *ypvsMap_Init(&map, steps, 3000, 6000);
  selfTest();
  for(int i = 0; i < MAX_STEPS; i++){
    printf("%d %d\n", map.steps[i].rpm, map.steps[i].ypvsOpenPercentage);
  }
}


void ypvsRun(void){
  hServo_Write_us(&hservo, getCurrentServoPulseWidth(&map));
}









