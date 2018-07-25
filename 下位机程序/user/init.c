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
	RCC_DeInit();		//ʱ�ӳ�ʼ��
	RCC_HSICmd(ENABLE);//ʹ��HSI    
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);//�ȴ�HSIʹ�ܳɹ�  
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_8);		
	RCC_PLLCmd(ENABLE);		//��PLL
	while(RCC_GetITStatus(RCC_IT_PLLRDY)!=RESET);		//�ȴ�PLL����
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);		//ѡ��PLLΪϵͳʱ��
	while(RCC_GetSYSCLKSource() != 0x08);
	RCC_HCLKConfig(RCC_SYSCLK_Div1);				//AHB����Ƶ
	RCC_PCLK1Config(RCC_HCLK_Div2);		
	RCC_PCLK2Config(RCC_HCLK_Div1);	
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);	//�ȴ�ϵͳʱ���л���PLL
	
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);  
	systick_clk = RCC_Clocks.SYSCLK_Frequency/1000000;
	
	delay_ms(4000);//��ֹ���ض˿�ռ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//�����˿���ӳ��
	GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);		 //�޷�ʹ��|һ��������
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		 //����������ã�ԭ����ֲ�P124

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	 //����GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	 //����GPIOBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);	 //����GPIOCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//����USART1
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	EXTI_DeInit();
}

void delay_us(u32 nus)
{
	u32 temp;
	SysTick->LOAD=nus*systick_clk; 					//ʱ�����
	SysTick->VAL=0x00;        					//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0X00;      					 //��ռ�����
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