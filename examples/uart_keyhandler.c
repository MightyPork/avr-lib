#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "lib/uart.h"
#include "lib/stream.h"

//
// Example of asynchronous UART key handling
//
// It recognizes special keys like arrows and some F keys,
// check the header file for full list.
//
// You need uart_ansi for this.
//

ISR(USART_RX_vect)
{
	char c = uart_rx();
	vt_handle_key(c);
	// we can do anything further with the received byte
}


// Our custom key handler function
void key_handler(uint8_t code, bool special)
{
	put_str_P(uart, special ? PSTR("Special: ") : PSTR("Char: "));
	put_char(uart, code); // the actual character
	put_char(uart, ' '); // space
	put_u8(uart, code); // as number
	put_nl(uart); // crlf
}


void main()
{
	uart_init(9600);  // set BAUD-rate
	uart_isr_rx(1);  // enable the ISR
	vt_init();  // initialize uart_ansi library

	vt_set_key_handler(&key_handler);  // assign our custom handler

	sei();

	put_str_P(uart, PSTR("UART key handler test!\r\n"));

	while(1);
}
