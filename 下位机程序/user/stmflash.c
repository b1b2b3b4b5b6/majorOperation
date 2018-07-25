#include "stmflash.h"

 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK miniSTM32������
//STM32 FLASH ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/13
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����.
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}
#if STM32_FLASH_WREN	//���ʹ����д   
//������д��
//address:��ʼ��ַ
//buf:����ָ��
//bytes:����(16λ)��   
void STMFLASH_Write_NoCheck(u32 address,u16 *buf,u16 bytes)   
{ 			 		 
	u16 i;
	for(i=0;i<bytes;i++)
	{
		FLASH_ProgramHalfWord(address,buf[i]);
	    address+=2;//��ַ����2.
	}  
} 
//��ָ����ַ��ʼд��ָ�����ȵ�����
//address:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//buf:����ָ��
//bytes:����(16λ)��(����Ҫд���16λ���ݵĸ���.)

u8 STMFLASH_Write(u32 address,u16 *buf,u16 bytes)	
{
	#if STM32_FLASH_SIZE<256
	#define STM_SECTOR_SIZE 1024 //�ֽ�
	#else
	#define STM_SECTOR_SIZE	2048
	#endif
	u16 temp_buf[STM_SECTOR_SIZE/2];
	u32 offset;
	u8 sector_first;	   //��һ������
	u8 sector_last;	   //���һ������	

	u16 sector_offset;	   //������ƫ�Ƶ�ַ
	u16 sector_remian;	   //������ƫ�Ƶ�ַ
	u16 sector_last_offset;
	u32 i;
	
	if(address<STM32_FLASH_BASE||(address>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return 0;//�Ƿ���ַ
	offset=address-STM32_FLASH_BASE;
	sector_offset=(offset%STM_SECTOR_SIZE)/2;	
	sector_remian=(STM_SECTOR_SIZE-sector_offset)/2;
	sector_last_offset=(bytes-sector_remian)%(STM_SECTOR_SIZE/2);

	sector_first=offset/STM_SECTOR_SIZE;			
	sector_last=(offset+bytes*2)/STM_SECTOR_SIZE;

	FLASH_Unlock();//����	
	if (bytes<=sector_remian)
	{
		STMFLASH_Read(sector_first*STM_SECTOR_SIZE+STM32_FLASH_BASE,temp_buf,STM_SECTOR_SIZE/2);
		for(i=0;i<bytes;i++) temp_buf[i+sector_offset]=buf[i];
		FLASH_ErasePage(sector_first*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
		STMFLASH_Write_NoCheck(sector_first*STM_SECTOR_SIZE+STM32_FLASH_BASE,temp_buf,STM_SECTOR_SIZE/2);//д����������
	}
	
	if (bytes>sector_remian)
	{
		STMFLASH_Read(sector_first*STM_SECTOR_SIZE+STM32_FLASH_BASE,temp_buf,STM_SECTOR_SIZE/2);
		for(i=0;i<sector_remian;i++) temp_buf[i+sector_offset]=buf[i];
		FLASH_ErasePage(sector_first*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
		STMFLASH_Write_NoCheck(sector_first*STM_SECTOR_SIZE+STM32_FLASH_BASE,temp_buf,STM_SECTOR_SIZE/2);
		
		for (sector_first++;sector_first<sector_last;sector_first++)
		{
			FLASH_ErasePage(sector_first*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
			STMFLASH_Write_NoCheck(sector_first*STM_SECTOR_SIZE+STM32_FLASH_BASE,buf+sector_remian,STM_SECTOR_SIZE/2);	
		}
		if (sector_last_offset!=0)
		{
			STMFLASH_Read(sector_last*STM_SECTOR_SIZE+STM32_FLASH_BASE,temp_buf,STM_SECTOR_SIZE/2);
			for(i=0;i<sector_last_offset;i++)	 temp_buf[i]=buf[bytes-sector_last_offset+i];
			FLASH_ErasePage(sector_last*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
			STMFLASH_Write_NoCheck(sector_first*STM_SECTOR_SIZE+STM32_FLASH_BASE,temp_buf,STM_SECTOR_SIZE/2);
		}
	}
	FLASH_Lock();//����
	return 1;
}
#endif

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//buf:����ָ��
//bytes:����(16λ)��
void STMFLASH_Read(u32 ReadAddr,u16 *buf,u16 bytes)   	
{
	u16 i;
	for(i=0;i<bytes;i++)
	{
		buf[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
















