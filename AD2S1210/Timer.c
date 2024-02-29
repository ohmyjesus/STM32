#include "stm32f10x.h"                  // Device header
#include <stdbool.h>

// 使能内部时钟->选择内部或外部时钟模式->配置时基单元->使能中断输出控制->NVIC->使能计数器控制

extern uint16_t num;
bool TIM3_STATE;
u16 Comm_times=0;

void TimerInit(u16 arr,u16 psc)
{
	
	// 1.使能内部时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
	
	// 2.选择内部或外部时钟模式
	TIM_InternalClockConfig(TIM3);
	
	// 3.配置时基单元
	//定时器TIM3初始化
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = arr; 	// 自动重装值 范围0-65535
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 	// 预分频值 定时1s即72M/(PSC+1)/(ARR+1)=1HZ=1s
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 1分频即不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
	
	// 进入中断前清除中断标志位 避免刚初始化时就进入中断
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	
	// 4.使能中断输出控制
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	// 5.配置NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStruct);  //初始化NVIC寄存器
	
	// 6.使能计数器控制
	TIM_Cmd(TIM3, ENABLE);
	
}

// 现在是每0.025秒执行一次定时中断 操作数++
//void TIM3_IRQHandler(void)
//{
//	// 检查标志位
//	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
//	{
//		 TIM_ClearITPendingBit(TIM3, TIM_IT_Update);    //清除TIMx更新中断标志 
//		 Comm_times++;
//		 TIM3_STATE = true;
//		
//	}
//}




