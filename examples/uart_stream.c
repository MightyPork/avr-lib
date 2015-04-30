//
// Example of using UART with stream
//

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>

#include "lib/uart.h"
#include "lib/stream.h"

void main()
{
	// Enable UART with baud rate 9600
	uart_init(9600);

	// Functions from `stream.h` will work with any stream
	// "uart" is a pointer to the UART stream

	put_str(uart, "UART is 1337");
	put_nl(uart);
	put_str_P(uart, PSTR("String from program memory!!!"));
	put_nl(uart);

	put_u16f(uart, 31416, 4);

	while(1);
}
