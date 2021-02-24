#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef int32_t CircularBufferDataType;

typedef struct CircularBuffer_s{
  volatile uint16_t* buffer;
  uint16_t head;
  uint16_t tail;
  uint16_t size;
}CircularBuffer_t;


void CircularBuffer_Init(volatile CircularBuffer_t* hbuff, volatile uint16_t* buffer, uint16_t size);
bool CircularBuffer_PushBack(volatile CircularBuffer_t* hbuff, uint16_t data);
bool CircularBuffer_Get(volatile CircularBuffer_t* hbuff, uint16_t* data);
bool CircularBuffer_Fill(volatile CircularBuffer_t* hbuff, uint16_t value);
uint32_t CircularBuffer_GetAverageValue(volatile CircularBuffer_t* hbuff);


#endif // !CIRCULAR_BUFFER_H
