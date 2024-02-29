#ifndef __MYSPI_H
#define __MYSPI_H


void MySPIInit(void);
void MySPIStart();

void MySPIStop();
void MySPI_W_SCK(uint8_t bitValue);

// 3个时序基本单元 起始 终止 交换一个字节
uint16_t MySPIChangeByte(void);

#endif
