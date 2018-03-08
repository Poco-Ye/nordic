#include "app_timer.h"
#include "mushroom.h"
#include "mushroom_notify.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "sci2a.h"
#include "uart.h"
#include "hx711.h"
#include "mlx90615.h"
#include "hc595.h"
#include "oled.h"

extern byte dataLow;
extern byte dataHigh;
extern uint8_t temp[16];

extern uint8_t  touch_min_wait,touch_max_wait;

uint32_t  niao_bu=0,zhong_biao=0,he_nai=0;
uint8_t receive_tmp[9];
uint8_t receive_tmp_wait[10];
uint8_t esp8266_tmp[20];
uint8_t tx_weight[10];
uint8_t tx_distance[8];
static uint8_t data_i = 0;
static double data1 = 0,data2 = 0;
static uint8_t  symble = 0;
static double weig_zero;
static uint32_t weig1 = 0,weig2 = 0,weig3 =0;
static uint8_t zifu[10] = {'0','1','2','3','4','5','6','7','8','9'};
uint16_t  touch_sum ;
static void distance_handler(ble_mushroom_t * p_mushroom,sci2a_handle* p_sci2a_handle);
static void weight_handler(ble_mushroom_t * p_mushroom);
static void  temperature_handler(ble_mushroom_t * p_mushroom);
static void  sumled_handler(ble_mushroom_t * p_mushroom);
static void  uart_handler(ble_mushroom_t * p_mushroom);
static uint8_t test_i = 0;

static mushroom_notify_pfn pfn = 
{
   weight_handler,
	 distance_handler,
	 temperature_handler,
   sumled_handler,
	 uart_handler
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

    
   
	 
   p_sci2a_handle->pfn->__p_getdistance();
	 touch_sum = p_sci2a_handle->touch_sum ;

	
	
	 tx_distance[0] = change(touch_sum/10000);
	 tx_distance[1] = change(touch_sum%10000/1000);
 	 tx_distance[2] = change(touch_sum%1000/100);
	 tx_distance[3] = '.';
	 tx_distance[4] = change(touch_sum%100/10);
	 tx_distance[5] = change(touch_sum%10);
	 tx_distance[6] = 'c';
	 tx_distance[7] = 'm';
	 ble_mushroom_string_send(p_mushroom, tx_distance, 8, p_mushroom->distance_handles.value_handle);
	 
	 

	
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
   //hc595_drive(0Xff,0X01);
	// ble_mushroom_string_send(p_mushroom, temp, 5, p_mushroom->weight_handles.value_handle);
}



static void  sumled_handler(ble_mushroom_t * p_mushroom)
{
/*   if(sum_i==0)
	 hc595_drive_16(0x01,temp[touch_sum%10000/1000]);
	 else if(sum_i==1)
	 hc595_drive_16(0x02,temp[touch_sum%1000/100]&0x7f);
	 else if(sum_i==2)
	 hc595_drive_16(0x04,temp[touch_sum%100/10]);
	 else if(sum_i==3)
	 hc595_drive_16(0x08,temp[touch_sum%10]);
	 sum_i++;
	 if(sum_i>3)sum_i=0;  
*/
	
	uint8_t temp[10],i=0;
	uint16_t adc_result;
	NRF_ADC->ENABLE = 1;
	NRF_ADC->TASKS_START =1;
	//nrf_delay_ms(1);
	adc_result = NRF_ADC->RESULT;
	if(adc_result<700)
	{
	 //temp[0]=change(adc_result/10000);
	 //temp[1]=change(adc_result%10000/1000);
	 //temp[2]=change(adc_result%1000/100);
	 //temp[3]=change(adc_result%100/10);
	 //temp[4]=change(adc_result%10);
	 //ble_mushroom_string_send(p_mushroom, temp, 5, p_mushroom->weight_handles.value_handle);
		
		if(adc_result>60&&adc_result<100)
		{
			niao_bu++;
	
			temp[0]='n';
		
			temp[1]='a';
			temp[2]='p';
			temp[3]=':';
		  temp[4]=change(niao_bu%1000/100);
		  temp[5]=change(niao_bu%100/10);
		  temp[6]=change(niao_bu%10);
			temp[7]='\0';
			OLED_ShowString(2,0,temp);
		}			
		if(adc_result>140&&adc_result<170)
		{
			niao_bu--;
				temp[0]='n';
		
			temp[1]='a';
			temp[2]='p';
			temp[3]=':';
		  temp[4]=change(niao_bu%1000/100);
		  temp[5]=change(niao_bu%100/10);
		  temp[6]=change(niao_bu%10);
			temp[7]='\0';
			OLED_ShowString(2,0,temp);
		}
		if(adc_result>280&&adc_result<320)
		{	
		he_nai++;
			temp[0]='m';
		
			temp[1]='i';
			temp[2]='l';
			temp[3]='k';
			temp[4]=':';
		  temp[5]=change(he_nai%1000/100);
		  temp[6]=change(he_nai%100/10);
		  temp[7]=change(he_nai%10);
			temp[8]='\0';
	  OLED_ShowString(2,2,temp);
		}
		
		if(adc_result>420&&adc_result<480)
		{
		he_nai--;
									temp[0]='m';
		
			temp[1]='i';
			temp[2]='l';
			temp[3]='k';
			temp[4]=':';
		  temp[5]=change(he_nai%1000/100);
		  temp[6]=change(he_nai%100/10);
		  temp[7]=change(he_nai%10);
			temp[8]='\0';
	  OLED_ShowString(2,2,temp);
		}

    		
	}
	
   ble_mushroom_string_send(p_mushroom, receive_tmp_wait,7, p_mushroom->distance_handles.value_handle);
}


