//
// Created by norbe on 20/09/2020.
//

#ifndef UARTDMA_ATCOMMANDSSERVICE_H
#define UARTDMA_ATCOMMANDSSERVICE_H
#include "main.h"




typedef enum inout_enum { ask = (uint8_t)0, param = (uint8_t)1, noparam = (uint8_t)2 }inout_t;
typedef enum atresult_enum {AT_OK = (int8_t)0, AT_ERROR = (int8_t)-1} atresult_t;



typedef struct atcmd_s{
    char AT_command[10];
    atresult_t (* at_service)(uint8_t inout, char *params);
}atcmd_t;


typedef struct AT_CommandService_s{
    UART_HandleTypeDef* huart;
    struct atcmd_s* AT_Commands;
    uint8_t atCommands_cnt;
}AT_CommandService_t;


void AT_CommandService_Init(AT_CommandService_t* atCommandService,
                            atcmd_t* AT_Commands,
                            UART_HandleTypeDef* huart,
                            uint8_t atCommands_cnt);

void sendAnswer(AT_CommandService_t* atCommandService, const char* message);
void AT_StringParseEventCallback(AT_CommandService_t* atCommandService, char *pbuf);


#endif //UARTDMA_ATCOMMANDSSERVICE_H
