#include <stdio.h>
#include "framework.h"
#include "app.h"
#include <Ypvs.h>

// *****Basic ticktimers defines and handlers*******

tickTimer ledBuiltinTim;
tickTimer ypvsTim;

#if DEBUG_MODE
tickTimer simulateRPMTim;
#endif

static void ledBuiltinBlink(tickTimer* tim) {
  HAL_GPIO_TogglePin(LED_BUILTIN_GPIO_Port, LED_BUILTIN_Pin);
  printf("%lu %u, %u \n", getEngineFrequency(), getEngineRPM(), getYpvsCurrentPos());
}

static void runYpvsHandler(tickTimer* tim) {
#if DEBUG_MODE

  HAL_ADC_Start(&hadc1);
  while ( HAL_ADC_PollForConversion(&hadc1, 2) != HAL_OK ) continue;
  uint32_t adcValue = HAL_ADC_GetValue(&hadc1);
  adcValue = mapValue(adcValue, 0, 4095, 3999, 14999);
  __HAL_TIM_SET_AUTORELOAD(&htim1, adcValue);

#endif

  ypvsRun();
}

static void ypvsErrorHandler(void) {
  puts("YPVS INTERNAL ERROR");
  tickTimer_Stop(&ledBuiltinTim);
  tickTimer_Stop(&ypvsTim);
  HAL_GPIO_WritePin(LED_BUILTIN_GPIO_Port, LED_BUILTIN_Pin, GPIO_PIN_SET);
}

// END *****Basic ticktimers defines and handlers*******



static void initTimers(void) {
  HAL_TIM_IC_Start_IT(&rpmTim, rpmTimChannel_1);
  HAL_TIM_IC_Start(&rpmTim, rpmTimChannel_2);
  HAL_TIM_PWM_Start(&servoTim, servoTimChannel);
  HAL_TIM_Base_Start_IT(&htim1);
}



//********************** OUTSIDE FUNCTIONS ***************************

void initPeripherals(void) {
  initTimers();
  uart_init_printf(&huart1);
  ypvsInit(ypvsErrorHandler);
  HAL_ADC_Stop(&hadc1);
  HAL_ADCEx_Calibration_Start(&hadc1);
  HAL_ADC_Start(&hadc1);
  ledBuiltinTim = *tickTimer_Init(&ledBuiltinTim, 200, true, ledBuiltinBlink);
  ypvsTim = *tickTimer_Init(&ypvsTim, 40, true, runYpvsHandler);
}

void mainLoop(void) {
  tickTimer_RunTask(&ledBuiltinTim);
  tickTimer_RunTask(&ypvsTim);
}



void handle_TIM_IC_interrupts(TIM_HandleTypeDef* htim) {
  if ( htim->Instance == rpmTim.Instance ) {
    if (htim->Channel == rpmTimActiveChannel_1) {
      rpmMeterIrqHandler(htim, rpmTimChannel_1);
    }
  }
}

void handle_TIM_PeriodElapsed_interrupts(TIM_HandleTypeDef* htim) {
  if ( htim->Instance == TIM1 ) {
    HAL_GPIO_TogglePin(SIM_RPM_GPIO_Port, SIM_RPM_Pin);
  }
}

