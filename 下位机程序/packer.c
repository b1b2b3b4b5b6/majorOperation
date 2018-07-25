#include "packer.h"

void endian_swto_u16(u16 num, u8 mode, u8 *buf)
{
	u8 temp[2];
	temp[0] = *((u8 *)&num);
	temp[1] = *((u8 *)&num+1);
	assert_param(IS_PACKER_MODE(mode));
	if(mode == LOCAL_MODE)
	{
		buf[0] = temp[0];
		buf[1] = temp[1];
	}
	else
	{
		buf[0] = temp[1];
		buf[1] = temp[0];		
	}
}

void endian_swto_u32(u32 num, u8 mode, u8 *buf)
{
	assert_param(mode);
	u8 temp[4];
	temp[0] = *((u8 *)&num);
	temp[1] = *((u8 *)&num+1);
	temp[2] = *((u8 *)&num+2);
	temp[3] = *((u8 *)&num+3);
	if(mode == LOCAL_MODE)
	{
		buf[0] = temp[0];
		buf[1] = temp[1];
		buf[2] = temp[2];
		buf[3] = temp[3];
	}
	else
	{
		buf[0] = temp[3];
		buf[1] = temp[2];
		buf[2] = temp[1];
		buf[3] = temp[0];	
	}
}

void packer_send_package(u8 *data, u16 len)
{
	u8 temp[20];
	sprintf((char *)temp, PACKAGE_START);
	packer_send_buf(temp, strlen((char const *)temp));
	endian_swto_u16(len, LITTLE_ENDIAN, temp);
	packer_send_buf(temp, 2);
	packer_send_buf(data, len);
	sprintf((char *)temp, PACKAGE_END);
	packer_send_buf(temp, strlen((char const *)temp));
}