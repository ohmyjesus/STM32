#include "stm32f10x.h"                  // Device header

void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能时钟
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_3); // 初始置为高电平 熄灭
}

void LED1_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_1); // 点亮led1
}

// 翻转功能 
void LED1_Turn(void)
{
	// 如果是点亮就熄灭 如果是熄灭就点亮
	if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1) == 0) 
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_1); // 熄灭led
	}else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_1); // 点亮led
	}
}

void LED1_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_1); // 熄灭led1
}

void LED2_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_3); // 点亮led2
}

void LED2_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_3); // 熄灭led2
}

void LED2_Turn(void)
{
	if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_3) == 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_3); 
	}else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_3); 
	}
}