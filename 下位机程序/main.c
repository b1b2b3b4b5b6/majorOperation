#include "stmflash.h"
#include "display.h"
#include "adc.h"
#include "key.h"
#include "power.h"
#include "pcap01.h"
#include "usart.h"
#include "cmd.h"
u16 temp_max;
u16 tpm_max;
u16 time= 6666;

void rtc_init(void);
void home(void);
void status(void);
void sleep(void);
void set_temperature(void);
void set_tpm(void);
void load_max(void);
void save_max(void);
float fixed = 56;

void temp_c(float value)
{
	st7565r_clear_area(1,8,1,128);
	char str[10];
	sprintf(str, "%.4f", value);
	show_num_string(5, 2*12,str, 12);	
}
int main(void)
{
	double value;
	u32 res = 0;
	u32 status = 0;
	u32 c0 = 0;
	u32 c1 = 0;
	clock_init();
	uart_init(115200);
	power_init();
	TURN_OFF(LCON);
	st7565r_init();
	delay_ms(20);
	st7565r_clear_screen();


	printf("ok\r\n");
	if(pcap_deinit() != 0)
		printf("Pcap01 init fail！\r\n");
	


	while(1)
	{
		pcap_write_cmd(0x8c);
		delay_ms(1000);
		status = pcap_read_res(CAP_STATUS);	
		c0 = pcap_read_res(CAP_C0);
		c1 = pcap_read_res(CAP_C1);
		value = c1;
		value = value/2097152*10;
		temp_c(value);
		cmd_pcap(value);
		cmd_scan();
	}
	
	
	//TURN_ON(LCON);
	//TURN_OFF(LEDCON);
	//TURN_ON(PCON);
	//delay_ms(20);
	//st7565r_init();
	//st7565r_clear_screen();
	//show_temperature(temperature);
	//load_max();
	//adc_init();
	//key_init();
	//rtc_init();
	//tpm_init();
	//CHARGE();
	home();
	

}
void change()
{

}

u16 array[50];
float value;
void home(void)
{
	int n;
	while(1)
	{
		
 		status();
		//show_temperature(temperature);

		//sleep();
		u8 key = get_key();
		switch(key)
		{
			case OK:
			{
				value = 0;
				for(n=0; n<800; n++)
				{
					//value += (u16)tpm_measure();
					delay_ms(2);
				}
				
				value/=800;
				show_tpm((u16)value);
			}
			break;
			
			case LEFT:
				//set_temperature();
			{	
			}
			break;
			
			case RIGHT:
			{
		
			}

				//set_tpm();
			break;
		
			default:
			break;
		}
		
	}
}

void show_alert(u8 page)
{
	 u8 temp[] = 
	{
		0x00,0x00,0x00,0x00,0x04,0x38,0x30,0x00,0x00,0x00,0x00,0xFC,0x38,0x08,0x08,0x08,0x08,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x02,0x02,0x02,0x02,0xFE,0x02,0x08,0x08,0x04,0x0B,0x19,0x68,0x88,0x08,0x08,0x88,0xE9,0x3F,0x0B,0x03,0x03,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x1F,0x18,0x44,0x42,0x41,0x20,0x20,0x10,0x19,0x0E,0x0E,0x0B,0x10,0x30,0x30,0x60,0x20,0x00,0x00,
		0x00,0x00,0x00,0x80,0xF0,0x40,0x40,0x40,0x40,0x40,0x44,0x4C,0x58,0x40,0x40,0x40,0x40,0x40,0x40,0xC0,0x60,0x40,0x00,0x00,
		0x00,0x00,0x01,0x05,0x04,0x04,0xE4,0x24,0x04,0x04,0x04,0xFC,0xFC,0x84,0x84,0x84,0x84,0x84,0xC7,0x86,0x04,0x00,0x00,0x00,
		0x00,0x00,0x40,0x20,0x18,0x07,0x01,0x06,0x08,0x18,0x10,0x3F,0x3F,0x20,0x20,0x60,0x60,0x60,0x60,0x60,0x60,0x20,0x00,0x00,
		0x00,0x00,0x10,0xD0,0xF0,0x7E,0x5C,0x50,0xD0,0x5C,0x50,0xF0,0x48,0xD0,0x30,0xFC,0x14,0x10,0xF0,0x30,0x10,0x10,0x00,0x00,
		0x00,0x00,0x01,0x40,0x47,0xC3,0xC3,0xC7,0xCB,0xD8,0xCC,0xD3,0xB0,0xD0,0xC8,0xC4,0xC3,0xC3,0x44,0x2C,0x28,0x08,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x78,0x7A,0x2A,0x2A,0x2A,0x2A,0x2A,0x2A,0x2A,0x2A,0x2A,0x7E,0x7B,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x80,0x80,0x80,0x80,0xFC,0xFC,0x80,0xC0,0x80,0x00,0xFC,0x08,0x08,0x08,0x08,0x08,0x08,0xFC,0x08,0x00,0x00,0x00,0x00,
		0x00,0x40,0x60,0x20,0x20,0xFF,0xFF,0x10,0x08,0x00,0x01,0xFF,0x08,0x18,0x68,0x89,0x09,0x8B,0xE9,0x1C,0x08,0x00,0x00,0x00,
		0x00,0x00,0x20,0x20,0x60,0x7F,0x3F,0x00,0x00,0x00,0x00,0x7F,0x20,0x10,0x08,0x04,0x03,0x07,0x0C,0x18,0x10,0x10,0x10,0x00,

	};
	u8 *buf = temp;
	u8 column =16;
	for(u8 n = 0; n <4; n++)
	{
		st7565r_show_24x24(page,column,buf);
		column += 24;
		buf += 24*3;		

	}
}
void set_temperature(void)
{
	u8 key = ERR;
	st7565r_clear_screen();
	status();
	show_temperature(temp_max);
	show_alert(3);
	while(key != OK)
	{
		sleep();
		key = get_key();
		switch(key)
		{		
			case RIGHT:
				temp_max += 1;
			break;
			
			case LEFT:
				temp_max -= 1;
			break;	
		
			default:
			break;
		}		
		show_temperature(temp_max);
		if(key == OK)	save_max();
	}
}

