/*
 * init.c
 *
 * Created: 2015/10/6 17:07:32
 *  Author: God
 */ 

#include "init.h"
u8 systick_clk;
void clock_init(void)
{
	RCC_DeInit();		//时钟初始化
	RCC_HSICmd(ENABLE);//使能HSI    
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);//等待HSI使能成功  
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_8);		
	RCC_PLLCmd(ENABLE);		//开PLL
	while(RCC_GetITStatus(RCC_IT_PLLRDY)!=RESET);		//等待PLL就绪
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);		//选择PLL为系统时钟
	while(RCC_GetSYSCLKSource() != 0x08);
	RCC_HCLKConfig(RCC_SYSCLK_Div1);				//AHB不分频
	RCC_PCLK1Config(RCC_HCLK_Div2);		
	RCC_PCLK2Config(RCC_HCLK_Div1);	
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);	//等待系统时钟切换至PLL
	
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);  
	systick_clk = RCC_Clocks.SYSCLK_Frequency/1000000;
	
	delay_ms(4000);//防止下载端口占用
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//开启端口重映射
	GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);		 //无法使用|一次性设置
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		 //必须最后设置，原因见手册P124

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	 //开启GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	 //开启GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);	 //开启GPIOC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//开启USART1
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	EXTI_DeInit();
}

void delay_us(u32 nus)
{
	u32 temp;
	SysTick->LOAD=nus*systick_clk; 					//时间加载
	SysTick->VAL=0x00;        					//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL =0X00;      					 //清空计数器
}
void delay_ms(u32 nms)
{
	for(;nms>0;nms--)
	{
		delay_us(1000);
	}
}
void gpio_init_pin(GPIO_TypeDef* GPIOx, u16 GPIO_Pin,GPIOMode_TypeDef MODE)
{
	GPIO_InitTypeDef io;
	io.GPIO_Mode = MODE;
	io.GPIO_Pin=GPIO_Pin;
	io.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOx,&io);
}



void assert_failed(u8* file, u32 line)
{ 
	printf("Wrong parameters value: file %s on line %d\r\n", file, line) ;
	while (1);
}