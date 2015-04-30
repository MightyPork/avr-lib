//
// Basic Sonar example
//

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

#include "lib/uart.h"
#include "lib/arduino_pins.h"
#include "lib/sonar.h"



// All PCINT vectors must call sonar_handle_pci
ISR(PCINT0_vect)
{
	if (sonar_handle_pci()) return;

	// Here you can do your other stuff
}

// This way, they are linked to PCINT0
// so we can avoid code duplication
ISR(PCINT1_vect, ISR_ALIASOF(PCINT0_vect));
ISR(PCINT2_vect, ISR_ALIASOF(PCINT0_vect));



// Timer overflow - if sonar has a timeout -> 0xFFFF in result.
ISR(TIMER1_OVF_vect)
{
	if (sonar_handle_t1ovf()) return;

	// ...
}




void main()
{
	// Init UART communication
	uart_init(9600);


	// Create and init Sonar instance
	sonar_t so;

	// Args: pointer to your sonar_t, and 2 pin aliases - Trig & Echo
	sonar_init(&so, A0, A1);

	// You can have more than one sonar connected, but only one can measure at a time


	// Allow interrupts
	sei();


	while(1) {

		// Measure
		sonar_start(&so);

		while(sonar_busy()) {
			// Sonar is asynchronous
			// Here we just wait for it to complete
		}

		int16_t res = sonar_result();



		// Print
		if (res < 0) {
			uart_puts("NO OBSTACLES\r\n");
		} else {
			uart_puti(res, 1); // one decimal place
			uart_puts(" cm\r\n");
		}


		// a delay...
		_delay_ms(500);
	}
}
