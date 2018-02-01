#ifndef __MLX90615_H
#define __MLX90615_H
#include "nrf_gpio.h"
#include "nrf_delay.h"

uint8_t read(uint8_t);
bool restart(uint8_t);
bool start(uint8_t);
void stop(void);
bool write(uint8_t);
float getTemperature(int);
typedef unsigned char byte;
#define MLX90615_OBJECT_TEMPERATURE     0x27
#define MLX90615_AMBIENT_TEMPERATURE    0x26

#endif
