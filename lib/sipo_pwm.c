#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

#include "sipo_pwm.h"
#include "iopins.h"
#include "sipo_pwm_config.h"


/* -------- SIPO PWM MODULE ---------- */

/** Buffer for sending bits to SIPO */
bool _buff[SPWM_CHANNELS];

uint8_t spwm_levels[SPWM_CHANNELS];


/** Send _buff to SIPO */
void _send_buffer()
{
	for (int8_t i = SPWM_CHANNELS - 1; i >= 0; i--)
	{
		#if (SPWM_INVERT)
		set_pin(SPWM_DATA, !_buff[i]); /* Common anode */
		#else
		set_pin(SPWM_DATA, _buff[i]);  /* Common cathode */
		#endif

		// send a CLK pulse
		pin_high(SPWM_CLK);
		pin_low(SPWM_CLK);
	}

	// send a STR pulse
	pin_high(SPWM_STR);
	pin_low(SPWM_STR);
}


void spwm_init()
{
	// Pin directions
	as_output(SPWM_CLK);
	as_output(SPWM_STR);
	as_output(SPWM_DATA);

	// Initial states
	pin_low(SPWM_CLK);
	pin_low(SPWM_STR);
}


/**
 * Display PWM channels.
 * This could be called in a Timer ISR.
 */
void spwm_send()
{
	// Set all bits to 1 (if their PWM level is 0, set to 0)
	for (uint8_t bit = 0; bit < SPWM_CHANNELS; bit++)
	{
		_buff[bit] = (bool) spwm_levels[bit];
	}

	// Show initial state
	_send_buffer();

	// For each PWM level...
	for (uint16_t pwm = 0; pwm < SPWM_COLOR_DEPTH; pwm++)
	{
		// Turn OFF bits that are below the level
		for (uint8_t bit = 0; bit < SPWM_CHANNELS; bit++)
		{
			if (spwm_levels[bit] < pwm)
			{
				_buff[bit] = 0;
			}
		}

		// And show...
		_send_buffer();
	}
}
