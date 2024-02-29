#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void Delay(vu32 nCount) ;

void MySPI_W_SS(uint8_t bitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)bitValue);	// WR
}

void MySPI_W_SCK(uint8_t bitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_2, (BitAction)bitValue);	// SCLK
}

void MySPI_W_MOSI(uint8_t bitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_3, (BitAction)bitValue);
}

uint8_t MySPI_R_MISO(void)
{
	uint8_t data = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5);
	return data;
}

void MySPIInit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
 	    
	GPIO_InitTypeDef  GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		// DO MISO	上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3|GPIO_Pin_4;	// 推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// 置默认电平
	MySPI_W_SS(1);	
	MySPI_W_SCK(1); // 空闲时高电平
}

// 3个时序基本单元 起始 终止 交换一个字节
void MySPIStart()
{
	MySPI_W_SCK(0);
	MySPI_W_SS(0);
}

void MySPIStop()
{
	MySPI_W_SS(1);
	MySPI_W_SCK(1);
}


// 上升沿移出数据 下降沿移入数据  SCK空闲时高电平
// SPI只写入 不读取
// 模式2的变形
void MySPISendByte(uint8_t sendData)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		MySPI_W_SCK(1);
		MySPI_W_MOSI(sendData & 0x80);
		sendData <<= 1;		// 左移1位 低位自动补0
		MySPI_W_SCK(0);		// 
	}
	return;
}

// SPI只读取 不写入
uint8_t MySPIReceiveByte(void)
{
	uint8_t i, data = 0;
	for (i = 0; i < 8; i++)
	{
		MySPI_W_SCK(1);
		data <<= 1;
		MySPI_W_SCK(0);
		if (MySPI_R_MISO() == 1)
		{
			data |= 0x01;
		}
	}
	return data;
}

// 交换字节 只用写主机的操作 从机不用管自动执行
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
