#include "app_timer.h"
#include "ble_nus.h"
#include "mpu6050.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "sci2a.h"
#include "uart.h"
extern ble_nus_t                        m_nus;                                      /**< Structure to identify the Nordic UART Service. */
extern uint8_t accel[];
extern uint8_t gyro[];




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
	uint16_t sum;
	uint8_t tx[7];
	
  __p_sci2a_handle->pfn->__p_getdistance();
	sum = __p_sci2a_handle->touch_sum ;
	
	//tx[0] = change(sum/10000);
	tx[0] = change(sum%10000/1000);
	tx[1] = change(sum%1000/100);
	tx[2] = '.';
	tx[3] = change(sum%100/10);
	tx[4] = change(sum%10);
	tx[5] = 'c';
  tx[6] = 'm';	
	
	ble_nus_string_send(&m_nus, tx, 7, m_nus.accel_handles.value_handle);

	//ble_nus_string_send(&m_nus, "2", 1, m_nus.gyro_handles.value_handle);
}

static void bmp180_timeout_handler(){
	static uint8_t tempe[2];
	static uint8_t press[4];
	long true_tempe,true_press;


	tempe[0] = (true_tempe >> 8)&0xff;
	tempe[1] = (true_tempe &0xff);
	
	press[0] = (true_press >> 24)&0xff;
	press[1] = (true_press >> 16)&0xff;
	press[2] = (true_press >> 8)&0xff;
	press[3] = (true_press >> 0)&0xff;
	ble_nus_string_send(&m_nus, tempe, 2, m_nus.tempe_handles.value_handle);
	ble_nus_string_send(&m_nus, press, 4, m_nus.press_handles.value_handle);
	
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
	ap3216c_timeout_handler();
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
	return app_timer_start(tempe_press_light_timer, APP_TIMER_TICKS(1500, 0), NULL);
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

