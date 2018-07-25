#ifndef PACKER_H
#define PACKER_H
#include "usart.h"

#define LOCAL_MODE	LITTLE_ENDIAN

#define	LITTLE_ENDIAN	0
#define BIG_RNDIAN	1
#define IS_PACKER_MODE(mode)	((mode == LITTLE_ENDIAN) || (mode == BIG_RNDIAN))

#define packer_send_byte(byte)	usart_send_byte(byte)
#define packer_send_buf(buf, len)	usart_send_buf(buf, len)

#define PACKAGE_START	"cmd_start\r\n"
#define PACKAGE_END		"cmd_end\r\n"

void endian_swto_u16(u16 num, u8 mode, u8 *buf); 
void endian_swto_u32(u32 num, u8 mode, u8 *buf);

void packer_send_package(u8 *data, u16 len);

#endif