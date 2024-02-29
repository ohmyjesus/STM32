#include "stm32f10x.h"                  // Device header
#include "Delay.h"

// 因为这里SS线未接，所以默认一直低电平，且AD转换器无写数据，所以无MOSI
void MySPI_W_SS(uint8_t bitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_1, (BitAction)bitValue);
}

//void MySPI_W_MOSI(uint8_t bitValue)
//{
//	GPIO_WriteBit(GPIOA, GPIO_Pin_7, (BitAction)bitValue);
//}


void MySPI_W_SCK(uint8_t bitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_14, (BitAction)bitValue);
}


uint8_t MySPI_R_MISO(void)
{
	uint8_t data = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12);
	return data;
}

void MySPIInit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能时钟
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	// SCK CONVERTA&B STBY RESET 推挽输出
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_0 | GPIO_Pin_9;	
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;		// MISO	上拉输入
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;		// BUSY	上拉输入
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;			// SS 推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// 置默认电平
	MySPI_W_SS(1);
	MySPI_W_SCK(1); // 
}

// 3个时序基本单元 起始 终止 交换一个字节
void MySPIStart()
{
	MySPI_W_SS(0);
}

void MySPIStop()
{
	MySPI_W_SS(1);
}

// 交换字节 只用写主机的操作 从机不用管自动执行
// 模式2 上升沿移出数据 下降沿移入数据 SCK空闲高电平
uint16_t MySPIChangeByte(void)
{
	uint16_t i, data = 0x0000;
	for (i = 0; i < 16; i++)
	{
		MySPI_W_SCK(0);		// 
		if (MySPI_R_MISO() == 1)
		{
			data |= 0x01;
		}
		data <<= 1;		// 左移1位 低位自动补0 高位先行
		MySPI_W_SCK(1); 	
		Delay(0x0f);
	}
	return data;
}
	
// 模式0 上升沿移入数据 下降沿移出数据 SCK空闲低电平
//uint8_t MySPIChangeByte(uint8_t sendData)
//{
//	uint8_t i;
//	for (i = 0; i < 8; i++)
//	{
//		MySPI_W_MOSI(sendData & 0x80);
//		sendData <<= 1;		// 左移1位 低位自动补0
//		MySPI_W_SCK(1);		// 上升沿移入数据
//		if (MySPI_R_MISO() == 1)
//		{
//			sendData |= 0x01;
//		}
//		MySPI_W_SCK(0); 	// 下降沿移出数据
//	}
//		
//	return sendData;
//}
	
//// 模式1 上升沿移出数据 下降沿移入数据 SCK空闲低电平
//uint8_t MySPIChangeByte(uint8_t sendData)
//{
//	uint8_t i;
//	for (i = 0; i < 8; i++)
//	{
//		MySPI_W_SCK(1);		// 上升沿移出数据
//		MySPI_W_MOSI(sendData & 0x80);
//		sendData <<= 1;		// 左移1位 低位自动补0
//		MySPI_W_SCK(0); 	// 下降沿移入数据
//		if (MySPI_R_MISO() == 1)
//		{
//			sendData |= 0x01;
//		}
//	}
//	return sendData;
//}

// 模式2 上升沿移出数据 下降沿移入数据 SCK空闲高电平
//uint8_t MySPIChangeByte(uint8_t sendData)
//{
//	uint8_t i;
//	for (i = 0; i < 8; i++)
//	{
//		MySPI_W_MOSI(sendData & 0x80);
//		sendData <<= 1;		// 左移1位 低位自动补0
//		MySPI_W_SCK(0);		// 
//		if (MySPI_R_MISO() == 1)
//		{
//			sendData |= 0x01;
//		}
//		MySPI_W_SCK(1); 	
//	}
//	return sendData;
//}

// 模式3 上升沿移入数据 下降沿移出数据  SCK空闲高电平
//uint8_t MySPIChangeByte(uint8_t sendData)
//{
//	uint8_t i;
//	for (i = 0; i < 8; i++)
//	{
//		MySPI_W_SCK(0);		// 
//		MySPI_W_MOSI(sendData & 0x80);
//		sendData <<= 1;		// 
//		MySPI_W_SCK(1); 	// 
//		if (MySPI_R_MISO() == 1)
//		{
//			sendData |= 0x01;
//		}
//	}
//	return sendData;
//}
	


