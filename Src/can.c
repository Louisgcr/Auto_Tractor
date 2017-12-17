#include "can.h"

/*@brief  : Function to configure CAN filter message, to configure which filter number
 * 			what message ID to filter out and what range of message to filter out
 * 			Example: Desired filter message ID = 0x80~0xF0
 * 			0x80 = 0b1000 0000 ID
 * 			0xF0 = 0b1111 0000 mask 
 * 			Hence the value 1 of the mask means it has to match the ID while 0 means it doesn't matter
 *@param  : filterNum = Filter number to configure 0 ~ 28
 *@param  : msgID = Message ID to filter out
 *@param  : maskID = Mask ID to filter out
 * */
void CAN_Filter_Config(uint8_t filterNum, uint8_t msgId, uint8_t maskId)
{

	 CAN_FilterConfTypeDef sFilterConfig;
	 sFilterConfig.FilterNumber = filterNum;
	 sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	 sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	 sFilterConfig.FilterIdHigh = msgId<<5; //0b1000 0000  Filter from 0x80 ~ 0xF0
	 sFilterConfig.FilterIdLow = 0x0000;
	 sFilterConfig.FilterMaskIdHigh = maskId<<5;//0b1111 0000  0 means don't care, 1 means must match
	 sFilterConfig.FilterMaskIdLow = 0x0000;
	 sFilterConfig.FilterFIFOAssignment = CAN_FIFO0;
	 sFilterConfig.FilterActivation = ENABLE;
	 sFilterConfig.BankNumber = 14;//Which filter onwards CAN2 uses

	 HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);

	 hcan1.pTxMsg->StdId = 1; //Specifies the standard identifier.
	 hcan1.pTxMsg->ExtId = 0x01; //Specifies the external identifier.
	 hcan1.pTxMsg->RTR = CAN_RTR_DATA;// specifies type of frame message
	 hcan1.pTxMsg->IDE = CAN_ID_STD;//Specifies the type of identifier

}


/*@brief  : Function to send out can message to the CAN1 bus, it will configure the 
 * 			message ID and message length. Also it will ensure that the message length is
 * 			less than 8
 *@param  : data = data to send out to the CAN bus
 *@param  : msgLength = message length 
 *@param  : msgID = message ID
 *@return : 0 = Okay, 1 = Error
 * */
HAL_StatusTypeDef CAN1_Tx(uint8_t data[], uint8_t msgLength, uint8_t msgID)
{
  volatile HAL_StatusTypeDef TXstatus;
  TXstatus = HAL_OK;
  if (msgLength > 8){
	  return HAL_ERROR;
  }

  hcan1.pTxMsg->StdId = msgID;
  hcan1.pTxMsg->DLC = msgLength; // Specifies frame length
  for (int i = 0; i < msgLength; i ++){
	  hcan1.pTxMsg->Data[i] = data[i];
  }

  //TXstatus = HAL_CAN_Transmit(&hcan1, 1000);
  HAL_CAN_Transmit(&hcan1, 100);
  //if (TXstatus == 0){
  //
//	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, (data[0] & 0b0001));
//	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, (data[0] & 0b0010));
//	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, (data[0] & 0b0100));
//	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, (data[0] & 0b1000));
  //}
  return TXstatus;
}



void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef *RxCANHandle)
{

	//TODO add switch case to split message up
	canRX[0] = RxCANHandle->pRxMsg->Data[0];
	HAL_CAN_Receive_IT(&hcan1, CAN_FIFO0);

}
