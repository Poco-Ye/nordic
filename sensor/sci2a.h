#ifndef __SCI2A_H
#define __SCI2A_H

#include "nrf_gpio.h"
#include "nrf_delay.h"

typedef const struct sci2a_pfn
{
   void (*__p_sci2a_SendByte)(uint8_t dat);
   uint16_t (*__p_sci2a_recive)(void);
   uint16_t (*__p_getdata)(uint8_t address);
}sci2a_pfn;


sci2a_pfn* sci2a_init(void);







#endif


