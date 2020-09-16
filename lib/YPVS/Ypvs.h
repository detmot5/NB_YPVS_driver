#ifndef YPVS_H
#define YPVS_H
#include "../../Inc/framework.h"
#include "Maps/ypvsMap.h"

uint8_t getYpvsCurrentPos();
void ypvsInit(void (*errorHandler)());
void ypvsRun(void);
#endif // !YPVS_H
