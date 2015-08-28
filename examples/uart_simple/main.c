//
// Example of basic UART usage
//

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>

#include "lib/uart.h"

void main()
{
	// Enable UART with baud rate 9600
	uart_init(9600);

	// --- some examples of printing functions ---


	// Printing a string
	uart_puts("Simple UART example!\r\n");

	// Use _pgm for strings in program memory
	// (PSTR macro is defined in avr/pgmspace.h)
	uart_puts_P(PSTR("Program memory string example\r\n"));

	// print a char. This is an alias for uart_tx
	uart_putc('a');

	// For printing numbers, please check "uart_stream.c" example


	// --- receive example ---

	// Example of listening for input
	while(1) {

		// If something is received
		if (uart_rx_ready()) {

			// Get the received byte
			char c = uart_rx();

			// Send it back, twice
			uart_tx(c);
			uart_tx(c);
		}
	}
}
