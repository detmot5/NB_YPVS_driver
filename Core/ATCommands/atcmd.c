//
// Created by norbe on 21/09/2020.
//


#include <stdlib.h>
#include "atcmd.h"
AT_CommandService_t atCommandService;


const char info[] = "Yamaha Power Valve System - NB YPVS Driver\nWARNING - this device has a development version software\n"
                    "Some functions may not work properly\n";

const char notInDebugMode[] = "This command is available only in device debug mode\n";

atresult_t at_service(uint8_t inout, char* params){
  return AT_OK;
}

atresult_t ati_service(uint8_t inout, char* params){
  sendAnswer(&atCommandService, info);

  return AT_OK;
}

atresult_t at_rpm_service(uint8_t inout, char* params){
#if DEBUG_MODE
  if(inout == param){
  char* conversionErrorPtr;
  uint16_t res = strtol(params, &conversionErrorPtr, 10);
  if(*conversionErrorPtr) return AT_ERROR;
  if(res < 2499 || res > 19999) return AT_ERROR;

  __HAL_TIM_SET_AUTORELOAD(&htim1, res);




  }
  else if(inout == ask){
    char answer[20];
    sprintf(answer, "AT_RPM=%lu\n",getEngineRPM());
    sendAnswer(&atCommandService, answer);
  }
  return AT_OK;

#else
  sendAnswer(&atCommandService, notInDebugMode);
#endif

  return AT_ERROR;
}

atresult_t at_srpm_service(uint8_t inout, char* params){
  if(inout == ask){
    char answerBuf[20];
    sprintf(answerBuf, "%d", getRPMState());
    sendAnswer(&atCommandService, answerBuf);
  }
  return AT_OK;
}





atcmd_t commands[AT_CNT] = {
        {"AT", at_service},
        {"ATI", ati_service},
        {"AT+RPM", at_rpm_service},
        {"AT+SRPM", at_srpm_service}
};




