#include "app_timer.h"
#include "ble_nus.h"
#include "mpu6050.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "sci2a.h"
#include "uart.h"
#include "hx711.h"


extern ble_nus_t                        m_nus;                                      /**< Structure to identify the Nordic UART Service. */
extern uint8_t accel[];
extern uint8_t gyro[];
uint8_t data_i = 0;
double data1 = 0,data2 = 0;
uint8_t  symble = 0;
double weig_zero;
uint32_t weig1 = 0,weig2 = 0,weig3 =0;


static app_timer_id_t accel_gyro_timer;
static app_timer_id_t tempe_press_light_timer;


uint8_t zifu[10] = {'0','1','2','3','4','5','6','7','8','9'};


extern sci2a_handle* __p_sci2a_handle;

static uint8_t change(uint8_t x)
{
   for(int i=0;i<10;i++)
	 {
	    if(x==i)
			return zifu[i];
	 }
   return 0;
}
static void mpu6050_timeout_handler(){

	//get_mpu6050data();
	uint32_t weig = 0;
	uint8_t tx[8];
	
  //__p_sci2a_handle->pfn->__p_getdistance();
	//sum = __p_sci2a_handle->touch_sum ;
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
		    //weig =(31579 -4.866*(data2/10))*10;
		
		    data2 = 0;
	
	
	      ble_nus_string_send(&m_nus, tx,8, m_nus.accel_handles.value_handle);
	} 
	//ble_nus_string_send(&m_nus, "2", 1, m_nus.gyro_handles.value_handle);
}

static void bmp180_timeout_handler(){
	
	
  uint8_t tx[8];
	uint16_t  touch_sum ;
   __p_sci2a_handle->pfn->__p_getdistance();
	 touch_sum = __p_sci2a_handle->touch_sum ;	
	 tx[0] = change(touch_sum/10000);
	 tx[1] = change(touch_sum%10000/1000);
 	 tx[2] = change(touch_sum%1000/100);
	 tx[3] = '.';
	 tx[4] = change(touch_sum%100/10);
	 tx[5] = change(touch_sum%10);
	 tx[6] = 'c';
	 tx[7] = 'm';
	 ble_nus_string_send(&m_nus, tx, 8, m_nus.gyro_handles.value_handle);
	
}

static void ap3216c_timeout_handler(){
	uint16_t temp;
	static uint8_t al_data[2];

	al_data[0] = (temp>>8)&0xff;
	al_data[1] = temp&0xff;
	ble_nus_string_send(&m_nus, al_data, 2, m_nus.light_handles.value_handle);
}


void accel_gyro_timeout_handler(void *p_context){
	mpu6050_timeout_handler();
}

void tempe_press_light_timeout_handler(void *p_context){
	bmp180_timeout_handler();
	//ap3216c_timeout_handler();
}

uint32_t create_sensor_timer(void){
	uint32_t err_code;
	err_code = app_timer_create(&accel_gyro_timer, APP_TIMER_MODE_REPEATED,accel_gyro_timeout_handler);
	err_code |= app_timer_create(&tempe_press_light_timer, APP_TIMER_MODE_REPEATED,tempe_press_light_timeout_handler);
	
	return err_code;
}


uint32_t accel_gyro_timer_start(void){
	return app_timer_start(accel_gyro_timer, APP_TIMER_TICKS(150, 0), NULL);
}
uint32_t tempe_press_light_timer_start(void){
	return app_timer_start(tempe_press_light_timer, APP_TIMER_TICKS(100, 0), NULL);
}

void sensor_timer_start(void){
	tempe_press_light_timer_start();
	accel_gyro_timer_start();
}

uint32_t close_all_sensor_timer(void){
	uint32_t err;
	err = app_timer_stop_all();
	
	return err;
}

