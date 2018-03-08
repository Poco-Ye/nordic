#include "hc595.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"

#define DIO  2 
#define SCK  4    
#define RCK  3 

#define DIO2  5  
#define SCK2  7
#define RCK2  6


uint8_t temp[16]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};

void hc595_init(void)
{
   nrf_gpio_cfg_output(DIO);
	 nrf_gpio_cfg_output(SCK);
	 nrf_gpio_cfg_output(RCK); 
	 nrf_gpio_cfg_output(DIO2);
	 nrf_gpio_cfg_output(SCK2);
	 nrf_gpio_cfg_output(RCK2); 
}


void hc595_drive_16(unsigned char ucLedStatusTemp16_09,unsigned char ucLedStatusTemp08_01)
{
   unsigned char i;
   unsigned char ucTempData;

   ucTempData=ucLedStatusTemp16_09; 
   for(i=0;i<8;i++)
   {        
         nrf_gpio_pin_clear(SCK);    
         if((ucTempData&0x80)>0)
				 nrf_gpio_pin_set(DIO);
         else 
				 nrf_gpio_pin_clear(DIO);
				 ucTempData=ucTempData<<1;
         nrf_gpio_pin_set(SCK);   
   }

   ucTempData=ucLedStatusTemp08_01;  
   for(i=0;i<8;i++)
   { 
         nrf_gpio_pin_clear(SCK);    
         if((ucTempData&0x80)>0)
				 nrf_gpio_pin_set(DIO);
         else 
				 nrf_gpio_pin_clear(DIO);
				 ucTempData=ucTempData<<1;
         nrf_gpio_pin_set(SCK);   
   }

   nrf_gpio_pin_clear(RCK);  
   nrf_delay_us(2); 
   nrf_gpio_pin_set(RCK);

}

void hc595_drive_24(unsigned char ucLedStatusTemp24_17,unsigned char ucLedStatusTemp16_09,unsigned char ucLedStatusTemp08_01)
{
   unsigned char i;
   unsigned char ucTempData;

	 ucTempData=ucLedStatusTemp24_17; 
   for(i=0;i<8;i++)
   {        
         nrf_gpio_pin_clear(SCK2);    
         if((ucTempData&0x80)>0)
				 nrf_gpio_pin_set(DIO2);
         else 
				 nrf_gpio_pin_clear(DIO2);
				 ucTempData=ucTempData<<1;
         nrf_gpio_pin_set(SCK2);   
   }
	
	
   ucTempData=ucLedStatusTemp16_09; 
   for(i=0;i<8;i++)
   {        
         nrf_gpio_pin_clear(SCK2);    
         if((ucTempData&0x80)>0)
				 nrf_gpio_pin_set(DIO2);
         else 
				 nrf_gpio_pin_clear(DIO2);
				 ucTempData=ucTempData<<1;
         nrf_gpio_pin_set(SCK2);   
   }

   ucTempData=ucLedStatusTemp08_01;  
   for(i=0;i<8;i++)
   { 
         nrf_gpio_pin_clear(SCK2);    
         if((ucTempData&0x80)>0)
				 nrf_gpio_pin_set(DIO2);
         else 
				 nrf_gpio_pin_clear(DIO2);
				 ucTempData=ucTempData<<1;
         nrf_gpio_pin_set(SCK2);   
   }

   nrf_gpio_pin_clear(RCK2);  
   nrf_delay_us(2); 
   nrf_gpio_pin_set(RCK2);

}





