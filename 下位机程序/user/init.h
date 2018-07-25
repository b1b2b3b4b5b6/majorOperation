/*
 * init.h
 *
 * Created: 2015/10/6 17:06:27
 *  Author: God
 */ 


#ifndef INIT_H_
#define INIT_H_
#define uchar u8
#define uint u16

#include "stm32f10x.h"
#include "sys.h"
#include "stdio.h"
#include "string.h"
//#include "malloc.h"	  



void clock_init(void);
void delay_us(u32 nus);
void delay_ms(u32 nms);
void gpio_init_pin(GPIO_TypeDef* GPIOx, u16 GPIO_Pin,GPIOMode_TypeDef MODE);

#endif /* INIT_H_ */
