#ifndef YPVS_H
#define YPVS_H
#include <stdint.h>
#include "../Core/YpvsMap/ypvsMap.h"

ypvsMapStep getCurrentStep(ypvsMap* hmap, uint16_t currentRpm);
uint16_t getCurrentServoPulseWidth(ypvsMap* hmap);

#endif // !YPVS_H
