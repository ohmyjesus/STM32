#ifndef __MYSPI_H
#define __MYSPI_H


void MySPIInit(void);

// 3个时序基本单元 起始 终止 交换一个字节
void MySPIStart(void);
void MySPIStop(void);

void MySPI_W_SS(uint8_t bitValue);
void MySPI_W_SCK(uint8_t bitValue);
void MySPI_W_MOSI(uint8_t bitValue);
uint8_t MySPI_R_MISO(void);

void MySPIInit(void);

// 3个时序基本单元 起始 终止 交换一个字节
void MySPIStart();

void MySPIStop();

void MySPISendByte(uint8_t sendData);
uint8_t MySPIReceiveByte(void);

#endif
