#ifndef TICKTIMER_H
#define TICKTIMER_H
#include <stdbool.h>




typedef struct _tickTimer{
    uint32_t interval;
    uint32_t previous_tick;
    bool isRunning;
    void (*task)(struct _tickTimer*);

}tickTimer;


tickTimer* tickTimer_Init(tickTimer* timer, uint32_t interval, bool isRunning, void (*task)(tickTimer*));
void tickTimer_RunTask(tickTimer* timer);

void tickTimer_Stop(tickTimer* timer);
void tickTimer_Start(tickTimer* timer);

void tickTimer_SetInterval(tickTimer* timer, uint32_t interval);
void tickTimer_SetTask(tickTimer* timer, void (*task)(tickTimer*));


uint32_t tickTimer_GetInterval(tickTimer* timer);



#endif