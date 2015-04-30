#pragma once

//
// Utilities for working with the HC-SR04 ultrasonic sensor
// Can be easily modified to work with other similar modules
//
// It's required that you call the sonar_handle_* functions from your ISRs
// See example program for more info.
//

#include <stdint.h>
#include <stdbool.h>

#include "lib/pins.h"

// Calib constant for the module
// CM = uS / _DIV_CONST
#define _SNR_DIV_CONST 58

// Max module distance in MM
#define _SNR_MAX_DIST 4000

// Trigger time in uS
#define _SNR_TRIG_TIME 10


// Sonar data object
typedef struct {
	PORT_P port; // Tx PORT
	uint8_t ntx; // Tx bit number
	PORT_P pin;  // Rx PIN
	uint8_t nrx; // Rx bit number
	uint8_t bank; // Rx PCINT bank
} sonar_t;


// Create a Sonar port
// Args: sonar_t* so, Trig pin, Echo pin
#define sonar_init(so, trig, echo) do { \
	as_output(io_pack(trig)); \
	as_input_pu(io_pack(echo)); \
	_sonar_init_do(so, &io2port(io_pack(trig)), io2n(io_pack(trig)), &io2pin(io_pack(echo)), io2n(io_pack(echo))); \
} while(0)

// private, in header because of the macro.
void _sonar_init_do(sonar_t* so, PORT_P port, uint8_t ntx, PORT_P pin, uint8_t nrx);


/** Check if sonar is busy */
bool sonar_busy();


/** Get result of last measurement, in millimeters. Returns -1 if no obstacle detected */
int16_t sonar_result();


/**
 * Start sonar measurement
 * Interrupts must be enabled
 * TIMER 1 will be used for the async measurement
 */
bool sonar_start(sonar_t* so);


/** Handle TIMER1_OVF (returns true if consumed) */
bool sonar_handle_t1ovf();


/** Handle pin change interrupt (returns true if consumed) */
bool sonar_handle_pci();
