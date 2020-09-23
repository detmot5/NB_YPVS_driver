//
// Created by norbe on 21/09/2020.
//


#include <stdlib.h>
#include "atcmd.h"
AT_CommandService_t atCommandService;


const char info[] = "Yamaha Power Valve System - NB YPVS Driver\nWARNING - this device has a development version software\n"
                    "Some functions may not work properly\n";

atresult_t at_service(uint8_t inout, char* params){
  return AT_OK;
}

atresult_t ati_service(uint8_t inout, char* params){
  sendAnswer(&atCommandService, info);
  return AT_OK;
}

atresult_t at_rpm_service(uint8_t inout, char* params){
  char* conversionErrorPtr;
  uint16_t res = strtol(params, &conversionErrorPtr, 10);
  if(*conversionErrorPtr) return AT_ERROR;

  res = mapValue(res, 0, 6000, 3999, 14999);

  __HAL_TIM_SET_AUTORELOAD(&htim1, res);

  return AT_OK;
}



atcmd_t commands[AT_CNT] = {
        {"AT", at_service},
        {"ATI", ati_service},
        {"AT+RPM", at_rpm_service},
};




