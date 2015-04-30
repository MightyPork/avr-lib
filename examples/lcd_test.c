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

	// Example of using the LCD stream
	put_u32f(lcd, 271828, 5);

	// Print a string
	lcd_puts(" Hello\r\n");

	// Use _pgm for strings in program memory
	// Here we print string at position xy
	lcd_puts_P(PSTR("String from PGM "));

	// declare custom glyph
	lcd_glyph_P(0, glyph);

	// and show it
	lcd_putc(0);

	lcd_puts(" !");

	// enable blink/bar cursor type
	lcd_cursor(CURSOR_BOTH);

	// print at given position
	lcd_puts_xy(5, 2, "Third line?");
	lcd_puts_xy(3, 3, "Fourth line!");

	// end
	while(1);
}
