#ifndef __HC595_H
#define __HC595_H
#include "nrf_gpio.h"
#include "nrf_delay.h"


void hc595_drive_16(unsigned char ucLedStatusTemp16_09,unsigned char ucLedStatusTemp08_01);
void hc595_init(void);
void hc595_drive_24(unsigned char ucLedStatusTemp24_17,unsigned char ucLedStatusTemp16_09,unsigned char ucLedStatusTemp08_01);


#endif


