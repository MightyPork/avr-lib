#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

#include "lib/iopins.h"
#include "lib/uart.h"

// Demo for ATMega328p on Arduino Pro Mini / Arduino Nano

// Open serial port monitor to see debug messages.

void main()
{
	// also a basic UART example, good for debugging
	uart_init(9600);
	uart_puts("Starting a blinking DEMO\r\n");
	
	as_output(D13); // configure the pin D13

	while(1)
	{
		uart_puts_P(PSTR("FOO\r\n")); // _P variant - string saved in program memory. Saves RAM space.
		
		toggle_pin(D13); // blink the LED

		_delay_ms(500);
	}
}
