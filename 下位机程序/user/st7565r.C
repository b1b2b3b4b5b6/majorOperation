#define ST7564R_HERE
#include "st7565r.h"

static void gpio_init(void)
{
	OUT(CS);
	OUT(RST);
	OUT(RS);
	OUT(SCK);
	OUT(SDA);
}
/*写指令到LCD模块*/
void st7565r_write_cmd(u8 data)   
{
	char i;
	RESET(RS);
	for(i=0;i<8;i++)
   {
		RESET(SCK);
		SCK_DELAY;
		if(data&0x80) SET(SDA);
		else RESET(SDA);
		SCK_DELAY;
		SET(SCK);
	 	data<<=1;
   }
}

/*写数据到LCD模块*/
void st7565r_write_data(u8 data)
{
	char i;
	SET(RS);
	for(i=0;i<8;i++)
   {
		RESET(SCK);
	   SCK_DELAY
		if(data&0x80) SET(SDA);
		else RESET(SDA);
	   SCK_DELAY
		SET(SCK);
	 	data<<=1;
    }
}

void st7565r_write_ndata(u8 *data,int n)
{
	for(int i=0; i<n; i++,data++)
		st7565r_write_data(*data);
}


/*LCD模块初始化*/
void st7565r_init()
{
	gpio_init();
	RESET(CS);
	RESET(RST);        /*低电平复位*/
	POWERUP_DELAY;
	SET(RST);		    /*复位完毕*/
	POWERUP_DELAY;        
	st7565r_write_cmd(0xe2);	 /*软复位*/
	POWERUP_DELAY;
	st7565r_write_cmd(0x2f);  /*升压步聚3*/
	POWERUP_DELAY;
	st7565r_write_cmd(0x23);  /*粗调对比度，可设置范围0x20～0x27*/
	st7565r_write_cmd(0x81);  /*微调对比度*/
	st7565r_write_cmd(0x2a);  /*0x28,微调对比度的值，可设置范围0x00～0x3f*/
	st7565r_write_cmd(0xa2);  /*1/9偏压比（bias）*/
	st7565r_write_cmd(0xc8);  /*行扫描顺序：从上到下*/
	st7565r_write_cmd(0xa0);  /*列扫描顺序：从左到右*/  
	st7565r_write_cmd(0xa6);
	st7565r_write_cmd(0xa4);
	st7565r_write_cmd(0x40);  /*起始行：第一行开始*/
	st7565r_write_cmd(0xaf);  /*开显示*/
	SET(CS);
}

void st7565r_set_addr(u8 page,u8 column)
{
	column=column-0x01;
	st7565r_write_cmd(0xb0+page-1);   /*设置页地址*/
	st7565r_write_cmd(0x10+(column>>4&0x0f));	/*设置列地址的高4位*/
	st7565r_write_cmd(column&0x0f);	/*设置列地址的低4位*/	
}
void st7565r_clear_area(u8 page_start, u8 page_end, u8 column_start, u8 column_end )
{
	int i;
	RESET(CS);
	u8 page_len = page_end - page_start + 1;
	u8 column_len = column_end - column_start +1;
	for(i = 0; i < page_len; i++)
	{
		st7565r_set_addr(page_start + i,column_start);
		for(int i=0; i < column_len; i++)
			st7565r_write_data(0);
	}
	SET(CS);
}
void st7565r_clear_screen()
{
	unsigned char i,j;
	RESET(CS);
	for(i=1;i<9;i++)
	{
		st7565r_set_addr(i,1);
		for(j=0;j<128;j++)
		{
		  	st7565r_write_data(0);
		}
	}
 	SET(CS);
}

