#ifndef CMD_H
#define CMD_H
#include "init.h"
#include "packer.h"

#define STX	0x02
#define ETX	0x03
#define CMD_BUF_LEN	2048

enum CmdFlag{START, PREPARE, RECEIVE, END, BUSY};

void cmd_message(char *s);
void cmd_hander(u8 temp);
void cmd_scan();
void cmd_analyse(u8 *cmd);
void cmd_pcap(float value);
#endif