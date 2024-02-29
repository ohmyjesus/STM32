#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "Delay.h"

// 开启GPIO和USART的时钟
// GPIO初始化 TX配置成复用输出 RX配置成输入
// 配置USART
// 开启输入中断 ITconfig NVIC
// 开启USART

void SerialInit(void)
{
	// 开启GPIO和USART的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// 这里用的USART3 对应的RX和TX就是PB10/PB11
	
	// GPIO初始化 TX配置成复用推挽输出 RX配置成输入
	GPIO_InitTypeDef GPIO_InitStruct1;
	GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct1);

	GPIO_InitTypeDef GPIO_InitStruct2;
	GPIO_InitStruct2.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct2.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStruct2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct2);
	
	// 配置USART
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 硬件流控制
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;		// 发送还是接收模式
	USART_InitStruct.USART_Parity = USART_Parity_No; 	// 校验位
	USART_InitStruct.USART_StopBits = USART_StopBits_1; // 停止位 0.5/1/1.5/2
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;	// 字长 无校验位8位
	USART_Init(USART3, &USART_InitStruct);
	
	// 开启输入中断 ITconfig NVIC
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	// 配置NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn; 		// 选择中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; // 抢占优先级 0优先级最高
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1; 	// 响应优先级
	NVIC_Init(&NVIC_InitStruct);
	
	USART_Cmd(USART3, ENABLE);
}


// 发送单个字节
void SerialSendByte(uint8_t byte)
{
	USART_SendData(USART3, byte);
	// 等待发送移位寄存器完成 且不需要手动清除标志位
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}

// 发送一个数组
void SerialSendArray(uint8_t *array, uint16_t length)
{
	uint16_t i = 0;
	for (i = 0; i < length; i++)
	{
		SerialSendByte(array[i]);
	}
}

// 发送字符串
void SerialSendString(char *string)
{
	uint16_t i = 0;
	for (i = 0; string[i] != 0; i++)
	{
		SerialSendByte(string[i]);
	}
}

uint32_t SerialPow(uint32_t x, uint32_t y)
{
	uint32_t res = 1;
	while (y--)
	{
		res = res * x;
	}
	return res;
}

// 发送一个数字 以字符串的形式显示数字
void SerialSendNum(uint32_t number, uint8_t length)
{
	uint32_t i;
	for (i = 0; i < length; i++)
	{
		uint32_t tmp;
		tmp = number / SerialPow(10, length-i-1);
		tmp = tmp % 10;
		SerialSendByte(tmp + '0'); // 转换为数字对应的ASCII码
	}
}

// 因为printf默认是打印到屏幕 而单片机没有屏幕 所以需要对printf进行重定向到串口
int fputc(int ch, FILE *f)
{
	SerialSendByte(ch);
	return ch;
}


// 发送数据包
void SerialSendPacket(uint8_t *array, uint16_t length)
{
	SerialSendByte(0xFF); // 包头
	SerialSendArray(array, length);
	SerialSendByte(0xFE); // 包尾
}
