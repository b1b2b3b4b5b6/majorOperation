#ifndef POWER_H
#define POWER_H


#define LCON		GPIOA,GPIO_Pin_7

#define power_init() do{\
						gpio_init_pin(LCON,GPIO_Mode_Out_PP);\
						TURN_ON(LCON);\
					}while(0)

#define TURN_OFF(arg)	GPIO_SetBits(arg)
#define TURN_ON(arg)	GPIO_ResetBits(arg)
					
#endif
					