#include "adc.h"

//TODO change polling to interrupt
//Polling,
uint8_t readADC(void){

	if(HAL_ADC_PollForConversion(&hadc1, 1000000) == HAL_OK){
		HAL_ADC_GetValue(&hadc1);
	}

	return HAL_OK;
};

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){

	return;
}