static void  uart_handler(ble_mushroom_t * p_mushroom)
{
	 int i;
/*   esp8266_tmp[0] = 'h';
	 esp8266_tmp[1] = 'e';
	 esp8266_tmp[2] = 'h';
	 esp8266_tmp[3] = 'e';

	 esp8266_tmp[4] = touch_min_wait;
	 esp8266_tmp[5] = touch_max_wait;
	 esp8266_tmp[6] = weig3/256;
	 esp8266_tmp[7] = weig3%256;
	 //esp8266_tmp[9] = change(touch_min_wait/100);
	 //esp8266_tmp[10] = change(touch_min_wait%100/10);
	 //esp8266_tmp[11] = change(touch_min_wait%10);
	 //esp8266_tmp[12] = change(touch_max_wait/100);
	 //esp8266_tmp[13] = change(touch_max_wait%100/10);
	 //esp8266_tmp[14] = change(touch_max_wait%10);
   esp8266_tmp[8]= '\0';
   //printf("%s",esp8266_tmp);
	for(i=0;i<9;i++)
	{
	  UART_Put(esp8266_tmp[i]);
	}
*/

          while(UART_Get()=='e')
					for(i=0;i<7;i++)
					{
						receive_tmp[i]=UART_Get();
					}
					receive_tmp[7] = '\0';
				




}




static void weight_handler(ble_mushroom_t * p_mushroom)
{
	
	uint32_t weig = 0,i;

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
			tx_weight[0] = 'w';
	    tx_weight[1] = 'a';
 	    tx_weight[2] = 'i';
	    tx_weight[3] = 't';
	    tx_weight[4] = '.';
	    tx_weight[5] = '.';
	    tx_weight[6] = '.';
      tx_weight[7] = '.';
		}
		else
		{
				
				 if(data2 >= weig_zero)
				 {
					weig = (data2 - weig_zero)*4.897 ;
					 
						if(weig>10)
					 {
						 weig1 = weig2;
						 weig2 = weig;
							 if(weig1>weig2)
							 {
									 if( weig1-weig2 > 5)
									 {
										weig3 = weig;
									
										}
							 }
							 else
							 {
									 if(weig2-weig1 >5)
									 {
										 weig3 = weig;
									 }
							 }
							 
							 tx_weight[0] = change(weig3/10000);
							 tx_weight[1] = change(weig3%10000/1000);
							 tx_weight[2] = '.';
							 tx_weight[3] = change(weig3%1000/100);
							 tx_weight[4] = change(weig3%100/10);
							 tx_weight[5] = change(weig3%10);
							 tx_weight[6] = 'k';
							 tx_weight[7] = 'g';
							 tx_weight[8] = ' ';
							 tx_weight[9] = '\0';
					 }
					 else
					 {
						  weig = 0;
							tx_weight[0] = change(weig/10000);
							tx_weight[1] = change(weig%10000/1000);
						  tx_weight[2] = '.';
							tx_weight[3] = change(weig%1000/100);
							tx_weight[4] = change(weig%100/10);
							tx_weight[5] = change(weig%10);
						  tx_weight[6] = 'k';
							tx_weight[7] = 'g';
						  tx_weight[8] = ' ';
						  tx_weight[9] = '\0';
					 }
								 
			 }
			 else
			 {
				 weig = (weig_zero -data2)*4.897 ;
				 
				 if(weig>10)
				 {
			     weig1 = weig2;
           weig2 = weig;
					   if(weig1>weig2)
             {
			           if( weig1-weig2 > 5)
				         {
							    weig3 = weig;
								
							    }
			       }
			       else
			       {
			           if(weig2-weig1 >5)
				         {
							     weig3 = weig;
							   }
			       }

					  	tx_weight[0] = '-';
							tx_weight[1] = change(weig/10000);
							tx_weight[2] = change(weig%10000/1000);
						  tx_weight[3] = '.';
							tx_weight[4] = change(weig%1000/100);
							tx_weight[5] = change(weig%100/10);
							tx_weight[6] = change(weig%10);
              tx_weight[7] = 'k';
					    tx_weight[8] = 'g';
						  tx_weight[9] = '\0';
			   }
				 else
				 {
				      weig = 0;
							tx_weight[0] = change(weig/10000);
							tx_weight[1] = change(weig%10000/1000);
						  tx_weight[2] = '.';
							tx_weight[3] = change(weig%1000/100);
							tx_weight[4] = change(weig%100/10);
							tx_weight[5] = change(weig%10);
              tx_weight[6] = 'k';
					    tx_weight[7] = 'g';
					    tx_weight[8] = ' ';
					    tx_weight[9] = '\0';
				 }
				 			
			 }
			 
		}
		    symble ++;
		    if(symble>100)
				symble = 100 ;
		    data2 = 0;
				//OLED_ShowString(0,6,"weight:");
				OLED_ShowString(2,4,"wei:");
	      OLED_ShowString(40,4,tx_weight);
				if(receive_tmp[2]=='.')
				{
						for(i=0;i<5;i++)
					{
						 receive_tmp_wait[i]=receive_tmp[i];
			 		}
					receive_tmp_wait[5]='c';
					receive_tmp_wait[6]='m';
					receive_tmp_wait[7]= '\0';
			  }
				OLED_ShowString(2,6,"dis:");
				OLED_ShowString(40,6,receive_tmp_wait);
	      ble_mushroom_string_send(p_mushroom, tx_weight,8, p_mushroom->weight_handles.value_handle);
				
	} 
	

}	


