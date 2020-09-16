#include <stdbool.h>
#include <string.h>
#include "framework.h"


#include "ypvsMap.h"




static bool ypvsMapCheck(ypvsMapStep* step){
  for(uint8_t i = 0; i < MAX_STEPS; i++){
    if(step[i].rpm < MIN_OPEN_RPM || step[i].rpm > MAX_RPM) return false;
  }
  return true;
}



bool ypvsMap_IsInitialized(ypvsMap* hmap){
  return hmap->initializedProperly;
}







ypvsMap* ypvsMap_Init(ypvsMap* hmap, ypvsMapStep* steps, uint16_t openingStartRPM, uint16_t fullOpenRPM){

  if(ypvsMapCheck(steps)){
    hmap->steps = steps;
    hmap->fullOpenRPM = fullOpenRPM;
    hmap->openingStartRPM = openingStartRPM;
    hmap->initializedProperly = true;
  }
  else{
    hmap->steps = NULL;
    hmap->fullOpenRPM = 0;
    hmap->openingStartRPM = 0;
    hmap->initializedProperly = false;
  }

  return hmap;
}


