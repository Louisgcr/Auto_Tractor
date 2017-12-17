#ifndef __LLINIT_H
#define __LLINIT_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"

CanTxMsgTypeDef CAN1_TxMessage;
CanRxMsgTypeDef CAN1_RxMessage;

ADC_HandleTypeDef hadc1;
CAN_HandleTypeDef hcan1;


void lowLevelInit(void);
extern void initialise_monitor_handles(void);
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_CAN1_Init(void);
void MX_ADC1_Init(void);

#endif /* LLINIT_H*/
