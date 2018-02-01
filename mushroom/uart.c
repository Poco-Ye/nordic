#include "uart.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "app_uart.h"
#include "app_error.h"
#include "nrf_delay.h"
#include "nrf.h"
#include "bsp.h"



void UART_Init(void)
{
 NRF_UART0->PSELRXD = RX_PIN_NUMBER;
 NRF_UART0->PSELTXD = TX_PIN_NUMBER;
	
 nrf_gpio_cfg_output(TX_PIN_NUMBER );

	
 nrf_gpio_cfg_input(RX_PIN_NUMBER, NRF_GPIO_PIN_NOPULL);

	
 
 NRF_UART0->BAUDRATE = UART_BAUDRATE_BAUDRATE_Baud115200<<UART_BAUDRATE_BAUDRATE_Pos;

 NRF_UART0->ENABLE = UART_ENABLE_ENABLE_Enabled << UART_ENABLE_ENABLE_Pos;
 NRF_UART0->TASKS_STARTRX = 1;
 NRF_UART0->TASKS_STARTTX = 1;
 NRF_UART0->EVENTS_RXDRDY = 0;
 NRF_UART0->EVENTS_TXDRDY = 0;
}



void UART_Put(uint8_t data)
{
 NRF_UART0->EVENTS_TXDRDY = 0;
 NRF_UART0->TXD = data; 
 while(NRF_UART0->EVENTS_TXDRDY == 0)
 {
  ;
 }
}



uint8_t UART_Get(void)
{
  while (NRF_UART0->EVENTS_RXDRDY != 1)
  {
    // Wait for RXD data to be received
  }
  
  NRF_UART0->EVENTS_RXDRDY = 0;
  return (uint8_t)NRF_UART0->RXD;
}





