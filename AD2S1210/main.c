#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD2S1210.h"
#include "AD2S1210_Ins.h"
#include "Serial.h"
#include "MySPI.h"

#define SMAPLE_H GPIO_SetBits(GPIOA,GPIO_Pin_5)
#define SMAPLE_L GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define A0_HIGH GPIO_SetBits(GPIOB,GPIO_Pin_3)	// A0
#define A0_LOW GPIO_ResetBits(GPIOB,GPIO_Pin_3)	// A0_low
#define A1_HIGH GPIO_SetBits(GPIOB,GPIO_Pin_4)	// A1
#define A1_LOW GPIO_ResetBits(GPIOB,GPIO_Pin_4)	// A1_low

uint8_t reg[15]; 	// 发送数据包数组

//	Delay(0x01);	// 0.51us
//	Delay(0x0f);	// 2.84us
//	Delay(0x3f);	// 10.84us
//uint32_t ns_delay = 0xffffffff;
//ns_delay >>= 1;	// 100ns

int main(void)
{
	SerialInit();
	AD2S1210Init();
	AD2S1210SoftReset();				// 软件复位
//	Clear_FAULT();
	
	AD2S1210WriteReg(0x88, 0x01);		// LOS阈值
	AD2S1210WriteReg(0x89, 0x7F);		// DOS超量程阈值
	AD2S1210WriteReg(0x8A, 0x7F);		// DOS失配阈值
	AD2S1210WriteReg(0x8B, 0x7F);		// DOS复位最大阈值
	AD2S1210WriteReg(0x8C, 0x01);		// DOS复位最小阈值
	AD2S1210WriteReg(0x8D, 0x7F);		// LOT上限
	AD2S1210WriteReg(0x8E, 0x01);		// LOT下限	
	AD2S1210WriteReg(0x91, 0x3C);		// 激励频率 15KHz
	AD2S1210WriteReg(0x92, 0x70);		// 控制 编码器和数字输出分辨率均为10位  忽略后六位数据
	Delay(0xff);
	
	reg[0] = AD2S1210ReadReg(0x88);		//LOS阀值
	reg[1] = AD2S1210ReadReg(0x89);		//DOS超量程阀值
	reg[2] = AD2S1210ReadReg(0x8A);		//DOS失配阀值
	reg[3] = AD2S1210ReadReg(0x8B);		//DOS复位最大阀值
	reg[4] = AD2S1210ReadReg(0x8C);		//DOS复位最小阀值
	reg[5] = AD2S1210ReadReg(0x8D);		//LOT上限
	reg[6] = AD2S1210ReadReg(0x8E);		//LOT下限
	reg[7] = AD2S1210ReadReg(0x91);		//激励频率
	reg[8] = AD2S1210ReadReg(0x92);		//控制寄存器
	reg[9] = AD2S1210ReadReg(0xff);		//Fault 
	
	Delay_ms(1);
	A0_HIGH;
	A0_HIGH;
	SerialSendArray(reg, 14);
	
	while(1){
		SMAPLE_H;
		SMAPLE_L;
		
		reg[10] = AD2S1210ReadReg(0xff);//Fault
		
		// 读取位置和速度寄存器 10位分辨率 低6位忽略
		reg[11] = AD2S1210ReadReg(AD2S1210_PosHigh);
		reg[12] = AD2S1210ReadReg(AD2S1210_PosLow);
		reg[12] &= 0xC0;
		
		reg[13] = AD2S1210ReadReg(AD2S1210_VelHigh);
		reg[14] = AD2S1210ReadReg(AD2S1210_VelLow);
		reg[14] &= 0xC0;
		
		SerialSendArray(reg, 15);
		
		Delay_ms(300);
		
	}
}

