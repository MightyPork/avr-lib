#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "lib/uart.h"

//
// This example receives asynchronously characters over UART
// and prints them back encrypted with ROT13 cipher (the one used for
// hints in GeoCaching)
//


// Echo, but with ROT13.
ISR(USART_RX_vect)
{
	char c = uart_rx();

	if (c == 13) {
		// print CRLF on enter
		uart_tx(10);
		uart_tx(13);
		return;
	}

	// do the ROT13
	if ((c >= 'a' && c <= 'm') || (c >= 'A' && c <= 'M')) {
			c += 13;
	} else
	if ((c >= 'n' && c <= 'z') || (c >= 'N' && c <= 'Z')) {
			c -= 13;
	}

	// Print the result
	uart_tx(c);
}


void main()
{
	uart_init(9600);
	uart_isr_rx(1);
	sei();

	uart_puts_P(PSTR("ROT13 UART echo example\r\n"));

	while(1);
}
