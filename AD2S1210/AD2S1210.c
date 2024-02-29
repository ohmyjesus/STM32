#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "AD2S1210_Ins.h"
#include "Delay.h"

//#define A0_HIGH GPIO_SetBits(GPIOB,GPIO_Pin_3)	// A0
//#define A1_HIGH GPIO_SetBits(GPIOB,GPIO_Pin_4)	// A1
//#define SMAPLE_H GPIO_SetBits(GPIOA,GPIO_Pin_5)
//#define SMAPLE_L GPIO_ResetBits(GPIOA,GPIO_Pin_5)
//#define RESET_H GPIO_SetBits(GPIOA,GPIO_Pin_0)
//#define RESET_L GPIO_ResetBits(GPIOA,GPIO_Pin_0)// reset
#define CS_L GPIO_ResetBits(GPIOA,GPIO_Pin_1)		// cs
#define CS_H GPIO_SetBits(GPIOA,GPIO_Pin_1)		// cs

#define RESET_H GPIO_SetBits(GPIOA,GPIO_Pin_0)
#define RESET_L GPIO_ResetBits(GPIOA,GPIO_Pin_0)
#define PCS_H GPIO_SetBits(GPIOA,GPIO_Pin_1)
#define PCS_L GPIO_ResetBits(GPIOA,GPIO_Pin_1)

#define SCLK_H GPIO_SetBits(GPIOA,GPIO_Pin_2)
#define SCLK_L GPIO_ResetBits(GPIOA,GPIO_Pin_2)
#define SDI_H GPIO_SetBits(GPIOA,GPIO_Pin_3)
#define SDI_L GPIO_ResetBits(GPIOA,GPIO_Pin_3)
#define NCS_H GPIO_SetBits(GPIOA,GPIO_Pin_4)
#define NCS_L GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define SMAPLE_H GPIO_SetBits(GPIOA,GPIO_Pin_5)
#define SMAPLE_L GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define SDO GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)

void AD2S1210Init(void)
{
	CS_L; 
	MySPIInit();
	// 配置模式1 & 1
//	A0_HIGH;
//	A1_HIGH;
	// 串行接口不需要CS输入，保持为低电平
	
	/* SAMPLE信号发生高电平至低电平转换后，数据从位置和速度积分器传输到位置和速度寄存器，
	故障寄存器也会进行更新 */
	SMAPLE_L;
}

// 软复位
void AD2S1210SoftReset(void)
{
	// AD2S1210需要一个外部复位信号使RESET输入保持低电平，直到VDD达到规定的工作电压范围
	SMAPLE_H;
	SMAPLE_L;
	RESET_L;
	Delay(0xff);
	RESET_H;
	Delay(0xff);
}

// 写寄存器
void AD2S1210WriteReg(uint8_t regAddress, uint8_t data)
{
	MySPIStart();
	MySPISendByte(regAddress);
	MySPIStop();
	MySPIStart();
	MySPISendByte(data);
	MySPIStop();
}

// 读寄存器  发送地址与接收数据拆分开
uint8_t AD2S1210ReadReg(uint8_t regAddress)
{
	MySPIStart();
	MySPISendByte(regAddress);
	MySPIStop();
	MySPIStart();
	uint8_t data = MySPIReceiveByte();
	MySPIStop();
	return data;
}

void Clear_FAULT(void)
{
	//清除故障寄存器
	SMAPLE_H;
	Delay_ms(1);
	SMAPLE_L;
	Delay_ms(1);
	SMAPLE_H;
//	AD2S1210ReadReg(0xFF);
	SMAPLE_H;
	Delay_ms(1);
	SMAPLE_L;
}

//void AD2S1210_WRITE(uint8_t addr,uint8_t data)
//{
//  uint8_t temp;
//  SCLK_L;
//	Delay(0x3f);
//  NCS_L;
//  temp=addr;
//  for(uint8_t s=0;s<8;s++)
//  {
//    SCLK_H;
//		Delay(0x0f);
//    if(temp&0x80){SDI_H;}
//    else{SDI_L;}
//		Delay(0x0f);
//    SCLK_L;
//    temp<<=1;
//		Delay(0x0f);
//  }
//  NCS_H;
//  //SCLK_H;
//  Delay(0x0f);
//  SCLK_L;
//	Delay(0x3f);
//  NCS_L;
//  temp=data;
//  for(uint8_t s=0;s<8;s++)
//  {
//    SCLK_H;
//		Delay(0x0f);
//    if(temp&0x80){SDI_H;}
//    else{SDI_L;}
//		Delay(0x0f);
//    SCLK_L;
//    temp<<=1;
//		Delay(0x0f);
//  }
//  NCS_H;
//  SCLK_H;
//}


//uint8_t AD2S1210_READ(uint8_t addr)
//{
//  uint8_t temp;
//  SCLK_L;
//	Delay(0x3f);
//  NCS_L;
//  temp=addr;
//  for(uint8_t s=0;s<8;s++)
//  {
//    SCLK_H;
//		Delay(0x0f);
//    if(temp&0x80){SDI_H;}
//    else{SDI_L;}
//		Delay(0x0f);
//    SCLK_L;
//    temp<<=1;
//		Delay(0x0f);
//  }
//  NCS_H;
//  //SCLK_H;
//  Delay(0x0f);
//  SCLK_L;
//	Delay(0x3f);
//  NCS_L;
//  temp=0;
//  for(uint8_t s=0;s<8;s++)
//  { 
//    SCLK_H;
//		Delay(0x0f);
//		temp<<=1;
//    if(SDO){temp++;}
//		//Delay(0x0f);
//    SCLK_L;
//		Delay(0x0f);
//  }
//  NCS_H;
//  SCLK_H;
//  return temp;
//}
