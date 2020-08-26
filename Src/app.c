#include "main.h"
#include "tim.h"
#include "usart.h"
#include "app.h"



// *****Basic ticktimers defines and handlers*******

tickTimer ledBuiltinTim;
#if DEBUG_MODE 
tickTimer simulateRPMTim;
#endif 

static void ledBuiltinBlink(tickTimer* tim){
  HAL_GPIO_TogglePin(LED_BUILTIN_GPIO_Port, LED_BUILTIN_Pin);
  printf("%ld %d \n", getEngineFrequency(), getEngineRPM());
}

static void simulateRPMhandler(tickTimer* tim){
  HAL_GPIO_TogglePin(SIM_RPM_GPIO_Port, SIM_RPM_Pin);
}


// END *****Basic ticktimers defines and handlers*******



static void initTimers(void){
  HAL_TIM_IC_Start_IT(&rpmTim, rpmTimChannel_1);
  HAL_TIM_IC_Start(&rpmTim, rpmTimChannel_2);
  HAL_TIM_PWM_Start(&servoTim, servoTimChannel);
}


void initPeripherals(void){
	initTimers();
	uart_init_printf(&huart1);
	ledBuiltinTim = *tickTimer_Init(&ledBuiltinTim, 400, true, ledBuiltinBlink);
#if DEBUG_MODE
  simulateRPMTim = *tickTimer_Init(&simulateRPMTim, 5, true, simulateRPMhandler);
#endif

}


void handle_TIM_IC_interrupts(TIM_HandleTypeDef* htim){
	if(htim->Instance == rpmTim.Instance){
		if(htim->Channel == rpmTimActiveChannel_1){
			rpmMeterIrqHandler(htim,rpmTimChannel_1);
		}
	}
}
