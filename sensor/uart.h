#ifndef __UART_H__
#define __UART_H__


#include <stdint.h>



void UART_Init(void);
void UART_Put(uint8_t data);
uint8_t UART_Get(void);


#endif


