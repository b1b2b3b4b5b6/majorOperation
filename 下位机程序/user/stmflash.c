#include "stmflash.h"

 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK miniSTM32开发板
//STM32 FLASH 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/13
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}
#if STM32_FLASH_WREN	//如果使能了写   
//不检查的写入
//address:起始地址
//buf:数据指针
//bytes:半字(16位)数   
void STMFLASH_Write_NoCheck(u32 address,u16 *buf,u16 bytes)   
{ 			 		 
	u16 i;
	for(i=0;i<bytes;i++)
	{
		FLASH_ProgramHalfWord(address,buf[i]);
	    address+=2;//地址增加2.
	}  
} 
//从指定地址开始写入指定长度的数据
//address:起始地址(此地址必须为2的倍数!!)
//buf:数据指针
//bytes:半字(16位)数(就是要写入的16位数据的个数.)

u8 STMFLASH_Write(u32 address,u16 *buf,u16 bytes)	
{
	#if STM32_FLASH_SIZE<256
	#define STM_SECTOR_SIZE 1024 //字节
	#else
	#define STM_SECTOR_SIZE	2048
	#endif
	u16 temp_buf[STM_SECTOR_SIZE/2];
	u32 offset;
	u8 sector_first;	   //第一个扇区
	u8 sector_last;	   //最后一个扇区	

	u16 sector_offset;	   //扇区内偏移地址
	u16 sector_remian;	   //扇区内偏移地址
	u16 sector_last_offset;
	u32 i;
	
	if(address<STM32_FLASH_BASE||(address>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return 0;//非法地址
	offset=address-STM32_FLASH_BASE;
	sector_offset=(offset%STM_SECTOR_SIZE)/2;	
	sector_remian=(STM_SECTOR_SIZE-sector_offset)/2;
	sector_last_offset=(bytes-sector_remian)%(STM_SECTOR_SIZE/2);

	sector_first=offset/STM_SECTOR_SIZE;			
	sector_last=(offset+bytes*2)/STM_SECTOR_SIZE;

	FLASH_Unlock();//上锁	
	if (bytes<=sector_remian)
	{
		STMFLASH_Read(sector_first*STM_SECTOR_SIZE+STM32_FLASH_BASE,temp_buf,STM_SECTOR_SIZE/2);
		for(i=0;i<bytes;i++) temp_buf[i+sector_offset]=buf[i];
		FLASH_ErasePage(sector_first*STM_SECTOR_SIZE+STM32_FLASH_BASE);//擦除这个扇区
		STMFLASH_Write_NoCheck(sector_first*STM_SECTOR_SIZE+STM32_FLASH_BASE,temp_buf,STM_SECTOR_SIZE/2);//写入整个扇区
	}
	
	if (bytes>sector_remian)
	{
		STMFLASH_Read(sector_first*STM_SECTOR_SIZE+STM32_FLASH_BASE,temp_buf,STM_SECTOR_SIZE/2);
		for(i=0;i<sector_remian;i++) temp_buf[i+sector_offset]=buf[i];
		FLASH_ErasePage(sector_first*STM_SECTOR_SIZE+STM32_FLASH_BASE);//擦除这个扇区
		STMFLASH_Write_NoCheck(sector_first*STM_SECTOR_SIZE+STM32_FLASH_BASE,temp_buf,STM_SECTOR_SIZE/2);
		
		for (sector_first++;sector_first<sector_last;sector_first++)
		{
			FLASH_ErasePage(sector_first*STM_SECTOR_SIZE+STM32_FLASH_BASE);//擦除这个扇区
			STMFLASH_Write_NoCheck(sector_first*STM_SECTOR_SIZE+STM32_FLASH_BASE,buf+sector_remian,STM_SECTOR_SIZE/2);	
		}
		if (sector_last_offset!=0)
		{
			STMFLASH_Read(sector_last*STM_SECTOR_SIZE+STM32_FLASH_BASE,temp_buf,STM_SECTOR_SIZE/2);
			for(i=0;i<sector_last_offset;i++)	 temp_buf[i]=buf[bytes-sector_last_offset+i];
			FLASH_ErasePage(sector_last*STM_SECTOR_SIZE+STM32_FLASH_BASE);//擦除这个扇区
			STMFLASH_Write_NoCheck(sector_first*STM_SECTOR_SIZE+STM32_FLASH_BASE,temp_buf,STM_SECTOR_SIZE/2);
		}
	}
	FLASH_Lock();//上锁
	return 1;
}
#endif

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//buf:数据指针
//bytes:半字(16位)数
void STMFLASH_Read(u32 ReadAddr,u16 *buf,u16 bytes)   	
{
	u16 i;
	for(i=0;i<bytes;i++)
	{
		buf[i]=STMFLASH_ReadHalfWord(ReadAddr);//读取2个字节.
		ReadAddr+=2;//偏移2个字节.	
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
















