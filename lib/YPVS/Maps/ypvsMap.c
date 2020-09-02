#include <stdbool.h>
#include <string.h>
#include "../../../Inc/framework.h"

#include "ypvsMap.h"




static bool ypvsMapCheck(ypvsMap* map){
  for(uint8_t i = 0; i < MAX_STEPS; i++){
    if(map->steps[i].rpm < MIN_OPEN_RPM) return false;
    if(map->steps[i].rpm > MAX_RPM) return false;
  }
  return true;
}








ypvsMap* ypvsMap_Init(ypvsMap* hmap, ypvsMapStep* steps, uint16_t openingStartRPM, uint16_t fullOpenRPM){
  hmap->steps = steps;
  hmap->fullOpenRPM = fullOpenRPM;
  hmap->openingStartRPM = openingStartRPM;
  ypvsMapCheck(hmap);
  return hmap;
}


