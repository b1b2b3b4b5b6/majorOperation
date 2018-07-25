#include "tpm.h"

u16 pre=1;
u8 flag = FINISH;

void gpio_init(void)
{
	gpio_init_pin(FALLING,GPIO_Mode_IPU);
	gpio_init_pin(SWITCH,GPIO_Mode_Out_PP);
}
void tpm_init()
{
	gpio_init();
	time_init();
}

void time_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_ICInitTypeDef  TIM1_ICInitStructure;	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_Period = 0xffff;  
	TIM_TimeBaseInitStruct.TIM_Prescaler =pre - 1; 
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);
	
	TIM1_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
  	TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;	
  	TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  	TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  	TIM1_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
  	TIM_ICInit(TIM1, &TIM1_ICInitStructure);
	
	NVIC_InitTypeDef   NVIC_InitStructure;	
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_CC_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;// 抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =15 ;// 子优先级位15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据上面指定的参数初始化NVIC寄存器
	
	TIM_ITConfig(TIM1,TIM_IT_CC1,DISABLE);
	TIM_Cmd(TIM1,ENABLE ); 	//使能定时器1
	DISCHARGE();
}

void tpm_start(void)
{
	DISCHARGE();
	flag = BUSY;
	TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
	TIM_ITConfig(TIM1,TIM_IT_CC1,ENABLE);//允许更新中断 ,允许CC1IE捕获中断
	TIM_PrescalerConfig(TIM1, pre-1, TIM_PSCReloadMode_Immediate);
	TIM_GenerateEvent(TIM1,TIM_EventSource_Update);
	CHARGE();
}

void TIM1_CC_IRQHandler()
{
	TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
	flag = FINISH;
	DISCHARGE();
	
}
u8 tpm_flag()
{
	return flag;
}
float get_time(void)
{
	float value;
	value = TIM_GetCapture1(TIM1);
	return value;
}

float tpm_measure(void)
{
	tpm_start();
	while(tpm_flag() == BUSY){};
	TIM_ITConfig(TIM1,TIM_IT_CC1,DISABLE);
	return get_time();
}

void set_range(enum cap range)
{
	switch(range)
	{
	case pf:
		pre = 1;
	break;
	
	case nf:
		pre = 72;
	break;

	case uf:
		pre = 7200;
	break;
	}
}