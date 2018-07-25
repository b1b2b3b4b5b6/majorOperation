#include "usart.h"	
#include "cmd.h"
void uart_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 
	//USART2_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART2_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	
	USART_Init(USART1, &USART_InitStructure); 
	USART_Cmd(USART1, ENABLE);       
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_InitTypeDef   NVIC_InitStructure;	
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;// ��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =14 ;// �����ȼ�λ15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//��������ָ���Ĳ�����ʼ��NVIC�Ĵ���
}

#define PUTCHAR_PROTOTYPE int putchar (int c)
PUTCHAR_PROTOTYPE
{
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) c;      
	return c;
}

void usart_send_buf(u8 *data, u16 len)
{
	for(;len > 0; len--)
	{
		while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
		USART1->DR = *data;	
		data+=1;
	}
  
}

void usart_send_byte(u8 byte)
{
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = byte;	
}



void USART1_IRQHandler()
{	
	cmd_hander(USART1->DR);
}
