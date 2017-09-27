#ifndef __SCI2A_H
#define __SCI2A_H

#include "nrf_gpio.h"
#include "nrf_delay.h"

typedef const struct sci2a_pfn
{
   void (*__p_sci2a_SendByte)(uint8_t dat);
   uint16_t (*__p_sci2a_recive)(void);
   uint16_t (*__p_getdata)(uint8_t address);
	 void (*__p_sci2a_SendByte2)(uint8_t dat);
   uint16_t (*__p_sci2a_recive2)(void);
   uint16_t (*__p_getdata2)(uint8_t address);
	 void (*__p_getdistance)(void);
	
}sci2a_pfn;

typedef  struct sci2a_handle
{
   sci2a_pfn *pfn;
	 uint16_t  handle_id;
	 uint16_t  touch_sum;
}sci2a_handle;


sci2a_handle* sci2a_init(void);







#endif


