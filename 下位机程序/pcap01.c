#include "pcap01.h"
extern u8 pcap_hex[];



void pcap_init(PCAP_TypeDef *pcap_reg)
{
	u32 reg[11];
	reg[0] = 0x4200FF;
	reg[1] = 0x201022;
	reg[2] = 0x00000B | (pcap_reg->pcx_en<<16) | pcap_reg->cmeas_method| pcap_reg->cmeas_rd_sel;
	reg[3] = pcap_reg->cmeas_clk_sel | ((pcap_reg->seq_time & 0x1f)<<16) | pcap_reg->cmeas_fake | (pcap_reg->cmeas_avrg & 0xfff);
	reg[4] = pcap_reg->cmeas_trig_sel | ((pcap_reg->cmeas_cycle_time & 0x3ff)<<8) | pcap_reg->tmeas_cycle_time | pcap_reg->tmeas_trig_sel;
	reg[5] = pcap_reg->tmeas_avrg | (pcap_reg->tmeas_trig_prediv & 0x3fffff);
	reg[6] = 0x000040 | pcap_reg->tmeas_fake | pcap_reg->tmeas_method;
	reg[7] = 0x1F0000;
	reg[8] = pcap_reg->dsp_sel | pcap_reg->dsp_speed;
	reg[9] = 0x000000;
	reg[10] = 0x180000 | pcap_reg->dsp_core;
	pcap_configure(reg);
	pcap_cmd(ENABLE);
	delay_ms(500);
	
}

void pcap_reg_init(PCAP_TypeDef *pcap_reg)
{
	pcap_reg->pcx_en = 0x0f;
	pcap_reg->cmeas_method = CMEAS_DRIFT_ONE;
	pcap_reg->cmeas_rd_sel = CMEAS_RD_10K;
	pcap_reg->cmeas_clk_sel = CMEAS_CLK_20US;
	pcap_reg->seq_time = 0;
	pcap_reg->cmeas_fake = CMEAS_FAKE_0;
	pcap_reg->cmeas_avrg = 0;
	pcap_reg->cmeas_trig_sel = CMEAS_TRIG_SOFT;
	pcap_reg->cmeas_cycle_time = 49;	//1ms
	pcap_reg->tmeas_cycle_time = TMEAS_CYCTIME_140US;
	pcap_reg->tmeas_trig_sel = TMEAS_TRIG_SOFT;
	pcap_reg->tmeas_avrg = TMEAS_AVRG_0;
	pcap_reg->tmeas_trig_prediv = 0;
	pcap_reg->tmeas_fake = TMEAS_FAKE_2;
	pcap_reg->tmeas_method = TMEAS_METHOD_2;
	pcap_reg->dsp_sel = DSP_SRAM;
	pcap_reg->dsp_speed = DSP_FAST;
	pcap_reg->dsp_core = DSP_CORE_NORMAL;

}

u8 pcap_deinit()
{
	u8 res = 0;
	u8 re_data[928];
	spi_init();
	pcap_write_cmd(PCAP_POWER_RESET);
	delay_ms(500);
	pcap_write_sram(0,0x55);
	res = pcap_read_sram(0);
	if(res != 0x55)
		return -1;
	
	pcap_write_cmd(PCAP_POWER_RESET);
	delay_ms(500);
	
	pcap_download(pcap_hex, 0, 928);
	pcap_export(re_data, 0, 928);
	u16 n;
	n = 0;
	while(n < 928)
	{
		if(pcap_hex[n]!=re_data[n])
			return -1;
		n++;
	}	
	PCAP_TypeDef reg;
	pcap_reg_init(&reg);
	pcap_init(&reg);

	return 0;
}
u32 pcap_read_res(u8 address)
{
	u32 res = 0;
	SPI_CS_RESET;
	spi_rw(0x40|address);
	res |= spi_rw(0);
	res<<=8;
	res |= spi_rw(0);
	res<<=8;
	res |= spi_rw(0);
	SPI_CS_SET;
	return res;
}

void pcap_configure(u32 *reg)
{
	u8 n = 0;
	for(n = 0; n <= 10; n++)
		pcap_write_config(n, reg[n]);
}
void pcap_write_cmd(u8 cmd)
{
	SPI_CS_RESET;
	spi_rw(cmd);
	SPI_CS_SET;
}

void pcap_write_sram(u16 address, u8 data)
{
	u8 temp;
	SPI_CS_RESET;
	temp = address>>8;
	temp|=0x90;
	spi_rw(temp);
	temp = address;
	spi_rw(temp);
	spi_rw(data);
	SPI_CS_SET;
}

u8 pcap_read_sram(u16 address)
{
	u8 temp;
	SPI_CS_RESET;
	temp = address>>8;
	temp|=0x10;
	spi_rw(temp);
	temp = address;
	spi_rw(temp);
	temp = spi_rw(0);
	SPI_CS_SET;
	return temp;
}

void pcap_write_config(u8 address, u32 config)
{
	u8 temp;
	SPI_CS_RESET;
	spi_rw(0xC0|address);
	temp = config>>16;
	spi_rw(temp);
	temp = config>>8;
	spi_rw(temp);
	temp = config>>0;
	spi_rw(temp);
	SPI_CS_SET;
}

void pcap_download(u8 *data, u16 address, u16 len)
{
	while(len > 0)
	{
		pcap_write_sram(address, *data);
		len--;
		data++;
		address++;
	}

}


void pcap_export(u8 *data, u16 address, u16 len)
{
	while(len > 0)
	{
		*data = pcap_read_sram(address);
		len--;
		data++;
		address++;
	}

}
void pcap_cmd(u8 state)
{
	if(state != DISABLE)
	{
		pcap_write_config(20, 1);
		pcap_write_cmd(PCAP_INIT);	
	}

	else
		pcap_write_config(20, 0);
}

