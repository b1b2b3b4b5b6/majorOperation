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
void i2c_init(void);			//i2c初始化
void i2c_start(void);			//开始
void i2c_stop(void);			//停止

uchar i2c_write_byte(uchar byte);	//传输字节
uchar i2c_read_byte(uchar ack);			//接收字节

uchar i2c_read_reg(u8 slave, uchar reg);//读一字节
uchar i2c_write_reg(u8 slave, uchar reg,uchar byte);//写一字节
uchar i2c_read_buf(u8 slave, uchar reg,uchar *date,uchar bytes);		//接收一组数据	返回操作结果
uchar i2c_write_buf(u8 slave,uchar reg,uchar *date,uchar bytes);	//发送一组数据	返回操作结果

#endif
