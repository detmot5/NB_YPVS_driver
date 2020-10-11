/*
 * UART_DMA.c
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
 *      Modifications: Norbert Bielak (detmot5)
 */

#include "uartDma.h"








void UARTDMA_UartIrqHandler(UARTDMA_HandleTypeDef *huartdma)
{
  if(huartdma->huart->Instance->SR & UART_FLAG_IDLE)       // Check if Idle flag is set
  {
    volatile uint32_t tmp;
    tmp = huartdma->huart->Instance->SR;                      // Read status register
    tmp = huartdma->huart->Instance->DR;                      // Read data register
    huartdma->huart->hdmarx->Instance->CCR &= ~DMA_CCR_EN; 	  // Disable DMA - it will force Transfer Complete interrupt if it's enabled
    // BUT! It's only for DMA Streams(i.e F4), not for Channels like there in F103!
    tmp = tmp; // For unused warning
    UARTDMA_DmaIrqHandler(huartdma); // Since DMA IRQ won't start independently for Channels, we have to handle it manually
  }
}

void UARTDMA_DmaIrqHandler(UARTDMA_HandleTypeDef *huartdma)
{
  uint8_t *UartBufferPointer, *DmaBufferPointer;
  uint32_t Length;
  uint16_t i, TempHead;

  typedef struct
  {
      __IO uint32_t ISR;   // DMA interrupt status register
      __IO uint32_t IFCR;  // DMA interrupt flag clear register
  } DMA_Base_Registers;

  DMA_Base_Registers *DmaRegisters = (DMA_Base_Registers *) huartdma->huart->hdmarx->DmaBaseAddress; // Take registers base address

  if (__HAL_DMA_GET_IT_SOURCE(huartdma->huart->hdmarx, DMA_IT_TC) != RESET) // Check if interrupt source is Transfer Complete
  {
    DmaRegisters->IFCR = DMA_IFCR_CTCIF1 << huartdma->huart->hdmarx->ChannelIndex;	// Clear Transfer Complete flag

    Length = DMA_RX_BUFFER_SIZE - huartdma->huart->hdmarx->Instance->CNDTR; // Get the Length of transfered data

    UartBufferPointer = huartdma->UART_Buffer;
    DmaBufferPointer = 	huartdma->DMA_RX_Buffer;

    // Write received data for UART main buffer - circular buffer
    for(i = 0; i < Length; i++)
    {
      TempHead = (huartdma->UartBufferHead + 1) % UART_BUFFER_SIZE;
      if(TempHead == huartdma->UartBufferTail)
      {
        huartdma->UartBufferHead = huartdma->UartBufferTail;	// No room for new data
      }
      else
      {
        UartBufferPointer[TempHead] = DmaBufferPointer[i];
        if(UartBufferPointer[TempHead] == 10)
        {
          huartdma->UartBufferLines++;
        }
        huartdma->UartBufferHead = TempHead;
      }
    }

    DmaRegisters->IFCR = 0x0FU << huartdma->huart->hdmarx->ChannelIndex; 		// Clear all interrupts
    huartdma->huart->hdmarx->Instance->CMAR = (uint32_t) huartdma->DMA_RX_Buffer; // Set memory address for DMA again
    huartdma->huart->hdmarx->Instance->CNDTR = DMA_RX_BUFFER_SIZE; // Set number of bytes to receive
    huartdma->huart->hdmarx->Instance->CCR |= DMA_CCR_EN;            	// Start DMA transfer
  }
}

int UARTDMA_GetCharFromBuffer(UARTDMA_HandleTypeDef *huartdma)
{
  if(huartdma->UartBufferHead == huartdma->UartBufferTail)
  {
    return -1; // error - no char to return
  }


  huartdma->UartBufferTail = (huartdma->UartBufferTail + 1) % UART_BUFFER_SIZE;

  return huartdma->UART_Buffer[huartdma->UartBufferTail];
}

uint8_t UARTDMA_IsDataReady(UARTDMA_HandleTypeDef *huartdma)
{
  if(huartdma->UartBufferLines)
    return 1;
  else
    return 0;
}

int UARTDMA_GetLineFromBuffer(UARTDMA_HandleTypeDef *huartdma, char *OutBuffer)
{
  char TempChar;
  char* LinePointer = OutBuffer;
  if(huartdma->UartBufferLines)
  {
    while((TempChar = UARTDMA_GetCharFromBuffer(huartdma)) > 0)
    {
      if(TempChar == '\n')
      {
        break;
      }
      *LinePointer = TempChar;
      LinePointer++;
    }
    *LinePointer = 0; // end of cstring
    huartdma->UartBufferLines--; // decrement line counter
  }
  return 0;
}

void UARTDMA_StringReceivedEvent(UARTDMA_HandleTypeDef* huartdma, char* buffer){
  if(UARTDMA_IsDataReady(huartdma)){
    UARTDMA_GetLineFromBuffer(huartdma, buffer);
    if(huartdma->StringReceivedEventCallback) huartdma->StringReceivedEventCallback(huartdma,buffer);
  }
}


void UARTDMA_Init(UARTDMA_HandleTypeDef *huartdma, UART_HandleTypeDef *huart,
                  void (*StringReceivedEventCallback)(UARTDMA_HandleTypeDef *huartdma, char* buffer))
{
  huartdma->huart = huart;

  __HAL_UART_ENABLE_IT(huartdma->huart, UART_IT_IDLE);   	// UART Idle Line interrupt
  __HAL_DMA_ENABLE_IT(huartdma->huart->hdmarx, DMA_IT_TC); // UART DMA Transfer Complete interrupt

  HAL_UART_Receive_DMA(huartdma->huart, huartdma->DMA_RX_Buffer, DMA_RX_BUFFER_SIZE); // Run DMA for whole DMA buffer

  huartdma->huart->hdmarx->Instance->CCR &= ~DMA_CCR_HTIE; // Disable DMA Half Complete interrupt

  if(StringReceivedEventCallback) huartdma->StringReceivedEventCallback = StringReceivedEventCallback;  //If you don't want to use event, just pass NULL instead of function pointer

}
