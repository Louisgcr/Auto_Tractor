#ifndef PWM_H_
#define PWM_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"

#define PWM_Period 8399

uint32_t pulse_length;

TIM_HandleTypeDef htim3;
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

static void MX_TIM3_Init(void){

	//TODO check if GPIO is init properly
	  TIM_MasterConfigTypeDef sMasterConfig;
	  TIM_OC_InitTypeDef sConfigOC;
	  //TODO Check if need to enable
	  /* Tim3 is connected to APB1 bus which runs at 42Mhz. But it has an internal PPL that brings it up to 84Mhz
	   * Prescaller is set to 0, hence timer_tick_frequemcy =  84000000/(0+1)=84000000
	  */
	  //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	  htim3.Instance = TIM3;
	  htim3.Init.Prescaler = 0;
	  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	  /* Tim3 is a 16 bit counter hence = 65535
	   * PWM_frequemcy = timer_tick_frequency / (TIM_Period + 1)
	   * Assuming PWM Frequency = 10k
	   * TIM_Period = 84000000/10000 -1 = 8399
	   * */
	  htim3.Init.Period = PWM_Period;
	  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
	  {
	    _Error_Handler(__FILE__, __LINE__);
	  }

	  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
	  {
	    _Error_Handler(__FILE__, __LINE__);
	  }

	  sConfigOC.OCMode = TIM_OCMODE_PWM1;
	  sConfigOC.Pulse = 0;
	  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	  {
	    _Error_Handler(__FILE__, __LINE__);
	  }

	  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
	  {
	    _Error_Handler(__FILE__, __LINE__);
	  }

	  HAL_TIM_MspPostInit(&htim3);
	  HAL_TIM_Base_Start(&htim3);
	  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);

};

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
