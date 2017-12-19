#ifndef ADC_H_
#define ADC_H_

#include "stm32f4xx_hal.h"
#include "LLInit.h"

//extern "C"
//{
//	void HAL_ADC_ConvCpltCallBack(ADC_HandleTypeDef* AdcHandle){
//		g_ADCValue = HAL_ADC_GetValue(AdcHandle);
//	}
//
//	void ADC_IRQHandler(){
//		HAL_ADC_IRQHandler(&hacd1);
//	}
//}

uint8_t readADC(void);
void initDMA(void);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);


#endif /*ADC_H_*/
