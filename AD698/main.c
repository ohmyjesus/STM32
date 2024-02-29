#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD7606.h"
#include "Serial.h"
#include <stdio.h>
#include "stdlib.h"
#include <math.h>

int16_t CH[8]; 	// 8个通道
#define PI 3.1415

//	Delay(0x01);	// 0.51us
//	Delay(0x0f);	// 2.84us
//	Delay(0x3f);	// 10.84us
//uint32_t ns_delay = 0xffffffff;
//ns_delay >>= 1;	// 100ns

int main(void)
{
	AD7606Init();
	SerialInit();
	
	// 16位分辨率 8通道
	// 模拟输入电压范围±10V
	// 16倍过采样 OS[2:0] = 100 Tconv最小68us 
	// 只用一路DoutA/DB7输出
	// CONVERNTA&B 一起转换  转换完成后读取数据
	
	while(1){
		AD7606ReadCH(CH);
		uint8_t flag = 1;	// 为0表示十六进制 为1表示字符串模式显示实际电压值
		if (flag == 0)
		{
			// 引脚悬空也有电压值，[-10, 10]大约4V左右
			SerialSendByte(CH[0]>>8);		// 高8位
			SerialSendByte(CH[0]&0x0011);	// 低8位
			SerialSendByte(CH[1]>>8);
			SerialSendByte(CH[1]&0x0011);
			SerialSendByte(CH[2]>>8);
			SerialSendByte(CH[2]&0x0011);
			SerialSendByte(CH[3]>>8);
			SerialSendByte(CH[3]&0x0011);
			SerialSendByte(CH[4]>>8);
			SerialSendByte(CH[4]&0x0011);
			SerialSendByte(CH[5]>>8);
			SerialSendByte(CH[5]&0x0011);
			SerialSendByte(CH[6]>>8);
			SerialSendByte(CH[6]&0x0011);
			SerialSendByte(CH[7]>>8);
			SerialSendByte(CH[7]&0x0011);
		}
		else if (flag == 1)
		{
			// 只输出第一通道值
			float Vout = ((float)CH[0]);
			if (Vout > 32767.0)
			{
				Vout = (Vout - 65536.0) / 32768.0 * 10.0;	// 16位有符号数 负数求其补码 再转换成对应电压
			}
			else
			{
				Vout = Vout / 32768.0 * 10.0;
			}
			float R2 = 6.5;		// R2电阻 单位kΩ
			float denom = 0.50 * R2;
			float A_B = Vout / denom;			// A/B
			double alpha = atan((double)A_B) * 180.0 / PI;	// 弧度转角度
			SerialPrintf("Vout = %.2f V, CH1 = %.2f degree, A/B = sin/cos = %.2f \r\n" , Vout, alpha, A_B);
		}
		else if (flag == 2)
		{
			float data1 = ((float)CH[0]) / 32768.0 * 10.0;
			float data2 = ((float)CH[1]) / 32768.0 * 10.0;
			float data3 = ((float)CH[2]) / 32768.0 * 10.0;
			float data4 = ((float)CH[3]) / 32768.0 * 10.0;
			float data5 = ((float)CH[4]) / 32768.0 * 10.0;
			float data6 = ((float)CH[5]) / 32768.0 * 10.0;
			float data7 = ((float)CH[6]) / 32768.0 * 10.0;
			float data8 = ((float)CH[7]) / 32768.0 * 10.0;
			SerialPrintf("CH1=%3.2fV, CH2=%3.2fV, CH3=%3.2fV, CH4=%3.2fV, CH5=%3.2fV, CH6=%3.2fV, CH7=%3.2fV, CH8=%3.2fV\r\n",
							data1, data2, data3, data4, data5, data6, data7, data8);
		}
		Delay_ms(500);
	}
}
