#ifndef __AD7606_H
#define __AD7606_H


void AD7606Init(void);

uint8_t AD7606IsBusy(void);
void AD7606ReadCH(int16_t *array);

#endif
