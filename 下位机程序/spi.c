#include"spi.h"

void spi_init(void)
{
	SPI_CS_OUT;
	SPI_CS_SET;
	SPI_SCK_OUT;
	SPI_SCK_RESET;
	SPI_MOSI_OUT;
	SPI_MOSI_SET;
	SPI_MISO_IN;
}
uchar spi_rw(uchar byte)
{
	uchar n;
	for(n=0;n<8;n++)
	{

		SPI_SCK_SET;
		if(byte&0x80)
			SPI_MOSI_SET;
		else
			SPI_MOSI_RESET;
		byte<<=1;
		DELAY_SCK;
		SPI_SCK_RESET;
		byte |= SPI_MISO_R;
		DELAY_SCK;
	}
	return byte;
}
uchar spi_write_reg(uchar reg, uchar value)
{
	uchar status;
	SPI_CS_RESET;
	status = spi_rw(reg);
	DELAY_REG; 
	spi_rw(value); 
	SPI_CS_SET;
	return	status;
} 



uchar spi_read_reg(uchar reg)
{	
	uchar value;
	SPI_CS_RESET; 
	spi_rw(reg);
	DELAY_REG; 
	value=spi_rw(0); 
	SPI_CS_SET; 
	return value;
} 

uchar spi_write_buf(uchar reg,uchar *Outbuf,uchar n)
{
	uchar i,status;
	SPI_CS_RESET;
	status= spi_rw(reg);
	DELAY_REG;
	for(i=0;i<n;i++)
	{
		spi_rw(*Outbuf);
		Outbuf++;
	}
	SPI_CS_SET;
	return status;
}
uchar spi_read_buf(uchar reg,uchar *Inbuf,uchar n)
{
	uchar i,status;
	SPI_CS_RESET;
	status = spi_rw(reg);
	DELAY_REG;
	for(i=0;i<n;i++)
	{
		*Inbuf=spi_rw(0);
		Inbuf++;
	}
	SPI_CS_SET;
	return status;
}