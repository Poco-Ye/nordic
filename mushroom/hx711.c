#include "hx711.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"



#define  ADDO  17
#define  ADSK  18

uint32_t hx711_read(void)
{
  uint32_t Count = 0;
  uint8_t i;
	
	nrf_gpio_cfg_output(ADDO);
	nrf_gpio_cfg_output(ADSK);
	nrf_gpio_pin_set(ADDO);
	nrf_gpio_pin_clear(ADSK);   //ADSK=0;
  Count=0;
	nrf_gpio_cfg_input(ADDO,NRF_GPIO_PIN_NOPULL);
  while(nrf_gpio_pin_read(ADDO) == 1); 
  for (i=0;i<24;i++)
  {
   nrf_gpio_pin_set(ADSK);  //ADSK=1; 
   Count=Count<<1; 
   nrf_gpio_pin_clear(ADSK); //ADSK=0; 
   if(nrf_gpio_pin_read(ADDO) == 1) Count++;
  }
   nrf_gpio_pin_set(ADSK);    //ADSK=1;
   Count=Count^0x800000;
   nrf_gpio_pin_clear(ADSK);   //ADSK=0;
   return(Count);
}


