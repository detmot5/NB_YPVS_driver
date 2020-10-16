#include <stdio.h>
#include "framework.h"
#include "app.h"
#include "../Core/YpvsDriver/ypvsDriver.h"
#include "YPVS/Ypvs.h"


// ***** Peripheral Handlers *******

tickTimer ledBuiltinTim;
tickTimer ypvsTim;
tickTimer rpmStateCheckTim;


UARTDMA_HandleTypeDef huartDma;

char USART1_StringBuffer[50];


// ***** Ticktimer Callbacks *******
static void ledBuiltinBlink(tickTimer* tim) {
  HAL_GPIO_TogglePin(LED_BUILTIN_GPIO_Port, LED_BUILTIN_Pin);
  //printf("%lu %u %u \n", getEngineFrequency(), getEngineRPM(), getRPMState());
}

static void runYpvsHandler(tickTimer* tim) {
  ypvsRun();
}



static void ypvsErrorHandler(void) {
  puts("YPVS INTERNAL ERROR");
  tickTimer_Stop(&ledBuiltinTim);

  tickTimer_Stop(&ypvsTim);
  HAL_GPIO_WritePin(LED_BUILTIN_GPIO_Port, LED_BUILTIN_Pin, GPIO_PIN_SET);
}



// ***** Initializations *******



static void initTimers(void) {
  HAL_TIM_IC_Start_IT(&rpmTim, rpmTimChannel_1);
  HAL_TIM_IC_Start(&rpmTim, rpmTimChannel_2);
  HAL_TIM_PWM_Start(&servoTim, servoTimChannel);
  HAL_TIM_Base_Start_IT(&htim1);
  HAL_TIM_Base_Start_IT(&rpmTim);

  tickTimer_Init(&ledBuiltinTim, 500, true, ledBuiltinBlink);
  tickTimer_Init(&ypvsTim, 1, true, runYpvsHandler);
  tickTimer_Init(&rpmStateCheckTim, 25, true, checkRPMstate);
}


static void USART1_StringReceivedCallback(UARTDMA_HandleTypeDef* huartdma, char* str){
  AT_StringParseEventCallback(&atCommandService, str);
}



//********************** OUTSIDE FUNCTIONS ***************************

void initPeripherals(void) {

  initTimers();
  uart_init_printf(&huart1);
  rpmMeterInit();
  ypvsInit(ypvsErrorHandler);
  UARTDMA_Init(&huartDma, &huart1, USART1_StringReceivedCallback);
  AT_CommandService_Init(&atCommandService, commands, &huart1, AT_CNT);

  puts("System start");

}

void mainLoop(void) {
  tickTimer_RunTask(&ledBuiltinTim);
 // tickTimer_RunTask(&ypvsTim);
  ypvsRun();
  tickTimer_RunTask(&rpmStateCheckTim);
  UARTDMA_StringReceivedEvent(&huartDma, USART1_StringBuffer);
}



void handle_TIM_IC_interrupts(TIM_HandleTypeDef* htim) {
  if ( htim->Instance == rpmTim.Instance ) {
    if ( htim->Channel == rpmTimActiveChannel_1 ) {
      rpmMeterIrqHandler(htim, rpmTimChannel_1);
    }
  }
}

void handle_TIM_PeriodElapsed_interrupts(TIM_HandleTypeDef* htim) {

  if ( htim->Instance == TIM1 ) {
    HAL_GPIO_TogglePin(SIM_RPM_GPIO_Port, SIM_RPM_Pin);
  } else if ( htim->Instance == rpmTim.Instance ){
    //rpmMeterTimPeriodElapsedIrqHandler(htim);
  }
}

