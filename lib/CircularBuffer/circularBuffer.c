#include <stdbool.h>
#include "circularBuffer.h"



void CircularBuffer_Init(volatile CircularBuffer_t* hbuff, volatile uint16_t* buffer, uint16_t size) {
  hbuff->head = 0;
  hbuff->tail = 0;
  hbuff->buffer = buffer;
  hbuff->size = size;
}

bool CircularBuffer_PushBack(volatile CircularBuffer_t* hbuff, uint16_t data) {
  uint16_t nextHead = hbuff->head + 1;

  if(nextHead >= hbuff->size) nextHead = 0;
  if(nextHead == hbuff->tail) hbuff->tail = 0;

  hbuff->buffer[nextHead] = data;

  hbuff->head = nextHead;
  return true;
}


bool CircularBuffer_Get(volatile CircularBuffer_t* hbuff, uint16_t* data) {
  if(hbuff->head == hbuff->tail) return false;

  hbuff->tail++;
  if(hbuff->tail == hbuff->size) hbuff->tail = 0;

  *data = hbuff->buffer[hbuff->tail];
  return true;
}




uint32_t CircularBuffer_GetAverageValue(volatile CircularBuffer_t* hbuff){
  uint32_t tmp = 0;

  for (uint16_t i = 0; i < hbuff->size; i++) {
    tmp += hbuff->buffer[i];
  }
  return tmp / hbuff->size;
}

