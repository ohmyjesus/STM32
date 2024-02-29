#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "Delay.h"

#define NONSTBY GPIO_SetBits(GPIOB,GPIO_Pin_0)
#define RESET_H GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define RESET_L GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define Convert_H GPIO_SetBits(GPIOB,GPIO_Pin_15)
#define Convert_L GPIO_ResetBits(GPIOB,GPIO_Pin_15)
#define CS_H GPIO_SetBits(GPIOA,GPIO_Pin_1)
#define CS_L GPIO_ResetBits(GPIOA,GPIO_Pin_1)

void AD7606Init(void)
{
	MySPIInit();
	
	// 复位
	RESET_H;
	Delay_us(1);
	RESET_L;
	// 配置AD7606
	// SER和OSI2直接接在了高电平，所以这里不再配置
	NONSTBY;	// 不待机
	Convert_H;	// 空闲高电平

}

uint8_t AD7606IsBusy(void)
{
	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13);
}

// 读通道CH1~CH8
// 转换之后读取数据
void AD7606ReadCH(int16_t *array)
{
	uint8_t i;
	Convert_L;
	Delay(0x1f);
	Convert_H;	// 上升沿开始转换
	Delay_us(66);	
	while (AD7606IsBusy() == 1); // 等待转换完成
	MySPIStart();
	for (i = 0; i < 8; i++)
	{
		array[i] = MySPIChangeByte(); // 
	}
	MySPIStop();
	return;
}

