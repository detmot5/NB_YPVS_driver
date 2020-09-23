//
// Created by norbe on 20/09/2020.
//

#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "ATCommandsService.h"


static const char endl[] = "\n";
static const char OK[] = "OK";
static const char errorCmd[] = "ERROR";
static const char unknownCmd[] = "ERROR: unknown cmd";

void AT_CommandService_Init(AT_CommandService_t* atCommandService,
                            atcmd_t* AT_Commands,
                            UART_HandleTypeDef* huart,
                            uint8_t atCommands_cnt){
  atCommandService->huart = huart;
  atCommandService->AT_Commands = AT_Commands;
  atCommandService->atCommands_cnt = atCommands_cnt;
}


void sendAnswer(AT_CommandService_t* atCommandService, const char* message){
  HAL_UART_Transmit(atCommandService->huart, (uint8_t*) message, strlen(message), 30);
}


void AT_StringParseEventCallback(AT_CommandService_t* atCommandService, char* pbuf){
  //AT command struct destructuring
  atcmd_t* atCommands = atCommandService->AT_Commands;
  uint8_t at_cnt = atCommandService->atCommands_cnt;


  atresult_t (*at_srv) (uint8_t inout, char *data);
  uint8_t len,i = 0;

  char *cmd_str;
  char *rest;

  if(strpbrk(pbuf,"=?")){
    if(strpbrk(pbuf,"?")){
      //device ask handle
      cmd_str = strtok_r(pbuf,"?",&rest);
      len = strlen(cmd_str);
      for(i = 0; i < at_cnt; i++){
        if(len && 0 == strncasecmp(cmd_str,atCommands[i].AT_command,len)){
          if(atCommands[i].at_service){
            at_srv = atCommands[i].at_service;
            if(at_srv) {
              if(at_srv(0, rest) < 0 ) sendAnswer(atCommandService, errorCmd);
            }
          }
          sendAnswer(atCommandService, endl);
          break;
        }
      }

    }
    else{
      // AT+CMD = parameters
      cmd_str = strtok_r(pbuf,"=", &rest);
      len = strlen(cmd_str);
      for(i = 0; i < at_cnt; i++){
        if(len && 0 == strncasecmp(cmd_str,atCommands[i].AT_command,len)){
          if(atCommands[i].at_service){
            at_srv = atCommands[i].at_service;
            if(at_srv && !at_srv(1,rest)) sendAnswer(atCommandService, OK);
            else sendAnswer(atCommandService, errorCmd);
          }
          sendAnswer(atCommandService, endl);
          break;
        }
      }
    }
  }
  else{
    //no parameters
    if(0 == pbuf[0]) sendAnswer(atCommandService, endl);
    else{
      for(i = 0; i < at_cnt; i++){
        if(0 == strcasecmp(pbuf,atCommands[i].AT_command)){
          if(atCommands[i].at_service){
            at_srv = atCommands[i].at_service;
            if(at_srv){
              if(at_srv(2,0)<0) sendAnswer(atCommandService, errorCmd);
              else sendAnswer(atCommandService, OK);
              sendAnswer(atCommandService, endl);
            }
          }
          break;
        }
      }
    }
  }
  if(at_cnt == i) {
    sendAnswer(atCommandService, unknownCmd);
    sendAnswer(atCommandService, endl);
  }
}

