#ifndef __USART_H
#define __USART_H
#include "init.h"
  	
void uart_init(u32 bound);
void usart_send_buf(u8 *data, u16 len);
void usart_send_byte(u8 byte);


#endif


