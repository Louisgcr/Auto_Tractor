#ifndef __CAN_H
#define __CAN_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"

#include "LLInit.h"

uint8_t RxData, canRX[8],canTX[8];

void CAN_Filter_Config(uint8_t filterNum, uint8_t msgId, uint8_t maskId);
HAL_StatusTypeDef CAN1_Tx(uint8_t data[], uint8_t msgLength, uint8_t msgID);
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef *RxCANHandle);

#endif /*__CAN_H*/
