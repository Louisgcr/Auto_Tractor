#ifndef PWM_H_
#define PWM_H_

#include "stm32f4xx_hal.h"
#include "LLInit.h"


uint32_t pulse_length;

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

void setPWM_Channel(uint8_t dutyCycle, uint8_t Channel){
	/* pulse_length = ((TIM_Period + 1) * dutyCycle)/100 -1
	 * 25% duty cycle: pulse_length  = ((PWM_Period)+ 1* 25)/100 -1 = 2099
	 * 50% duty cycle: pulse_length  = ((PWM_Period)+ 1* 50)/100 -1 = 4199
	 * 75% duty cycle: pulse_length  = ((PWM_Period)+ 1* 75)/100 -1 = 6299
	 * 100% duty cycle: pulse_length = ((PWM_Period)+ 1* 100)/100 -1 = 8399
	 */
	if (dutyCycle > 100){
		dutyCycle = 100;
	}else if(dutyCycle < 0){
		dutyCycle = 0;
	}
	pulse_length = (((PWM_Period + 1) * dutyCycle)/100) -1;

	switch (Channel){
		case 1:{
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,PWM_Period);
			return;
		}
		case 2:{
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,PWM_Period);
			return;
		}

		default:
			return;
		}
	return;
};

#endif /*PWM_H_*/
