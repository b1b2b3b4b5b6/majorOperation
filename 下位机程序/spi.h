#ifndef SPI_H
#define SPI_H

#include "init.h"
#define DELAY_REG 	;
#define DELAY_SCK	;
#define GPIO_GROUP	GPIOA
#define PIN_SCK		GPIO_Pin_9
#define PIN_MISO	GPIO_Pin_8
#define PIN_MOSI	GPIO_Pin_11
#define PIN_CS		GPIO_Pin_10

#define SPI_SCK_OUT 	gpio_init_pin(GPIO_GROUP,PIN_SCK,GPIO_Mode_Out_PP)	
#define SPI_SCK_SET		GPIO_SetBits(GPIO_GROUP,PIN_SCK)
#define SPI_SCK_RESET	GPIO_ResetBits(GPIO_GROUP,PIN_SCK)

#define SPI_MOSI_OUT 	gpio_init_pin(GPIO_GROUP,PIN_MOSI,GPIO_Mode_Out_PP)	
#define SPI_MOSI_SET	GPIO_SetBits(GPIO_GROUP,PIN_MOSI)
#define SPI_MOSI_RESET	GPIO_ResetBits(GPIO_GROUP,PIN_MOSI)

#define SPI_MISO_IN 	gpio_init_pin(GPIO_GROUP,PIN_MISO,GPIO_Mode_IN_FLOATING)	
#define SPI_MISO_R		GPIO_ReadInputDataBit(GPIO_GROUP,PIN_MISO)

#define SPI_CS_OUT 		gpio_init_pin(GPIO_GROUP,PIN_CS,GPIO_Mode_Out_PP)	
#define SPI_CS_SET		GPIO_SetBits(GPIO_GROUP,PIN_CS)
#define SPI_CS_RESET	GPIO_ResetBits(GPIO_GROUP,PIN_CS);

uchar spi_rw(uchar byte);
uchar spi_write_reg(uchar reg,uchar value);
uchar spi_read_reg(uchar reg);
uchar spi_write_buf(uchar reg,uchar *OutBuf,uchar n);
uchar spi_read_buf(uchar reg,uchar *InBuf,uchar n);
void spi_init(void);
#endif