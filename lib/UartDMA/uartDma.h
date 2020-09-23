/*
 * UART_DMA.h
 *
 *  Created on: 09.12.2019
 *      Author: Mateusz Salamon
 *		www.msalamon.pl
 *
 *      Website: https://msalamon.pl/odbieranie-uart-z-dma-na-f103-to-rowniez-jest-proste/
 *      GitHub:  https://github.com/lamik/UART_DMA_receiving_F103
 *      Contact: mateusz@msalamon.pl
 *
 *      Modified on: 20.09.2020
 *      Modifications: Norbert Bielak (detmot6)
 *
 *      Remember to call DmaIrqHandler and UartIrqHandler in stm32f1xx_it.c or stm32f4xx_it.c
 *      and use return statement after to avoid calling HAL built-in irq handlers
 */


#include "main.h"
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif


#define DMA_RX_BUFFER_SIZE          64
#define UART_BUFFER_SIZE            256

typedef struct UARTDMA_struct {
    UART_HandleTypeDef *huart;          // UART handler

    uint8_t DMA_RX_Buffer[DMA_RX_BUFFER_SIZE];  // DMA direct buffer
    uint8_t UART_Buffer[UART_BUFFER_SIZE];    // UART working circular buffer

    uint16_t UartBufferHead;
    uint16_t UartBufferTail;
    uint8_t UartBufferLines;
    void (*StringReceivedEventCallback)(struct UARTDMA_struct *huartdma, char* buffer);
} UARTDMA_HandleTypeDef;

void UARTDMA_UartIrqHandler(UARTDMA_HandleTypeDef *huartdma);

void UARTDMA_DmaIrqHandler(UARTDMA_HandleTypeDef *huartdma);

uint8_t UARTDMA_IsDataReady(UARTDMA_HandleTypeDef *huartdma);

int UARTDMA_GetLineFromBuffer(UARTDMA_HandleTypeDef *huartdma, char *OutBuffer);

int UARTDMA_GetCharFromBuffer(UARTDMA_HandleTypeDef *huartdma);

void UARTDMA_Init(UARTDMA_HandleTypeDef *huartdma, UART_HandleTypeDef *huart,
                                            void (*StringReceivedEventCallback)(UARTDMA_HandleTypeDef *huartdma, char* buffer));

void UARTDMA_StringReceivedEvent(UARTDMA_HandleTypeDef* huartdma, char* buffer);

#ifdef __cplusplus
}
#endif
