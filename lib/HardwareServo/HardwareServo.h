#ifndef HARDWARESERVO_H
#define HARDWARESERVO_H

typedef struct _hardwareServo{
    TIM_HandleTypeDef* timer;
    uint32_t timerChannel;
    uint16_t minPulseWidth;
    uint16_t maxPulseWidth;
    uint16_t actualPulseWidth;

}HardwareServo_t;


HardwareServo_t* hServo_Init(   
                                HardwareServo_t* hservo, 
                                TIM_HandleTypeDef* htim, 
                                uint32_t channel,
                                uint16_t minPulseW, 
                                uint16_t maxPulseW,
                                uint16_t actualPulseW
                            );


bool hServo_Write_us(HardwareServo_t* hservo, uint32_t pulse);
bool hServo_Write_dg(HardwareServo_t* hservo, uint8_t angle);

uint16_t hservo_GetMax(HardwareServo_t* hservo);
uint16_t hservo_GetMin(HardwareServo_t* hservo);



#endif // !HARDWARESERVO_H
