//
// Basic Sonar example
//

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

#include "lib/arduino_pins.h"
#include "lib/sonar.h"
#include "lib/stream.h"
#include "lib/lcd.h"

// Sonar interrupts

ISR(PCINT0_vect)
{
	if (sonar_handle_pci()) return;
}

ISR(PCINT1_vect, ISR_ALIASOF(PCINT0_vect));
ISR(PCINT2_vect, ISR_ALIASOF(PCINT0_vect));

ISR(TIMER1_OVF_vect)
{
	if (sonar_handle_t1ovf()) return;
}



void main()
{
	lcd_init();

	// Init sonar
	sonar_t so;
	sonar_init(&so, A0, A1);

	sei();


	while(1) {
		// Measure
		sonar_start(&so);
		while(sonar_busy);

		int16_t res = sonar_result;

		// Print
		lcd_clear();
		if (sonar_result < 0) {
			put_str(lcd, "---");
		} else {
			put_i16f(lcd, res, 1); // one decimal place
			put_str(lcd, " cm");
		}

		// a delay...
		_delay_ms(200);
	}
}