void st7565r_show_5x7(uchar page,uchar column,uchar *dp)
{
	uint col_cnt;	
	RESET(CS);	
	st7565r_set_addr(page, column);
	for (col_cnt=0;col_cnt<6;col_cnt++)
	{	
		st7565r_write_data(*dp);
		dp++;
	}
	SET(CS);
}
void st7565r_show_8x16(uchar page,uchar column,uchar *dp)
{
	uint i,j;
	RESET(CS);	
	for(j=0;j<2;j++)
	{
		st7565r_set_addr(page+j,column);
		for (i=0;i<8;i++)
		{	
			st7565r_write_data(*dp);					/*写数据到LCD,每写完一个8位的数据后列地址自动加1*/
			dp++;
		}
	}
	SET(CS);
}
void st7565r_show_12x24(uchar page,uchar column,uchar *dp)
{
	uint i,j;
	RESET(CS);	
	for(j=0;j<3;j++)
	{
		st7565r_set_addr(page+j,column);
		for (i=0;i<12;i++)
		{	
			st7565r_write_data(*dp);					/*写数据到LCD,每写完一个8位的数据后列地址自动加1*/
			dp++;
		}
	}
	SET(CS);
}
void st7565r_show_16x16(uchar page,uchar column,uchar *dp)
{
	uint i,j;
 	RESET(CS);	
	for(j=0;j<2;j++)
	{
		st7565r_set_addr(page+j,column);
		for (i=0;i<16;i++)
		{	
			st7565r_write_data(*dp);					/*写数据到LCD,每写完一个8位的数据后列地址自动加1*/
			dp++;
		}
	}
	SET(CS);
}
void st7565r_show_16x32(uchar page,uchar column,uchar *dp)
{
	uint i,j;
 	RESET(CS);	
	for(j=0;j<4;j++)
	{
		st7565r_set_addr(page+j,column);
		for (i=0;i<16;i++)
		{	
			st7565r_write_data(*dp);					/*写数据到LCD,每写完一个8位的数据后列地址自动加1*/
			dp++;
		}
	}
	SET(CS);
}
void st7565r_show_20x40(uchar page,uchar column,uchar *dp)
{
	uint i,j;
 	RESET(CS);	
	for(j=0;j<5;j++)
	{
		st7565r_set_addr(page+j,column);
		for (i=0;i<20;i++)
		{	
			st7565r_write_data(*dp);					/*写数据到LCD,每写完一个8位的数据后列地址自动加1*/
			dp++;
		}
	}
	SET(CS);
}
void st7565r_show_24x24(uchar page,uchar column,uchar *dp)
{
	int j;
 	RESET(CS);	
	for(j=page;j<page+3;j++)
	{
		st7565r_set_addr(j,column);
		st7565r_write_ndata(dp, 24);
		dp += 24;
	}
	SET(CS);
}
void st7565r_show_24x48(uchar page,uchar column,uchar *dp)
{
	int j;
 	RESET(CS);	
	for(j=page;j<page+6;j++)
	{
		st7565r_set_addr(j,column);
		st7565r_write_ndata(dp, 24);
		dp += 24;
	}
	SET(CS);
}
void st7565r_show_32x12(uchar page,uchar column,uchar *dp)
{
	int j;
 	RESET(CS);	
	for(j=page;j<page+2;j++)
	{
		st7565r_set_addr(j,column);
		st7565r_write_ndata(dp, 32);
		dp += 32;
	}
	SET(CS);
}
void st7565r_show_32x32(uchar page,uchar column,uchar *dp)
{
	int j;
 	RESET(CS);	
	for(j=page;j<page+4;j++)
	{
		st7565r_set_addr(j,column);
		st7565r_write_ndata(dp, 32);
		dp += 32;
	}
	SET(CS);
}

/*显示128x64点阵图像*/
void st7565r_flush(uchar *data)
{
	uint i,j;
	RESET(CS);
	for(j=0;j<8;j++)
	{
		st7565r_set_addr(j+1,1);
		for (i=0;i<128;i++)
		{	
			st7565r_write_data(*data);					/*写数据到LCD,每写完一个8位的数据后列地址自动加1*/
			data++;
		}
	}
	SET(CS);
}



void st7565r_show_page()
{
	RESET(CS);
	for(int n = 1; n <= 8; n++)
	{
		st7565r_set_addr(n, 1);
		for(int i = 1;i <= 128; i++)
		{
			st7565r_write_data(0x80);
		}
	}
	SET(CS);
}
