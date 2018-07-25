#ifndef DISPALY_H
#define DISPALY_H


#include "st7565r.h"


void show_num_string(u8 page, u8 column, char *s, u8 size);
void show_num_char(u8 page, u8 column, char c, u8 size);
void show_array(u8 page, u8 column, u8 *buf, u8 len, u8 size);
void show_temperature(u16 temperature);

void show_status(u8 battery, u8 flag_temp, u8 flag_tpm);
void show_tpm(u16 tpm);


#endif
