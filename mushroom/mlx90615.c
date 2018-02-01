#include "mlx90615.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"

#define SDA3 0   //define the SDA pin (8 is wiringPi pin for raspberry pin 3)
#define SCL3 1   //define the SCL pin (9 is wiringPi pin for raspberry pin 5)

#define i2c_addr 0x5b



uint8_t const I2C_DELAY_USEC = 4;
uint8_t const I2C_READ = 1;
uint8_t const I2C_WRITE = 0;


byte dataLow = 0;
byte dataHigh = 0;
uint8_t read(uint8_t);
bool restart(uint8_t);
bool start(uint8_t);
void stop(void);
bool write(uint8_t);
float getTemperature(int);

/*
int main()
{

    //printf("RESULT : %f\n",getTemperature(MLX90615_AMBIENT_TEMPERATURE));

    return 0;
}
*/

float getTemperature(int Temperature_kind) {


    nrf_gpio_cfg_output(SDA3);
    nrf_gpio_pin_set(SDA3);
    nrf_gpio_cfg_output(SCL3);
    nrf_gpio_pin_set(SCL3);

    //wiringPiI2CSetup(0x5b);
    byte dev = (i2c_addr << 1);  // remain to be seen!

    float celcius = 0.0;

    start(dev | I2C_WRITE);
    write(Temperature_kind);

    // read
    restart(dev | I2C_READ);
    dataLow = read(false);
    dataHigh = read(false);
    read(true);
    stop();
    //This converts high and low bytes together and processes temperature,       MSB is a error bit and is ignored for temps
    double tempFactor = 0.02; // 0.02 degrees per LSB (measurement resolution of the MLX90614)
    double tempData = 0x0000; // zero out the data

    // This masks off the error bit of the high byte, then moves it left 8 bits and adds the low byte.
    tempData = (double)(((dataHigh & 0x007F) << 8) | dataLow);
    tempData = (tempData * tempFactor) - 0.01;

    celcius = (float)(tempData - 273.15);

    return celcius;
    //float fahrenheit = (celcius*1.8) + 32;
 }

uint8_t read(uint8_t last) {
    uint8_t b = 0;
    // make sure pull-up enabled
    nrf_gpio_pin_set(SDA3);
    nrf_gpio_cfg_input(SDA3,NRF_GPIO_PIN_NOPULL);
    // read byte
    for (uint8_t i = 0; i < 8; i++) {
        // don't change this loop unless you verify the change with a scope
        b <<= 1;
        nrf_delay_us(4);
        nrf_gpio_pin_set(SCL3);
        if (nrf_gpio_pin_read(SDA3)) b |= 1;
        nrf_gpio_pin_clear(SCL3);
    }
    // send Ack or Nak
    nrf_gpio_cfg_output(SDA3);
		if(last >0)
		nrf_gpio_pin_set(SDA3);
		else
		nrf_gpio_pin_clear(SDA3);
    nrf_gpio_pin_set(SCL3);
    nrf_delay_us(4);
    nrf_gpio_pin_clear(SCL3);
    nrf_gpio_pin_clear(SDA3);

    return b;
}
//--------------------------------------------------------------------------    ----
/** Issue a restart condition.
*
* \param[in] addressRW I2C address with read/write bit.
*
* \return The value true, 1, for success or false, 0, for failure.
*/
bool restart(uint8_t address) {
    nrf_gpio_pin_set(SDA3);
    nrf_gpio_pin_set(SCL3);
    nrf_delay_us(4);
    return start(address);
}
//------------------------------------------------------------------------------
/** Issue a start condition.
*
* \param[in] addressRW I2C address with read/write bit.
*
* \return The value true, 1, for success or false, 0, for failure.
*/
bool start(uint8_t address) {
    nrf_gpio_pin_clear(SDA3);
    nrf_delay_us(4);
    nrf_gpio_pin_clear(SCL3);
    return write(address);
}
//------------------------------------------------------------------------------
/**  Issue a stop condition. */
void stop(void) {
    nrf_gpio_pin_clear(SDA3);
    nrf_delay_us(4);
    nrf_gpio_pin_set(SCL3);
    nrf_delay_us(4);
    nrf_gpio_pin_set(SDA3);
    nrf_delay_us(4);
}
//------------------------------------------------------------------------------
/**
* Write a byte.
*
* \param[in] data The byte to send.
*
* \return The value true, 1, if the slave returned an Ack or false for Nak.
*/
bool write(uint8_t data) {
    // write byte
    for (uint8_t m = 0X80; m != 0; m >>= 1) {
        // don't change this loop unless you verify the change with a scope
       
			  if((m & data) >0)
				nrf_gpio_pin_set(SDA3);
				else 
				nrf_gpio_pin_clear(SDA3);
        nrf_gpio_pin_set(SCL3);
        nrf_delay_us(4);
        nrf_gpio_pin_clear(SCL3);
    }
    // get Ack or Nak
    nrf_gpio_cfg_input(SDA3,NRF_GPIO_PIN_NOPULL);
    // enable pullup
    nrf_gpio_pin_set(SDA3);
    nrf_gpio_pin_set(SCL3);
    uint8_t rtn = nrf_gpio_pin_read(SDA3);
    nrf_gpio_pin_clear(SCL3);
    nrf_gpio_cfg_output(SDA3);
    nrf_gpio_pin_clear(SDA3);
    return rtn == 0;
}

