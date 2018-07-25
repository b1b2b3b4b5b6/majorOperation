#include "cmd.h"
#include "pcap01.h"
u8 cmd_buf[CMD_BUF_LEN+1];
u32 usart_offset = 0;
enum CmdFlag cmd_flag = START; 
u16 len = 0;
u16 len_flag = 0;

#define FLAG_INIT	do{\
	usart_offset = 0;\
	cmd_flag = START;\
	len_flag = 0;\
	}while(0)

#define PARAM_NUM	18
#define VALUE_NUM	43
char *value_table[2][VALUE_NUM] =
{
	{	
		"CMEAS_NORMAL",
		"CMEAS_DRIFT_ONE",
		"CMEAS_DRIFT_DIFF",
		"CMEAS_RD_10K",
		"CMEAS_RD_30K",
		"CMEAS_RD_90K",
		"CMEAS_RD_180K",
		"CMEAS_CLK_20US",
		"CMEAS_CLK_1US",
		"CMEAS_CLK_025US",
		"CMEAS_FAKE_0",
		"CMEAS_FAKE_1",
		"CMEAS_FAKE_2",
		"CMEAS_FAKE_3",
		"CMEAS_FAKE_4",
		"CMEAS_TRIG_SOFT",
		"CMEAS_TRIG_ALWAYS",
		"CMEAS_TRIG_CLOCK",
		"CMEAS_TRIG_PULSE",
		"TMEAS_CYCTIME_140US",
		"TMEAS_CYCTIME_280US",
		"TMEAS_TRIG_SOFT",
		"TMEAS_TRIG_CMEAS",
		"TMEAS_TRIG_CLOCK",
		"TMEAS_TRIG_PULSE",
		"TMEAS_AVRG_0",
		"TMEAS_AVRG_4",
		"TMEAS_AVRG_8",
		"TMEAS_AVRG_16",
		"TMEAS_FAKE_2",
		"TMEAS_FAKE_8",
		"TMEAS_METHOD_1",
		"TMEAS_METHOD_2",
		"TMEAS_METHOD_3",
		"TMEAS_METHOD_4",
		"TMEAS_METHOD_5",
		"TMEAS_METHOD_6",
		"DSP_OTP",
		"DSP_SRAM",
		"DSP_FAST",
		"DSP_SLOW",
		"DSP_CORE_NORMAL",
		"DSP_CORE_LOW",
	},
	{	
		(char *)CMEAS_NORMAL,
		(char *)CMEAS_DRIFT_ONE,
		(char *)CMEAS_DRIFT_DIFF,
		(char *)CMEAS_RD_10K,
		(char *)CMEAS_RD_30K,
		(char *)CMEAS_RD_90K,
		(char *)CMEAS_RD_180K,
		(char *)CMEAS_CLK_20US,
		(char *)CMEAS_CLK_1US,
		(char *)CMEAS_CLK_025US,
		(char *)CMEAS_FAKE_0,
		(char *)CMEAS_FAKE_1,
		(char *)CMEAS_FAKE_2,
		(char *)CMEAS_FAKE_3,
		(char *)CMEAS_FAKE_4,
		(char *)CMEAS_TRIG_SOFT,
		(char *)CMEAS_TRIG_ALWAYS,
		(char *)CMEAS_TRIG_CLOCK,
		(char *)CMEAS_TRIG_PULSE,
		(char *)TMEAS_CYCTIME_140US,
		(char *)TMEAS_CYCTIME_280US,
		(char *)TMEAS_TRIG_SOFT,
		(char *)TMEAS_TRIG_CMEAS,
		(char *)TMEAS_TRIG_CLOCK,
		(char *)TMEAS_TRIG_PULSE,
		(char *)TMEAS_AVRG_0,
		(char *)TMEAS_AVRG_4,
		(char *)TMEAS_AVRG_8,
		(char *)TMEAS_AVRG_16,
		(char *)TMEAS_FAKE_2,
		(char *)TMEAS_FAKE_8,
		(char *)TMEAS_METHOD_1,
		(char *)TMEAS_METHOD_2,
		(char *)TMEAS_METHOD_3,
		(char *)TMEAS_METHOD_4,
		(char *)TMEAS_METHOD_5,
		(char *)TMEAS_METHOD_6,
		(char *)DSP_OTP,
		(char *)DSP_SRAM,
		(char *)DSP_FAST,
		(char *)DSP_SLOW,
		(char *)DSP_CORE_NORMAL,
		(char *)DSP_CORE_LOW,
	}
};

