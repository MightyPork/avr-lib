//
// Example of UART with interrupts
//

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>

#include "lib/uart.h"
#include "lib/stream.h"


// UART receive interrupt vector
ISR(USART_RX_vect)
{
	uint8_t c = uart_rx();

	// print character and it's code
	uart_tx(c);
	uart_tx(' ');
	put_u8(uart, c);
	put_nl(uart);
}


void main()
{
	// Set up UART at speed 9600 baud
	uart_init(9600);
	// Enable receive ISR
	uart_isr_rx(1);
	// Enable interrupts
	sei();

	// Greeter string
	put_str_P(uart, PSTR("UART receiver with ISR\r\n"));

	while(1);
}
