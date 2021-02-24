#include "Ypvs.h"


static inline uint8_t calculateOffset(uint16_t actualRpm, uint16_t prevRpm) {
  static uint8_t offset = 0;
  if (actualRpm < prevRpm) offset = 100;
  else if (actualRpm > prevRpm) offset = 0;
  return offset;
}





ypvsMapStep getCurrentStep(ypvsMap* hmap, uint16_t currentRpm) {
  static ypvsMapStep prevStep = {0, 0};
  static uint8_t offset;
  ypvsMapStep actualStep = hmap->steps[0];

  for (int i = 1; i < MAX_STEPS; i++) {
    if (currentRpm < hmap->steps[i].rpm - offset) {
      actualStep = hmap->steps[i-1];
      break;
    }
  }

  if (currentRpm >= hmap->steps[MAX_STEPS-1].rpm - 100) {
    actualStep = hmap->steps[MAX_STEPS-1];
  } else if(currentRpm < 400) {
    actualStep = hmap->steps[5];
  }

  offset = calculateOffset(actualStep.rpm, prevStep.rpm);
  prevStep = actualStep;

  return actualStep;
}

