char *param_table[PARAM_NUM] =
{
	"pcx_en",
	"cmeas_method",
	"cmeas_rd_sel",
	"cmeas_clk_sel",
	"seq_time",
	"cmeas_fake",
	"cmeas_avrg",
	"cmeas_trig_sel",
	"cmeas_cycle_time",
	"tmeas_cycle_time",
	"tmeas_trig_sel",
	"tmeas_avrg",
	"tmeas_trig_prediv",
	"tmeas_fake",
	"tmeas_method",
	"dsp_sel",
	"dsp_speed",
	"dsp_core",
};

void cmd_hander(u8 temp)
{
	switch(cmd_flag)
	{
		case START:
			if(temp == STX)
			{
				cmd_flag = PREPARE;
				len_flag = 0;			
			}
			break;
			
		case PREPARE:
			*(((u8 *)&len + len_flag)) = temp;
			len_flag += 1;
			if(len_flag == 2)
			{ 
				endian_swto_u16(len, LITTLE_ENDIAN, (u8 *)&len);
				if(len > CMD_BUF_LEN)
				{
					FLAG_INIT;
					cmd_message("buf overload!\r\n");
				}
				else
				{
					len_flag = 0;
					cmd_flag = RECEIVE;	
				}
			}		
			break;
			
		case RECEIVE:
			cmd_buf[len_flag] = temp;
			len_flag += 1;
			if(len_flag >= len)
				cmd_flag = END;
			break;
			
		case END:
			if(temp == ETX)
			{
				cmd_buf[len] = 0;
				cmd_flag = BUSY;
			}
			else
			{
				FLAG_INIT;
				cmd_message("cmd is not complete!\r\n");
				printf("len:%d",len);
			}
			break;
		
		case BUSY:
			{
				cmd_message("cmd is busy!\r\n");
			}
			break;
		default:
			break;
	}	
}

void cmd_scan()
{
	if(cmd_flag == BUSY)
	{
		cmd_message((char *)cmd_buf);
		cmd_analyse(cmd_buf);
		cmd_flag = START;
	}
		
}
void cmd_message(char *s)
{
	static u8 temp[2000];
	u16 len;
	strcpy((char *)temp,"boot_message\r\n");
	len = strlen((char *)temp);
	strcpy((char *)(temp + len), s);
	len = strlen((char *)temp);
	packer_send_package(temp, len);
}

void cmd_pcap(float value)
{
	u8 temp[100];
	u16 len;
	strcpy((char *)temp,"boot_pcap\r\n");
	len = strlen((char *)temp);
	sprintf((char *)(temp + len), "%f", value);
	len = strlen((char *)temp);
	packer_send_package(temp, len);
}
char * param_division(char *s, char *param)
{
	char *ptr;
	int len;
	ptr = strstr(s, "\r\n");
	if(ptr == NULL)
		return NULL;
	len = ptr - s;
	strncpy(param, s, len);
	*(param+len) = 0;
	ptr += 2;
	return ptr;
}

u8 param_extract(char *param, char *name, char *value)
{
	char * ptr;
	int len;
	ptr = strchr(param, ':');
	if(ptr == NULL)
		return -1;
	len = ptr - param;
	if(len == 0)
		return -1;
	strncpy(name, param, len);
	*(name + len) = 0;
	ptr += 1;
	if(*ptr == 0)
		return -1;
	strcpy(value, ptr);
	return	0;
}

u8 param_load(PCAP_TypeDef *pcap_reg, char *name, char *value)
{
	int n, i;
	for(n = 0; n < PARAM_NUM; n++)
	{
		if(strcmp(name, param_table[n]) == 0)
		{
			for(i = 0; i < VALUE_NUM; i++)
			{
				if(strcmp(value, value_table[0][i]) == 0)
				{
					*((u32 *)pcap_reg + n) = (u32)value_table[1][i];
					return 0;
				}
			}
			u32 temp;
			if(sscanf(value, "%uld", &temp) != EOF)
			{
				*((u32 *)pcap_reg + n) = temp;
				return 0;
			}
			return -1;
		}
	}
	return -1;
}

void cmd_analyse(u8 *cmd)
{
	char name[50];
	char value[50];
	char temp[100];
	char *ptr;
	PCAP_TypeDef reg;
	ptr = (char *)cmd;
	u8 res;
	do
	{
		ptr = param_division(ptr, temp);
		if(ptr == NULL)
			break;
		res = param_extract(temp, name, value);
		if(res == 0)
		{
			param_load(&reg, name, value);
		}
	}while(ptr != NULL);
	pcap_init(&reg);
}