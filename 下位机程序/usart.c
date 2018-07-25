#include "usart.h"	
#include "cmd.h"
void uart_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 
	//USART2_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART2_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOA.10  

  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	
	USART_Init(USART1, &USART_InitStructure); 
	USART_Cmd(USART1, ENABLE);       
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_InitTypeDef   NVIC_InitStructure;	
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;// 抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =14 ;// 子优先级位15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据上面指定的参数初始化NVIC寄存器
}

#define PUTCHAR_PROTOTYPE int putchar (int c)
PUTCHAR_PROTOTYPE
{
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) c;      
	return c;
}

void usart_send_buf(u8 *data, u16 len)
{
	for(;len > 0; len--)
	{
		while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
		USART1->DR = *data;	
		data+=1;
	}
  
}

void usart_send_byte(u8 byte)
{
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = byte;	
}



void USART1_IRQHandler()
{	
	cmd_hander(USART1->DR);
}
