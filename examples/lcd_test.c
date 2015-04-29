//
// Example of basic LCD usage
//

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "lib/calc.h"
#include "lib/lcd.h"

// Bytes for custom glyph

// Here's a good glyph generator you can use:
//     http://omerk.github.io/lcdchargen/

const uint8_t glyph[] PROGMEM = {
	0b00000,
	0b00111,
	0b01101,
	0b11101,
	0b10001,
	0b10001,
	0b10001,
	0b11111
};


void main()
{
	lcd_init();

	// Define custom glyph at character 0
	lcd_glyph_pgm(0, glyph);

	// After writing a glyph, the address is changed
	// So we need to fix that:
	lcd_xy(0, 0);

	// Print a string
	lcd_puts("Hello");

	// Use _pgm for strings in program memory
	// Here we print string at position xy
	lcd_puts_xy_pgm(0, 1, PSTR("String from PGM "));

	// print the custom glyph
	lcd_putc(0);

	// enable blink/bar cursor type
	lcd_cursor(CURSOR_BOTH);

	lcd_puts_xy(5, 2, "Third line?");
	lcd_puts_xy(3, 3, "Fourth line!");

	// end
	while(1);
}
