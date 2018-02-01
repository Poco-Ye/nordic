#include "app_timer.h"
#include "mushroom.h"
#include "mushroom_notify.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "sci2a.h"
#include "uart.h"
#include "hx711.h"
#include "mlx90615.h"

extern byte dataLow;
extern byte dataHigh;
	
static uint8_t data_i = 0;
static double data1 = 0,data2 = 0;
static uint8_t  symble = 0;
static double weig_zero;
static uint32_t weig1 = 0,weig2 = 0,weig3 =0;
static uint8_t zifu[10] = {'0','1','2','3','4','5','6','7','8','9'};


static void distance_handler(ble_mushroom_t * p_mushroom,sci2a_handle* p_sci2a_handle);
static void weight_handler(ble_mushroom_t * p_mushroom);
static void  temperature_handler(ble_mushroom_t * p_mushroom);



static mushroom_notify_pfn pfn = 
{
   weight_handler,
	 distance_handler,
	 temperature_handler

};


mushroom_handle handle =
{
   &pfn
};



mushroom_handle * mushrooom_init()
{
     return &handle ;
}




static uint8_t change(uint8_t x)
{
   for(int i=0;i<10;i++)
	 {
	    if(x==i)
			return zifu[i];
	 }
   return 0;
}



static void distance_handler(ble_mushroom_t * p_mushroom,sci2a_handle* p_sci2a_handle)
{

    
   uint8_t tx[8];
	 uint16_t  touch_sum ;
   p_sci2a_handle->pfn->__p_getdistance();
	 touch_sum = p_sci2a_handle->touch_sum ;	
	 tx[0] = change(touch_sum/10000);
	 tx[1] = change(touch_sum%10000/1000);
 	 tx[2] = change(touch_sum%1000/100);
	 tx[3] = '.';
	 tx[4] = change(touch_sum%100/10);
	 tx[5] = change(touch_sum%10);
	 tx[6] = 'c';
	 tx[7] = 'm';
	 ble_mushroom_string_send(p_mushroom, tx, 8, p_mushroom->distance_handles.value_handle);
}

static void  temperature_handler(ble_mushroom_t * p_mushroom)
{

   float data;
	 uint16_t data1;
	 uint8_t temp[5];
	 data = getTemperature(MLX90615_AMBIENT_TEMPERATURE);
   data1 = data*100;
	 temp[0]=change(data1/10000);
	 temp[1]=change(data1%10000/1000);
	 temp[2]=change(data1%1000/100);
	 temp[3]=change(data1%100/10);
	 temp[4]=change(data1%10);

   //ReadTem();
   
	 ble_mushroom_string_send(p_mushroom, temp, 5, p_mushroom->weight_handles.value_handle);
}




static void weight_handler(ble_mushroom_t * p_mushroom)
{
	
	uint32_t weig = 0;
	uint8_t tx[8];
	weig = hx711_read()/100; 
  data1 = weig;
	data2 = data2 + data1/10 ; 
	data_i = data_i + 1 ;
  if(data_i > 9)
	{		
		data_i =0;
		data2 = data2/10;
		if(symble < 4)
		{
		  weig_zero = data2;
			weig = 0;
			tx[0] = 'w';
	    tx[1] = 'a';
 	    tx[2] = 'i';
	    tx[3] = 't';
	    tx[4] = '.';
	    tx[5] = '.';
	    tx[6] = '.';
      tx[7] = '.';
		}
		else
		{
      
			 if(data2 >= weig_zero)
			 {
		    weig = (data2 - weig_zero)*48.97 ;
				 
				  if(weig>100)
				 {
			     weig1 = weig2;
           weig2 = weig;
					   if(weig1>weig2)
             {
			           if( weig1-weig2 > 50)
				         {
							    weig3 = weig;
								
							    }
			       }
			       else
			       {
			           if(weig2-weig1 >50)
				         {
							     weig3 = weig;
							   }
			       }
					   tx[0] = '-';
	           tx[1] = change(weig3%100000/10000);
 	           tx[2] = change(weig3%10000/1000);
	           tx[3] = change(weig3%1000/100);
	           tx[4] = change(weig3%100/10);
	           tx[5] = '.';
	           tx[6] = change(weig3%10);
             tx[7] = 'g';
			   }
				 else
				 {
				   weig = 0;
					  tx[0] = change(weig/100000);
	          tx[1] = change(weig%100000/10000);
 	          tx[2] = change(weig%10000/1000);
	          tx[3] = change(weig%1000/100);
	          tx[4] = change(weig%100/10);
	          tx[5] = '.';
	          tx[6] = change(weig%10);
            tx[7] = 'g';
				 }
				 			 
				tx[0] = '-';
	      tx[1] = change(weig%100000/10000);
 	      tx[2] = change(weig%10000/1000);
	      tx[3] = change(weig%1000/100);
	      tx[4] = change(weig%100/10);
	      tx[5] = '.';
	      tx[6] = change(weig%10);
        tx[7] = 'g';
			 }
			 else
			 {
				 weig = (weig_zero -data2)*48.97 ;
				 
				 if(weig>100)
				 {
			     weig1 = weig2;
           weig2 = weig;
					   if(weig1>weig2)
             {
			           if( weig1-weig2 > 50)
				         {
							    weig3 = weig;
								
							    }
			       }
			       else
			       {
			           if(weig2-weig1 >50)
				         {
							     weig3 = weig;
							   }
			       }
					   tx[0] = change(weig3/100000);
	           tx[1] = change(weig3%100000/10000);
 	           tx[2] = change(weig3%10000/1000);
	           tx[3] = change(weig3%1000/100);
	           tx[4] = change(weig3%100/10);
	           tx[5] = '.';
	           tx[6] = change(weig3%10);
             tx[7] = 'g';
			   }
				 else
				 {
				   weig = 0;
					  tx[0] = change(weig/100000);
	          tx[1] = change(weig%100000/10000);
 	          tx[2] = change(weig%10000/1000);
	          tx[3] = change(weig%1000/100);
	          tx[4] = change(weig%100/10);
	          tx[5] = '.';
	          tx[6] = change(weig%10);
            tx[7] = 'g';
				 }
				 			
			 }
			 
		}
		    symble ++;
		    if(symble>100)
				symble = 100 ;
		    data2 = 0;
	
	
	      ble_mushroom_string_send(p_mushroom, tx,8, p_mushroom->weight_handles.value_handle);
	} 
	

}	


