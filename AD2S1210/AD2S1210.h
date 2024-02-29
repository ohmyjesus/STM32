#ifndef __AD2S1210_H
#define __AD2S1210_H


void AD2S1210Init(void);
void AD2S1210WriteReg(uint8_t regAddress, uint8_t data);
uint8_t AD2S1210ReadReg(uint8_t regAddress);
void AD2S1210SoftReset(void);
void Clear_FAULT(void);

#endif
