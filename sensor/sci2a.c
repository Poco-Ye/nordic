#include "sci2a.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"

#define  SDA  11
#define  SCL  12

nrf_gpio_pin_pull_t config = NRF_GPIO_PIN_NOPULL ;

static void sci2a_SendByte(uint8_t dat);
static uint16_t sci2a_recive(void);
static uint16_t read_sci2a(uint8_t address);


static sci2a_pfn pfn = {
  sci2a_SendByte,
	sci2a_recive,
	read_sci2a
};



static void sci2a_Start(void)
{
	  nrf_gpio_pin_set(SDA);
	  nrf_gpio_pin_set(SCL);
    nrf_delay_us(5);                 
	  nrf_gpio_pin_clear(SDA);
    nrf_delay_us(5);               
	  nrf_gpio_pin_clear(SCL);
}

sci2a_pfn* sci2a_init(void)
{
    
		nrf_gpio_cfg_output(SDA);
	  nrf_gpio_cfg_output(SCL);
	  nrf_gpio_pin_set(SDA);
	  nrf_gpio_pin_set(SCL);
	  return &pfn;
}




static void sci2a_Stop(void)
{   
	  nrf_gpio_pin_clear(SDA);
	  nrf_gpio_pin_set(SCL);
    nrf_delay_us(5);                                   
	  nrf_gpio_pin_set(SDA);
    nrf_delay_us(5);               
}

static void sci2a_RecvACK(void)
{
	  nrf_gpio_pin_set(SCL);
    nrf_delay_us(5);                                 
	  nrf_gpio_pin_clear(SCL); 
    nrf_delay_us(5);             
}


static void sci2a_SendByte(uint8_t dat)
{
    uint8_t i;
    for (i=0; i<8; i++)       
    {
        if((dat&0x80)>>7)  
           	nrf_gpio_pin_set(SDA);
        else
            nrf_gpio_pin_clear(SDA);							
			  dat<<=1;
				nrf_gpio_pin_set(SCL);
        nrf_delay_us(5);             
				nrf_gpio_pin_clear(SCL);
        nrf_delay_us(5);            
    }
    sci2a_RecvACK();            
}


static uint16_t sci2a_recive(void)         
{
    uint8_t i;
    uint16_t dat = 0;
	  nrf_gpio_pin_set(SDA); 
    nrf_gpio_cfg_input(SDA,NRF_GPIO_PIN_NOPULL);
    for (i=0; i<16; i++)         
    {
        dat <<= 1;
			  nrf_gpio_pin_set(SCL);
        nrf_delay_us(5);            
        dat |= nrf_gpio_pin_read(SDA);    
        nrf_gpio_pin_clear(SCL);			
        nrf_delay_us(5);            
    }
		nrf_gpio_cfg_output(SDA);
    return dat;
}


static uint16_t read_sci2a(uint8_t address)
{
	uint16_t res;
  sci2a_Start();
  sci2a_SendByte(address);	
	res = sci2a_recive();
	sci2a_Stop();  
	return res;
}


















