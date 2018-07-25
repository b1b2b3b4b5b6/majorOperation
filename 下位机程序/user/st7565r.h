#ifndef ST7565R_H
#define ST7565R_H

#include "init.h"

void st7565r_init(void);
void st7565r_clear_area(u8 page_start, u8 page_end, u8 column_start, u8 column_end );
void st7565r_clear_screen(void);
void st7565r_show_5x7(uchar page,uchar column,uchar *dp);
void st7565r_show_8x16(uchar page,uchar column,uchar *dp);
void st7565r_show_12x24(uchar page,uchar column,uchar *dp);
void st7565r_show_16x16(uchar page,uchar column,uchar *dp);
void st7565r_show_16x32(uchar page,uchar column,uchar *dp);
void st7565r_show_20x40(uchar page,uchar column,uchar *dp);
void st7565r_show_24x24(uchar page,uchar column,uchar *dp);
void st7565r_show_24x48(uchar page,uchar column,uchar *dp);
void st7565r_show_32x12(uchar page,uchar column,uchar *dp);
void st7565r_show_32x32(uchar page,uchar column,uchar *dp);
void st7565r_flush(uchar *data);
void st7565r_show_page(void);

#ifdef ST7564R_HERE
	#define SCK_DELAY
	#define POWERUP_DELAY delay_ms(100)
	#define SCK		GPIOB,GPIO_Pin_1
	#define SDA		GPIOB,GPIO_Pin_0
	#define RS		GPIOB,GPIO_Pin_2
	#define RST		GPIOB,GPIO_Pin_10
	#define CS		GPIOB,GPIO_Pin_11


	#define OUT(arg)	gpio_init_pin(arg,GPIO_Mode_Out_PP)
	#define IN(arg)		gpio_init_pin(arg,GPIO_Mode_IPU)
	#define SET(arg)	GPIO_SetBits(arg)
	#define RESET(arg)	GPIO_ResetBits(arg)
	#define READ(arg)	GPIO_ReadInputDataBit(arg)
#endif


#endif
