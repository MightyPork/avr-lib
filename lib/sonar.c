#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

#include "pins.h"
#include "sonar.h"

// Currently measured sonar
sonar_t* _sonar_active_so;

// Flag that measurement is in progress
volatile bool sonar_busy;

// Result of last measurement, in millimeters
volatile int16_t sonar_result;


void _sonar_init_do(sonar_t* so, PORT_P port, uint8_t ntx, PORT_P pin, uint8_t nrx)
{
	so->port = port;
	so->ntx = ntx;
	so->pin = pin;
	so->nrx = nrx;

	switch((const uint16_t) pin) {
		case (const uint16_t)&PINB:
			so->bank = 0;
			break;
		case (const uint16_t)&PINC:
			so->bank = 1;
			break;
		case (const uint16_t)&PIND:
			so->bank = 2;
			break;
	}
}


/**
 * Start sonar measurement
 * Interrupts must be enabled
 * TIMER 1 will be used for the async measurement
 * Timer 1 overflow and Pin Change interrupts must invoke Sonar handlers.
 */
bool sonar_start(sonar_t* so)
{
	if (sonar_busy) return false;

	_sonar_active_so = so;

	sonar_busy = true;

	// make sure the timer is stopped (set clock to NONE)
	TCCR1B = 0;

	// Timer overflow interrupt enable
	// We'll stop measuring on overflow
	TIMSK1 |= (1 << TOIE1);

	// Clear the timer value
	TCNT1 = 0;

	// Set up pin change interrupt mask for the RX pin
	switch(so->bank) {
		case 0:
			PCMSK0 |= (1 << (so->nrx));
			break;
		case 1:
			PCMSK1 |= (1 << (so->nrx));
			break;
		case 2:
			PCMSK2 |= (1 << (so->nrx));
			break;
	}

	// send positive pulse
	*(so->port) |= (1 << so->ntx);
	_delay_us(_SNR_TRIG_TIME);
	*(so->port) &= ~(1 << so->ntx);

	// Wait for start of response
	while ( (*(so->pin) & (1 << so->nrx)) == 0 );

	// Set timer clock source: F_CPU / 8 (0.5 us resolution)
	TCCR1B = (0b010 << CS10);

	// Enable pin change interrupt
	PCICR |= (1 << (so->bank));

	return true;
}


/** Stop the timer */
void _sonar_stop()
{
	// stop timer
	TCCR1B = 0;

	// Disable RX pin interrupt mask
	switch(_sonar_active_so->bank) {
		case 0:
			PCMSK0 &= ~(1 << (_sonar_active_so->nrx));
			break;
		case 1:
			PCMSK1 &= ~(1 << (_sonar_active_so->nrx));
			break;
		case 2:
			PCMSK2 &= ~(1 << (_sonar_active_so->nrx));
			break;
	}

	// Disable timer1 overflow interrupt
	TIMSK1 &= ~(1 << TOIE1);

	sonar_busy = false;
}


/** Handle TIMER1_OVF (returns true if consumed) */
inline bool sonar_handle_t1ovf()
{
	if (!sonar_busy) return false; // nothing

	sonar_result = -1;
	_sonar_stop();

	return true;
}


/** Handle pin change interrupt (returns true if consumed) */
inline bool sonar_handle_pci()
{
	if (!sonar_busy) {
		return false; // nothing
	}

	if (*(_sonar_active_so->pin) & (1 << _sonar_active_so->nrx)) {
		// rx is high, not our pin change event
		return false;
	}

	uint64_t x = TCNT1;
	x /= _SNR_DIV_CONST;
	x *= 100000000L;
	x /= F_CPU;
	sonar_result = (int16_t) x;

	// no obstacle
	if (sonar_result > _SNR_MAX_DIST) sonar_result = -1;

	_sonar_stop();

	return true;
}
