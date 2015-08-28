#pragma once

// --- SIPO PWM Module ---
//
// SIPO = shift register with paralel output.
//
// This module lets you use SIPO outputs as a "software PWM".
//
// Tested to work on 74hc4094 and 74hc595

#include <stdint.h>

// Your file with configs
#include "sipo_pwm_config.h"
/*
	// --- PWM pin aliases ---
	
	// Store signal
	#define SPWM_STR  D2
	// Shift/clock signal
	#define SPWM_CLK  D3
	// Data signal
	#define SPWM_DATA D4
	
	// --- Other settings ---
	
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


/** Perform one PWM cycle.
 * This should be called in a Timer ISR or a loop.
 */
void spwm_send();
