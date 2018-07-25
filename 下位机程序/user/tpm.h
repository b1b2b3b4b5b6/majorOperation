#ifndef TPM_H
#define TPM_H

#include "init.h"


#define FINISH	1
#define BUSY	0
enum cap{
	pf = 1,
	nf,
	uf,
};
#define SWITCH	GPIOB,GPIO_Pin_9 
#define FALLING	GPIOA,GPIO_Pin_8 

#define DISCHARGE()	GPIO_SetBits(SWITCH)
#define CHARGE()		GPIO_ResetBits(SWITCH)

void tpm_init(void);
void time_init(void);
void tpm_start(void);
u8 tpm_flag();
float get_time(void);
float tpm_measure(void);
void set_range(enum cap range);

#endif
