#ifndef ADC_H
#define ADC_H

#include "init.h"
	
#define PIN_TEMP	GPIOA,GPIO_Pin_1 
#define PIN_POWER	GPIOA,GPIO_Pin_2

#define CH_TEMP		1
#define CH_POWER	2

extern u16 temperature;
extern u16 power;

void adc_init(void);
void adc_sample(void);


#endif
