#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

void SerialInit(void);
void SerialSendPacket(uint8_t *array, uint16_t length);
uint8_t SerialGetRxFlag(void);
void SerialSendByte(uint8_t byte);
void SerialSendArray(uint8_t *array, uint16_t length);
void SerialSendString(char *string);
void SerialSendNum(uint32_t number, uint8_t length);

#endif
