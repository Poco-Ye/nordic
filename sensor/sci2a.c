#include "sci2a.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"

#define  SDA  13
#define  SCL  14
#define  SDA2 11
#define  SCL2 12
#define  DISTANCE  1310


nrf_gpio_pin_pull_t config = NRF_GPIO_PIN_NOPULL ;


uint8_t  touch_min,touch_max,touch_one = 1,touch[48];
uint16_t sum;
//static uint16_t touch_sum,handle_id;




static void sci2a_SendByte(uint8_t dat);
static uint16_t sci2a_recive(void);
static uint16_t read_sci2a(uint8_t address);
static void sci2a_SendByte2(uint8_t dat);
static uint16_t sci2a_recive2(void);
static uint16_t read_sci2a2(uint8_t address);
static void getdistance(void);

static sci2a_pfn pfn = {
  sci2a_SendByte,
	sci2a_recive,
	read_sci2a,
	sci2a_SendByte2,
	sci2a_recive2,
	read_sci2a2,
	getdistance
};

static sci2a_handle handle = {
   &pfn,
	 
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

static void sci2a_Start2(void)
{
	  nrf_gpio_pin_set(SDA2);
	  nrf_gpio_pin_set(SCL2);
    nrf_delay_us(5);                 
	  nrf_gpio_pin_clear(SDA2);
    nrf_delay_us(5);               
	  nrf_gpio_pin_clear(SCL2);
}






sci2a_handle* sci2a_init(void)
{
    
		nrf_gpio_cfg_output(SDA);
	  nrf_gpio_cfg_output(SCL);
	  nrf_gpio_pin_set(SDA);
	  nrf_gpio_pin_set(SCL);
		nrf_gpio_cfg_output(SDA2);
	  nrf_gpio_cfg_output(SCL2);
	  nrf_gpio_pin_set(SDA2);
	  nrf_gpio_pin_set(SCL2);
	  return &handle;
}




static void sci2a_Stop(void)
{   
	  nrf_gpio_pin_clear(SDA);
	  nrf_gpio_pin_set(SCL);
    nrf_delay_us(5);                                   
	  nrf_gpio_pin_set(SDA);
    nrf_delay_us(5);               
}

static void sci2a_Stop2(void)
{   
	  nrf_gpio_pin_clear(SDA2);
	  nrf_gpio_pin_set(SCL2);
    nrf_delay_us(5);                                   
	  nrf_gpio_pin_set(SDA2);
    nrf_delay_us(5);               
}



static void sci2a_RecvACK(void)
{
	  nrf_gpio_pin_set(SCL);
    nrf_delay_us(5);                                 
	  nrf_gpio_pin_clear(SCL); 
    nrf_delay_us(5);             
}

static void sci2a_RecvACK2(void)
{
	  nrf_gpio_pin_set(SCL2);
    nrf_delay_us(5);                                 
	  nrf_gpio_pin_clear(SCL2); 
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

static void sci2a_SendByte2(uint8_t dat)
{
    uint8_t i;
    for (i=0; i<8; i++)       
    {
        if((dat&0x80)>>7)  
           	nrf_gpio_pin_set(SDA2);
        else
            nrf_gpio_pin_clear(SDA2);							
			  dat<<=1;
				nrf_gpio_pin_set(SCL2);
        nrf_delay_us(5);             
				nrf_gpio_pin_clear(SCL2);
        nrf_delay_us(5);            
    }
    sci2a_RecvACK2();            
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


static uint16_t sci2a_recive2(void)         
{
    uint8_t i;
    uint16_t dat = 0;
	  nrf_gpio_pin_set(SDA2); 
    nrf_gpio_cfg_input(SDA,NRF_GPIO_PIN_NOPULL);
    for (i=0; i<16; i++)         
    {
        dat <<= 1;
			  nrf_gpio_pin_set(SCL2);
        nrf_delay_us(5);            
        dat |= nrf_gpio_pin_read(SDA2);    
        nrf_gpio_pin_clear(SCL2);			
        nrf_delay_us(5);            
    }
		nrf_gpio_cfg_output(SDA2);
    return dat;
}


static uint16_t read_sci2a2(uint8_t address)
{
	uint16_t res;
  sci2a_Start2();
  sci2a_SendByte2(address);	
	res = sci2a_recive2();
	sci2a_Stop2();  
	return res;
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





static void getdistance(void)
{
     uint16_t i ;
	  nrf_gpio_cfg_output(SDA);
	  nrf_gpio_cfg_output(SCL);
	  nrf_gpio_pin_set(SDA);
	  nrf_gpio_pin_set(SCL);
		nrf_gpio_cfg_output(SDA2);
	  nrf_gpio_cfg_output(SCL2);
	  nrf_gpio_pin_set(SDA2);
	  nrf_gpio_pin_set(SCL2);
	   if(handle.handle_id==0)
		 {
				 sum = read_sci2a(0x81);
				
				 for(i=0;i<16;i++)
				 {
						if((sum&0x0001)== 0 )
						{
							 touch[i]=1;
							 if(touch_one)
							 {
								touch_min = i ;
								touch_one = 0;
							 }
							 else
							 {
								 touch_max = i;
							 }
						}
						else
							touch[i] = 0 ;
						sum>>=1;
				 }

				 sum = read_sci2a(0x85);
				 //printf("%x\n",sum);
				 for(i=12;i<28;i++)
				 {
						if((sum&0x0001)== 0 )
						{
							 touch[i]=1;
							 if(touch_one)
							 {
								touch_min = i ;
								touch_one = 0;
							 }
							 else
							 {
								 touch_max = i;
							 }
						}
						else
							touch[i] = 0 ;
						sum>>=1;
				 }
				 
				 
				 sum = read_sci2a(0x89);
				 //printf("%x\n",sum);
				 for(i=24;i<40;i++)
				 {
						if((sum&0x0001)== 0 )
						{
							 touch[i]=1;
							 if(touch_one)
							 {
								touch_min = i ;
								touch_one = 0;
							 }
							 else
							 {
								 touch_max = i;
							 }
						}
						else
							touch[i] = 0 ;
						sum>>=1;
				 }
	 }

		 
//-------------------------------------------------//
		else if(handle.handle_id == 1)
		{
				 sum = read_sci2a2(0x81);
				//printf("%x\n",sum);
				 for(i=36;i<52;i++)
				 {
						if((sum&0x0001)== 0 )
						{
							 touch[i]=1;
							 if(touch_one)
							 {
								touch_min = i ;
								touch_one = 0;
							 }
							 else
							 {
								 touch_max = i;
							 }
						}
						else
							touch[i] = 0 ;
						sum>>=1;
				 }

				 sum = read_sci2a2(0x85);
				 //printf("%x\n",sum);
				 for(i=48;i<64;i++)
				 {
						if((sum&0x0001)== 0 )
						{
							 touch[i]=1;
							 if(touch_one)
							 {
								touch_min = i ;
								touch_one = 0;
							 }
							 else
							 {
								 touch_max = i;
							 }
						}
						else
							touch[i] = 0 ;
						sum>>=1;
				 }
				 
				 
				 sum = read_sci2a2(0x89);
				 //printf("%x\n",sum);
				 for(i=60;i<76;i++)
				 {
						if((sum&0x0001)== 0 )
						{
							 touch[i]=1;
							 if(touch_one)
							 {
								touch_min = i ;
								touch_one = 0;
							 }
							 else
							 {
								 touch_max = i;
							 }
						}
						else
							touch[i] = 0 ;
						sum>>=1;
				 }
		 
	 }

		
		else if(handle.handle_id == 2)
		{
		 //printf("touch min %d touch max %d\n",touch_min,touch_max);
		 
		 if(touch_max>0&&touch_min>0)
		 {

			 
       handle.touch_sum = 70 * (touch_max - touch_min);
       if(touch_max-touch_min == 1)
			 {
         handle.touch_sum = 70;
       }
			 else
			 {
					if(touch[touch_max-1]>0&&touch[touch_min+1]>0&&(touch_max-touch_min==2))
					handle.touch_sum = 105;
					else
					{
					if(touch[touch_max-1]>0)
					handle.touch_sum -=70; 
					if(touch[touch_min+1]>0)
					handle.touch_sum -=70;
		      }
		   }			 
			 if(touch_min<40&&touch_max>39)
			 handle.touch_sum += DISTANCE - 70;
			 
     }

	 }
	  handle.handle_id++;
	 if(handle.handle_id>2)
	 {
	   handle.handle_id = 0;
     touch_max = 0; 
		 touch_min = 0; 

		 touch_one = 1;
		 
	 }
	 

}