void set_tpm(void)
{
	u8 key = ERR;
	st7565r_clear_screen();
	status();
	show_tpm(tpm_max);
	show_alert(6);
	while(key != OK)
	{
		sleep();
		key = get_key();
		switch(key)
		{		
			case RIGHT:
				tpm_max += 1;
			break;
			
			case LEFT:
				tpm_max -= 1;
			break;	
		
			default:
			break;
		}		
		show_tpm(tpm_max);
		if(key == OK)	save_max();
	}
}
void load_max(void)
{
	u16 buf[2];

	STMFLASH_Read(0x08000000+0x10000,buf,2);//64KB外第一个字	
	temp_max = buf[0];
	tpm_max = buf[1];
	
	if(temp_max > 350) temp_max = 250;
	if(tpm_max > 100) tpm_max = 30;
	save_max();
}
void save_max(void)
{
	u16 buf[2];
	buf[0] = temp_max;
	buf[1] = tpm_max;
	STMFLASH_Write(0x08000000+0x10000,buf,2);//64KB外第一个字
}
void sleep(void)
{
	/*
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1, DISABLE );	  //关闭ADC1通道时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,DISABLE);	 //关闭GPIOA时钟
	__WFI();	//进入睡眠模式
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1, ENABLE );	  //关闭ADC1通道时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	 //开启GPIOA
	*/
}

void status(void)
{
	u8 a,b;
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1, ENABLE );	  //关闭ADC1通道时钟
	adc_sample();
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1, DISABLE );	  //关闭ADC1通道时钟
	(temperature >= temp_max)?(a=1):(a=0);
	//(tpm >= tpm_max)?(b=1):(b=0);
	show_status(power,a,b);
}
void RTC_Alarm_EXIT(void)
{ 
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel=RTCAlarm_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;// 抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =15 ;// 子优先级位15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据上面指定的参数初始化NVIC寄存器
	
	EXTI_ClearITPendingBit(EXTI_Line17);
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

void rtc_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);  
    PWR_BackupAccessCmd(ENABLE);  
    BKP_DeInit();  
	RCC_LSICmd(ENABLE);
	while(RCC_GetITStatus(RCC_IT_LSIRDY)!=RESET);
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	RCC_RTCCLKCmd(ENABLE); 
	 RTC_WaitForLastTask();	
	
	RTC_ITConfig(RTC_IT_ALR,ENABLE);
	RTC_WaitForLastTask();  
	
	RTC_SetPrescaler(40000);
	RTC_WaitForLastTask(); 
	
	RTC_SetCounter(0);
	RTC_WaitForLastTask(); 
	RTC_SetAlarm(2);
	RTC_WaitForLastTask(); 
	RTC_Alarm_EXIT();
	
}

void RTCAlarm_IRQHandler(void)
{ 
	if(RTC_GetITStatus(RTC_IT_ALR) != RESET)
	{
		RTC_SetCounter(0);
	}
	EXTI_ClearITPendingBit(EXTI_Line17);
	
	RTC_ClearITPendingBit(RTC_IT_ALR);
	RTC_WaitForLastTask();
 }


