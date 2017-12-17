#include "main.h"
#include "packet.h"
#include "LLInit.h"
#include "can.h"
#include "stm32f4xx_hal.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "pwm.h"

uint16_t i = 0;
uint8_t receive_data[64];


//Create different messages for different parameteres
//struct serialMsg brakes;
//struct serialMsg steeringAngle;
//struct serialMsg odometer;
//struct serialMsg wheelEncoder;

void LightLED(void)
{

	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, (canRX[0] & 0b0001));
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, (canRX[0] & 0b0010));
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, (canRX[0] & 0b0100));
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, (canRX[0] & 0b1000));

}

int main(void)
{
  //TODO list
  //Confire PWM
  //Configure USB to receive data
  //Configure CAN Filters to make sure that they filter out the right message
  //
  lowLevelInit();
  CAN_Filter_Config(0, 0x80, 0xF0);

  vcpPacket *throttle = vcpMsgPtr(1, 2);


  while (1)
  {
	  i++;
	  canRX[0] = (uint8_t)i;
	  //CAN1_Tx(canTX, 1, 0x80); // Transmit data
	  //sprintf(mystring, "CAN Data %u\r\n", canRX[0]);
	  //sendUSBPacket(mystring, 14);



	  //TODO change code to make the thing more abstract
	  uint8_t msgLen = throttle->data[2];
	  throttle->data[4] = (i>>8) & 0xFF;
	  throttle->data[5] = i & 0xFF;
	  throttle->data[msgLen-1] = checkSum(1, throttle->data, msgLen -1);
	  sendUSBPacket(throttle->data, msgLen);

	  //TODO Remove once done
	  LightLED();

	  //TODO change to time based code
	  HAL_Delay(20);

  }

}


void _Error_Handler(char * file, int line)
{
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }

}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
