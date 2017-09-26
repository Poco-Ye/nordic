/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "twi_master.h"
#include "mpu6050.h"
#include "mpu_reg.h"
#include "nrf_gpio.h"
/*lint ++flb "Enter library region" */

#define ADDRESS_WHO_AM_I (0x75U) //!< WHO_AM_I register identifies the device. Expected value is 0x68.
#define ADDRESS_SIGNAL_PATH_RESET (0x68U) //!< 

static const uint8_t expected_who_am_i = 0x68U; //!< Expected value to get from WHO_AM_I register.
static uint8_t m_device_address; //!< Device address in bits [7:1]



uint8_t test_who_am_i;

bool mpu6050_init()
{   
  bool transfer_succeeded = true;
	uint8_t device_address = 0x69;
	
	mpu_reg_address smplrt_div =SMPLRT_DIV;//赋值0x07
	mpu_reg_address config=CONFIG;//0x06
	mpu_reg_address gyro_config=GYRO_CONFIG;//0x18
	mpu_reg_address accel_config=ACCEL_CONFIG;
	mpu_reg_address pwr_memg_1 =PWR_MEMT_1;//0x00
	
	m_device_address = (uint8_t)(device_address << 1);
  

//	transfer_succeeded &= mpu6050_register_write(pwr_memg_1,WAKEUP);//wakeup
//	
  uint8_t reset_value = 0x04U | 0x02U | 0x01U; // Resets gyro, accelerometer and temperature sensor signal paths
//  transfer_succeeded &= mpu6050_register_write(ADDRESS_SIGNAL_PATH_RESET, reset_value);

//	transfer_succeeded &= mpu6050_register_write(smplrt_div, SAMPLE_50HZ);		//50HZ
//  transfer_succeeded &= mpu6050_register_write(config, DLPF_21HZ);	
//  transfer_succeeded &= mpu6050_register_write(gyro_config, GYRO_FS_2000);//2000  0/s
//  transfer_succeeded &= mpu6050_register_write(accel_config, ACCEL_FS_2g);//2g
//transfer_succeeded &= mpu6050_register_write(pwr_memg_1, CLOCK_SEL_PllGyroX);// pLL x for clock
	
  // Read and verify product ID
 // transfer_succeeded &= mpu6050_verify_product_id();
 
 //用来测试i2cwrite 和i2cread 于上面的注释掉的函数功能相同
	uint8_t tempbuf1[1]={WAKEUP};
	uint8_t tempbuf2[1]={SAMPLE_50HZ};
	uint8_t tempbuf3[1]={DLPF_21HZ};
	uint8_t tempbuf4[1]={GYRO_FS_2000};
	uint8_t tempbuf5[1]={ACCEL_FS_2g};
	uint8_t tempbuf6[1]={CLOCK_SEL_PllGyroX};

	transfer_succeeded &=my_twi_write(device_address,pwr_memg_1,1,tempbuf1);
	transfer_succeeded &=my_twi_write(device_address,ADDRESS_SIGNAL_PATH_RESET,1,&reset_value);
	transfer_succeeded &=my_twi_write(device_address,smplrt_div,1,tempbuf2);
	transfer_succeeded &=my_twi_write(device_address,config,1,tempbuf3);
	transfer_succeeded &=my_twi_write(device_address,gyro_config,1,tempbuf4);
	transfer_succeeded &=my_twi_write(device_address,accel_config,1,tempbuf5);	
	transfer_succeeded &=my_twi_write(device_address,pwr_memg_1,1,tempbuf6);
	
//transfer_succeeded &=mpu6050_verify_product_id();
	uint8_t who = 0;;
	transfer_succeeded &=my_twi_read(device_address, ADDRESS_WHO_AM_I, 1, &who);
	
	if(who!= expected_who_am_i){				
		return false;
	}
	return transfer_succeeded;
	
}

bool mpu6050_verify_product_id(void)
{
  uint8_t who_am_i;
  
  if (mpu6050_register_read(ADDRESS_WHO_AM_I, &who_am_i, 1))
  {
    if (who_am_i != expected_who_am_i)
    {
      return false;
    }
    else
    {
      return true;
    }
  }
  else
  {
    return false;
  }
}

bool mpu6050_register_write(uint8_t register_address, uint8_t value)
{	
	uint8_t w2_data[2];
	
	w2_data[0] = register_address;
	w2_data[1] = value;
  return twi_master_transfer(m_device_address, w2_data, 2, TWI_ISSUE_STOP);	  
}

bool mpu6050_register_read(uint8_t register_address, uint8_t *destination, uint8_t number_of_bytes)
{
  bool transfer_succeeded;
  transfer_succeeded = twi_master_transfer(m_device_address, &register_address, 1, TWI_DONT_ISSUE_STOP);
  transfer_succeeded &= twi_master_transfer(m_device_address|TWI_READ_BIT, destination, number_of_bytes, TWI_ISSUE_STOP);
  return transfer_succeeded;
}



uint8_t accel[6];
uint8_t gyro[6];


bool get_mpu6050data(void){
	bool err;
	err = mpu6050_register_read(ACCEL_XOUT_H,accel,6);
	err &= mpu6050_register_read(GYRO_XOUT_H,gyro,6);
	return err;
//	mpu6050_register_read(TEMP_OUT_H,tmp,2);	
//	
//	accel_x=((((((short)(accel[0]<<8))|accel[1]))*1.0)*2)/32768;
//	accel_y=((((((short)(accel[2]<<8))|accel[3]))*1.0)*2)/32768;
//	accel_z=((((((short)(accel[4]<<8))|accel[5]))*1.0)*2)/32768;
//	
//	gyro_x=((((((short)((gyro[0])<<8))|gyro[1]))*1.0)*2000)/32768;
//	gyro_y=((((((short)((gyro[2])<<8))|gyro[3]))*1.0)*2000)/32768;
//	gyro_z=((((((short)((gyro[4])<<8))|gyro[5]))*1.0)*2000)/32768;
//	

//	printf("accel.....:");
//	printf("%f\t",accel_x);printf("%f\t",accel_y);printf("%f\t\r\n",accel_z);

//	printf("gyro.....:");
//	printf("%f\t", gyro_x);printf("%f\t", gyro_y);printf("%f\t\r\n", gyro_z);
}



/*lint --flb "Leave library region" */ 
