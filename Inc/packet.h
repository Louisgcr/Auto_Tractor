#ifndef __PACKET_H
#define __PACKET_H

#include "stdint.h"
#include <stdlib.h>
#include "stm32f4xx_hal.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"

#define msgOverhead 6
#define startByte 175
#define endByte 243
#define timeout 5

uint32_t tickstart;

/*@brief:  This structure allocates a pointer to the data  
 * */
typedef struct vcpPacket{

	uint8_t *data;

}vcpPacket;

/*@brief : This structure dynamically allocates memory based on the message length
 *         It does 2 malloc to allocate 1 to structure pointer and another to the 
 *         size of the message and returns a pointer to the data
 *@param : msgId = message Id 
 *@param : sz = Size of message excluding the message overhead
 *@return: usbMsgPtr = Pointer to the structure
 * */
extern vcpPacket *vcpMsgPtr(uint8_t msgId, uint8_t sz){

	uint8_t size = sz + msgOverhead;
	//First malloc to allocate pointer
	vcpPacket *usbMsgPtr = malloc(sizeof(vcpPacket));
	if (usbMsgPtr == NULL)
		return NULL;

	//Second malloc to allocate the message overhead plus the message length
	usbMsgPtr->data = malloc(size * sizeof(uint8_t));
	if (usbMsgPtr->data == NULL){
        free(usbMsgPtr);
        return NULL;
    }
    usbMsgPtr->data[0] = startByte;
    usbMsgPtr->data[1] = startByte;
    usbMsgPtr->data[2] = size;
    usbMsgPtr->data[3] = msgId;
    usbMsgPtr->data[size-2] = endByte;

	return usbMsgPtr;
};


/*@brief  : Function calculates check sum based of the fletcher8 algorithm
 *@param  : f8 = start value of flecter8, cannot be 0, needs to be same as CPU
 *@param  : *data = pointer to start of data
 *@param  : numBytes = number of bytes in packet, includes message overheads
 *@return : checksum
 * */
uint8_t checkSum(uint8_t f8, uint8_t *data, size_t numBytes)
{

	uint8_t* byteStream =  data;
	volatile unsigned long sum1, sum2;
	volatile size_t part;

	sum1 = f8 & 0xf;
	sum2 = (f8 >> 4) & 0xf;
	while (numBytes) {
		part = numBytes > 5803 ? 5803 : numBytes;
		numBytes -= part;
		do {
			sum2 += sum1 += *byteStream++;
		} while (--part);
		sum1 %= 15;
		sum2 %= 15;
	}
	return (sum2 << 4) + sum1;
}

/*@brief  : Function to send data over virtual comp port, keeps calling the CDC_Transmit_FS
 * 			until message is sent successfully or has timed out
 *@param  : *Buf = buffer that contains the data
 *@param  : Len = length of message to send over
 *@return : returns 0 if successful and 1 if timed out
 * */
uint8_t sendUSBPacket(uint8_t *Buf, uint16_t Len){

	tickstart = HAL_GetTick();
	while(CDC_Transmit_FS(Buf, Len) != USBD_OK ){
		if (HAL_GetTick() - tickstart > timeout){
			break;
			return 1;
		}
	}
	return 0;
}


#endif /* __PACKET_H */
