/* Config for SIPO PWM */

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
