#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "lib/uart.h"


// UART receive interrupt vector
ISR(USART_RX_vect)
{
	uint8_t c = uart_rx();

	// print character and it's code
	uart_putc(c);
	uart_putc(' ');
	uart_putu(c);
	uart_nl();
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
	uart_puts_pgm(PSTR("UART receiver with ISR\r\n"));

	while(1);
}
