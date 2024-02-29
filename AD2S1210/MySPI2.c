#include "stm32f10x.h"                  // Device header

void MySPI_W_SS2(uint8_t bitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, (BitAction)bitValue);
}

void MySPIInit2(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 	// 使能GPIO时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);	// 使能SPI2时钟
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;	// SS 推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;	// SCK MOSI 复用推挽输出 外设控制的输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;		// DO MISO 上拉输入
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	SPI_InitTypeDef SPI_InitStruct;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;		// 主模式	
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	// 双线全双工	
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;	// 高位先行
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;// 时钟频率SCK = PCLK/分频系数
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;		// CPHA=0 模式2
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;			// CPOL=1
	SPI_InitStruct.SPI_CRCPolynomial = 0;			// CRC校验 随便填一个
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;			// 软件控制还是硬件控制
	SPI_Init(SPI2, &SPI_InitStruct);

	SPI_Cmd(SPI2, ENABLE);
	
	// 置默认电平
	MySPI_W_SS2(1);
}

// 3个时序基本单元 起始 终止 交换一个字节
void MySPIStart2()
{
	MySPI_W_SS2(0);
}

void MySPIStop2()
{
	MySPI_W_SS2(1);
}

// 交换字节 只用写主机的操作 从机不用管自动执行
// 模式0  非连续模式
uint8_t MySPIChangeByte2(uint8_t sendData)
{
	// 1. 等待发送寄存器为空
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == 0);
	// 2. 发送数据
	SPI_I2S_SendData(SPI2, sendData);
	// 3. 等待接收寄存器不为空
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == 0);
	// 4. 读出数据
	uint16_t data = SPI_I2S_ReceiveData(SPI2);

	return data;
}
	


