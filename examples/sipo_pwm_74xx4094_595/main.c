#include <stdint.h>

#include "lib/iopins.h"

#include "lib/sipo_pwm.h"
#include "sipo_pwm_config.h"

// Example with 3 chained 74HC4094
// Pins configured in sipo_pwm_config.h

const uint8_t rainbow[] = {
	128, 161, 192, 218, 238, 251, 255, 251,
	238, 218, 191, 161, 128, 95, 64, 38, 18,
	5, 1, 5, 18, 38, 65, 95
};

void main()
{
	// Initialize SIPO PWM module (74HC4094)
	spwm_init();

	// Tnitialize the array with default values
	for (uint8_t i = 0; i < SPWM_CHANNELS; i++) {
		spwm_levels[i] = rainbow[i];
	}

	// Animation
	while(1)
	{
		// SHOW THE PWM
		spwm_send();


		// Make a change - effect
		uint8_t first = spwm_levels[0];

		for (uint8_t bit = 0; bit < SPWM_CHANNELS - 1; bit++)
		{
			spwm_levels[bit] = spwm_levels[bit + 1];
		}

		spwm_levels[SPWM_CHANNELS - 1] = first;
	}
}
