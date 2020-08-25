#include <stdbool.h>
#include "../../Inc/main.h"
#include "HardwareServo.h"

// Arduino map function
static inline long mapAngleToPulseWidth(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

static inline bool isPulseInCorrectRange(HardwareServo_t* hservo, uint16_t pulse){
  return (pulse >= hservo->minPulseWidth && pulse <= hservo->maxPulseWidth); 
}



//Servo object init function
//Input: servo handler, timer handler, timer channel and servo params
HardwareServo_t* hServo_Init(   
                                HardwareServo_t* hservo, 
                                TIM_HandleTypeDef* htim, 
                                uint32_t channel,
                                uint16_t minPulseW, 
                                uint16_t maxPulseW,
                                uint16_t actualPulseW
                            )
{
	hservo->timer = htim;
	hservo->timerChannel = channel;
	hservo->minPulseWidth = minPulseW;
	hservo->maxPulseWidth = maxPulseW;
	hservo->actualPulseWidth = (actualPulseW != 0) ? actualPulseW : minPulseW;


  return hservo;
}



bool hServo_Write_us(HardwareServo_t* hservo, uint32_t pulse){
  if(isPulseInCorrectRange(hservo, pulse)){
		__HAL_TIM_SET_COMPARE(hservo->timer, hservo->timerChannel, pulse);
		hservo->actualPulseWidth = pulse;
		return true;
	}
	return false;
}


bool hServo_Write_dg(HardwareServo_t* hservo, uint8_t angle){
	uint16_t pulse = mapAngleToPulseWidth(angle, 0, 180, hservo->minPulseWidth, hservo->maxPulseWidth);

	if(isPulseInCorrectRange(hservo, pulse)){
		__HAL_TIM_SET_COMPARE(hservo->timer, hservo->timerChannel, pulse);
		hservo->actualPulseWidth = pulse;
		return true;
	}
	return false;
}