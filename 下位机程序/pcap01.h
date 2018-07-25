#ifndef PCAP01_H
#define PCAP01_H

#include "spi.h"
//reg2
#define	CMEAS_PORT_EN			((u32)0xFF0000)
#define CMEAS_BITS				((u32)0x00F800)
#define RDCHG_INT_SEL			((u32)0x000700)

//reg3
#define	CY_CLK_SEL				((u32)0xC00000)
#define	SEQ_TIME				((u32)0x1F0000)
#define CMEAS_FAKE				((u32)0x006000)
#define C_AVRG					((u32)0x001FFF)

//reg4
#define	CMEAS_STARTPIN			((u32)0x300000)
#define CMEAS_TRIG_SEL			((u32)0x0C0000)
#define CMEAS_CYTIME			((u32)0x03FF00)
#define	TMEAS_CYTIME			((u32)0x000010)
#define TMEAS_STARTPIN			((u32)0x00000C)
#define TMEAS_TRIG_SEL			((u32)0x000003)

//reg5
#define T_AVRG					((u32)0xC00000)
#define	TMEAS_TRIG_PREDIV		((u32)0x7FFFFF)

//reg6
#define	TMEAS_FAKE				((u32)0x008000)
#define TMEAS_7BITS				((u32)0x007F00)

//reg7

//reg8
#define	DSP_SRAM_SEL			((u32)0x800000)
#define	DSP_START				((u32)0x400000)
#define DSP_STARTONOVFL			((u32)0x200000)
#define DSP_STARTONTEMP			((u32)0x100000)
#define DSP_STARTPIN			((u32)0x0F0000)
#define	DSP_FF_IN				((u32)0x00F000)
#define DSP_WATCHDOG_LENGTH		((u32)0x000C00)
#define DSP_MOFLO_EN			((u32)0x000300)
#define DSP_SPEED				((u32)0x000030)
#define INT2PG2					((u32)0x000004)
#define PG1_X_G3				((u32)0x000002)
#define PG0_X_G2				((u32)0x000001)

//reg9
#define	PG_DIR_IN				((u32)0xF00000)
#define PG_PULL_UP				((u32)0x0F0000)
#define PI_EN					((u32)0x00F000)
#define	PI1_CLK_SEL				((u32)0x000F00)
#define PI0_CLK_SEL				((u32)0x0000F0)
#define PI1_RES					((u32)0x00000C)
#define	PI0_RES					((u32)0x000003)

//reg10
#define	V_CORE_CTL				((u32)0x0000FF)


//res_reg
#define CAP_C0		0
#define CAP_C1		1
#define CAP_STATUS	8

//cmd
#define PCAP_POWER_RESET	0x88
#define	PCAP_INIT	0x8a
#define PCAP_CMEAS	0x8c
#define PCAP_TMEAS	0x8e

/*参数宏定义*/
#define	CMEAS_NORMAL		((u32)0x20<<11)
#define	CMEAS_DRIFT_ONE		((u32)0x08<<11)
#define CMEAS_DRIFT_DIFF	((u32)0x10<<11)
#define CMEAS_RD_10K		((u32)0x07<<8)
#define	CMEAS_RD_30K		((u32)0x06<<8)
#define CMEAS_RD_90K		((u32)0x05<<8)
#define CMEAS_RD_180K		((u32)0x04<<8)
#define CMEAS_CLK_20US		((u32)0x00<<22)
#define CMEAS_CLK_1US		((u32)0x02<<22)
#define CMEAS_CLK_025US		((u32)0x03<<22)
#define CMEAS_FAKE_0		((u32)0x00<<13)
#define CMEAS_FAKE_1		((u32)0x01<<13)
#define CMEAS_FAKE_2		((u32)0x02<<13)
#define CMEAS_FAKE_3		((u32)0x03<<13)
#define CMEAS_FAKE_4		((u32)0x04<<13)

#define CMEAS_TRIG_SOFT		((u32)0x00<<18)
#define CMEAS_TRIG_ALWAYS	((u32)0x01<<18)
#define CMEAS_TRIG_CLOCK	((u32)0x02<<18)
#define CMEAS_TRIG_PULSE	((u32)0x03<<18)
#define TMEAS_CYCTIME_140US	((u32)0x00<<4)
#define TMEAS_CYCTIME_280US	((u32)0x01<<4)
#define TMEAS_TRIG_SOFT		((u32)0x00<<0)
#define TMEAS_TRIG_CMEAS	((u32)0x01<<0)
#define TMEAS_TRIG_CLOCK	((u32)0x02<<0)
#define TMEAS_TRIG_PULSE	((u32)0x03<<0)
#define TMEAS_AVRG_0		((u32)0x00<<22)
#define TMEAS_AVRG_4		((u32)0x01<<22)
#define TMEAS_AVRG_8		((u32)0x02<<22)
#define TMEAS_AVRG_16		((u32)0x03<<22)

#define TMEAS_FAKE_2		((u32)0x00<<15)
#define TMEAS_FAKE_8		((u32)0x01<<15)
#define TMEAS_METHOD_1		((u32)0x43<<8)
#define TMEAS_METHOD_2		((u32)0x4b<<8)
#define TMEAS_METHOD_3		((u32)0x49<<8)
#define TMEAS_METHOD_4		((u32)0x06<<8)
#define TMEAS_METHOD_5		((u32)0x0e<<8)
#define TMEAS_METHOD_6		((u32)0x0c<<8)

#define DSP_OTP				((u32)0x00<<23)
#define DSP_SRAM			((u32)0x01<<23)
#define DSP_FAST			((u32)0x01<<4)
#define DSP_SLOW			((u32)0x03<<4)

#define DSP_CORE_NORMAL		((u32)0x47<<0)
#define DSP_CORE_LOW		((u32)0x87<<0)


typedef struct
{
	u32	pcx_en;
	u32	cmeas_method;
	u32	cmeas_rd_sel;
	u32	cmeas_clk_sel;
	u32	seq_time;
	u32	cmeas_fake;
	u32	cmeas_avrg;
	u32	cmeas_trig_sel;
	u32 cmeas_cycle_time;
	u32	tmeas_cycle_time;
	u32	tmeas_trig_sel;
	u32	tmeas_avrg;
	u32	tmeas_trig_prediv;
	u32	tmeas_fake;
	u32	tmeas_method;
	u32	dsp_sel;
	u32	dsp_speed;
	u32	dsp_core;
	
}PCAP_TypeDef;

u8 pcap_deinit();
void pcap_init(PCAP_TypeDef *pcap_reg);
void pcap_reg_init(PCAP_TypeDef *pcap_reg);
void pcap_cmd(u8 state);
void pcap_configure(u32 *reg);
u32 pcap_read_res(u8 address);
void pcap_write_cmd(u8 cmd);
void pcap_write_sram(u16 address, u8 data);
u8 pcap_read_sram(u16 address);
void pcap_write_config(u8 address, u32 config);
void pcap_download(u8 *data, u16 address, u16 len);
void pcap_export(u8 *data, u16 address, u16 len);
#endif