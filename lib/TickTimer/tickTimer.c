#include <stdbool.h>
#include "../../Inc/main.h" 
#include "tickTimer.h"

static inline bool isTimerReadyToRun(tickTimer* timer){
    if(true == timer->isRunning && NULL != timer->task && 0 < timer->interval){
        return true;
    }
    return false;
    
}




tickTimer* tickTimer_Init(tickTimer* timer, uint32_t interval, bool isRunning, void (*task)(tickTimer*)){
    timer->interval = interval;
    timer->isRunning = isRunning;
    timer->task = task;

    timer->previous_tick = isRunning ? HAL_GetTick() : 0;
    return timer;
}


void tickTimer_RunTask(tickTimer* timer){
    if(isTimerReadyToRun(timer)){   
        uint32_t actual = HAL_GetTick();
        if(actual - timer->previous_tick >= timer->interval){
            timer->task(timer);
            timer->previous_tick = actual;
        }
    }
}

void tickTimer_Stop(tickTimer* timer){
    timer->isRunning = false;
}

void tickTimer_Start(tickTimer* timer){
    timer->isRunning = true;
}

void tickTimer_SetInterval(tickTimer* timer, uint32_t interval){
    timer->interval = interval;
}

void tickTimer_SetTask(tickTimer* timer, void (*task)(tickTimer*)){
    timer->task = task;
}

uint32_t tickTimer_GetInterval(tickTimer* timer){
    return timer->interval;
}

