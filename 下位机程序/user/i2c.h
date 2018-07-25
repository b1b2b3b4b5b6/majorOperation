#ifndef I2C_H
#define I2C_H
#include"init.h"

#define PIN_SCL		GPIO_Pin_6
#define PIN_SDA		GPIO_Pin_7

#define I2C_SCL_OUT 	gpio_init_pin(GPIOB,PIN_SCL,GPIO_Mode_Out_PP)	
#define I2C_SCL_SET		GPIO_SetBits(GPIOB,PIN_SCL)
#define I2C_SCL_RESET		GPIO_ResetBits(GPIOB,PIN_SCL)

#define I2C_SDA_OUT	gpio_init_pin(GPIOB,PIN_SDA,GPIO_Mode_Out_OD)
#define I2C_SDA_IN		gpio_init_pin(GPIOB,PIN_SDA,GPIO_Mode_IPU)
#define I2C_SDA_SET		GPIO_SetBits(GPIOB,PIN_SDA)
#define I2C_SDA_RESET	GPIO_ResetBits(GPIOB,PIN_SDA)
#define I2C_SDA_R			GPIO_ReadInputDataBit(GPIOB,PIN_SDA)

#define SCL_DELAY		//delay_us(50)
#define RESTART_DELAY	//delay_us(10)


void delay2us(void);
void i2c_init(void);			//i2c��ʼ��
void i2c_start(void);			//��ʼ
void i2c_stop(void);			//ֹͣ

uchar i2c_write_byte(uchar byte);	//�����ֽ�
uchar i2c_read_byte(uchar ack);			//�����ֽ�

uchar i2c_read_reg(u8 slave, uchar reg);//��һ�ֽ�
uchar i2c_write_reg(u8 slave, uchar reg,uchar byte);//дһ�ֽ�
uchar i2c_read_buf(u8 slave, uchar reg,uchar *date,uchar bytes);		//����һ������	���ز������
uchar i2c_write_buf(u8 slave,uchar reg,uchar *date,uchar bytes);	//����һ������	���ز������

#endif
