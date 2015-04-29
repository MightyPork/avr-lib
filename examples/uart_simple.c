//
// Example of basic UART usage
//

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>

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
	uart_puts_pgm(PSTR("Program memory string example\r\n"));

	// print a char. This is an alias for uart_tx
	uart_putc('a');

	// print a newline
	uart_nl();

	// Printing numbers is also easy
	// Look in `uart.h` for more info
	uart_putn(123);
	uart_nl();

	// Print int as float (adds decimal point at 4th place from the end)
	uart_puti(31415, 4);
	uart_nl();


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
