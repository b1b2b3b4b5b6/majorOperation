#include "key.h"
#define TEST(key) do{\
						if(R_##key == 0)\
						{\
							while(R_##key == 0);\
							key_value = key;\
						}\
					}while(0)
u8 key_value;
u8 get_key(void)
{
	u8 temp;
	temp = key_value;
	if(temp != ERR)
		key_value = ERR;
	return temp;
}
void exit_init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI3_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;// 抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =15 ;// 子优先级位15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据上面指定的参数初始化NVIC寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI4_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;// 抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =15 ;// 子优先级位15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据上面指定的参数初始化NVIC寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI9_5_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;// 抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =15 ;// 子优先级位15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据上面指定的参数初始化NVIC寄存器

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource3);
	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);
	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
}
void key_init(void)
{
	gpio_init_pin(KEY_OK,GPIO_Mode_IPU);
	gpio_init_pin(KEY_LEFT,GPIO_Mode_IPU);	
	gpio_init_pin(KEY_RIGHT,GPIO_Mode_IPU);
	exit_init();	
}


void EXTI3_IRQHandler(void)
{	
	SHAKE_DELAY;
	TEST(LEFT);
	EXTI_ClearITPendingBit(EXTI_Line3);

}

void EXTI4_IRQHandler(void)
{	
	SHAKE_DELAY;
	TEST(OK);
	EXTI_ClearITPendingBit(EXTI_Line4);
}

void EXTI9_5_IRQHandler(void)
{	
	SHAKE_DELAY;
	TEST(RIGHT);
	EXTI_ClearITPendingBit(EXTI_Line5);
}
