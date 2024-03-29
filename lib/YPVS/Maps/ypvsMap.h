#ifndef YPVSMAP_H
#define YPVSMAP_H

#define MAX_STEPS 11
#define MIN_OPEN_RPM 2000
#include <stdint.h>
#include <stdbool.h>


typedef struct _ypvsMapStep{
  uint16_t rpm;
  uint8_t ypvsOpenPercentage;
} ypvsMapStep;


typedef struct _ypvsMap{
  struct _ypvsMapStep* steps;
  uint16_t fullOpenRPM;
  uint16_t openingStartRPM;
  bool initializedProperly;
} ypvsMap;


bool ypvsMap_IsInitialized(ypvsMap* hmap);
ypvsMap* ypvsMap_Init(ypvsMap* hmap, ypvsMapStep* steps, uint16_t openingStartRPM, uint16_t fullOpenRPM);



#endif 