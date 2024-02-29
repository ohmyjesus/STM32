#ifndef __MYSPI2_H
#define __MYSPI2_H


void MySPIInit2(void);

// 3个时序基本单元 起始 终止 交换一个字节
void MySPIStart2(void);
void MySPIStop2(void);
uint8_t MySPIChangeByte2(uint8_t sendData);

#endif
