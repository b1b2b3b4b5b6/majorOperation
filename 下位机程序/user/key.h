#ifndef KEY_H
#define KEY_H

#include "init.h"



#define SHAKE_DELAY	delay_ms(3)	//us

#define ERR 0
#define LEFT 1
#define RIGHT 2
#define OK 3

#define KEY_OK		GPIOB,GPIO_Pin_4
#define KEY_RIGHT	GPIOB,GPIO_Pin_5
#define KEY_LEFT	GPIOB,GPIO_Pin_3

#define R_OK	GPIO_ReadInputDataBit(KEY_OK)
#define R_LEFT	GPIO_ReadInputDataBit(KEY_LEFT)
#define R_RIGHT	GPIO_ReadInputDataBit(KEY_RIGHT)
void key_init(void);
u8 get_key(void);

#endif
