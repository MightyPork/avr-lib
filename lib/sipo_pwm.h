#pragma once

// --- SIPO PWM Module ---
//
// SIPO = Serial IN, Parallel OUT
//
// This module lets you use SIPO outputs as a "software PWM".
//
// Tested on 74HC4094,
// should also work on 74HC595 (may need some small changes)

#include <stdint.h>

// Your file with configs
#include "sipo_pwm_config.h"
/*
	// PWM pin aliases
	#define SPWM_STR  D2
	#define SPWM_CLK  D3
	#define SPWM_DATA D4

	// Number of PWM levels (color depth)
	#define SPWM_COLOR_DEPTH 256

	// Number of SIPO channels
	#define SPWM_CHANNELS 24

	// Invert outputs (for Common Anode LEDs)
	#define SPWM_INVERT 1
*/


// Array for setting PWM levels (PWM_CHANNELS-long)
extern uint8_t spwm_levels[SPWM_CHANNELS];


/** Configure output pins etc */
void spwm_init();


/** Display PWM channels.
 * This could be called in a Timer ISR.
 */
void spwm_send();
