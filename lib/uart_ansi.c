#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <stdint.h>

#include "uart.h"
#include "uart_ansi.h"

void _vt_apply_style();
void _vt_reset_attribs_do();
void _vt_style_do();
void _vt_color_do();


void vt_goto(uint8_t x, uint8_t y)
{
	uart_putc(27);
	uart_putc('[');
	uart_put8u(x);
	uart_putc(';');
	uart_put8u(y);
	uart_putc('H');
}


void vt_goto_x(uint8_t x)
{
	uart_putc(27);
	uart_putc('[');
	uart_put8u(x);
	uart_putc('`');
}


void vt_goto_y(uint8_t y)
{
	uart_putc(27);
	uart_putc('[');
	uart_put8u(y);
	uart_putc('d');
}


void vt_move(int8_t x, int8_t y)
{
	vt_move_x(x);
	vt_move_y(y);
}


void vt_move_x(int8_t x)
{
	if (x < 0) {
		vt_left(-x);
	} else {
		vt_right(x);
	}
}


void vt_move_y(int8_t y)
{
	if (y < 0) {
		vt_up(-y);
	} else {
		vt_down(y);
	}
}


void vt_up(uint8_t y)
{
	if (y == 0) return;
	uart_putc(27);
	uart_putc('[');
	uart_put8u(y);
	uart_putc('A');
}


void vt_down(uint8_t y)
{
	if (y == 0) return;
	uart_putc(27);
	uart_putc('[');
	uart_put8u(y);
	uart_putc('B');
}


void vt_left(uint8_t x)
{
	if (x == 0) return;
	uart_putc(27);
	uart_putc('[');
	uart_put8u(x);
	uart_putc('D');
}


void vt_right(uint8_t x)
{
	if (x == 0) return;
	uart_putc(27);
	uart_putc('[');
	uart_put8u(x);
	uart_putc('C');
}


void vt_scroll(int8_t y)
{
	while (y < 0) {
		uart_putc(27);
		uart_putc('D'); // up
		y++;
	}

	while (y > 0) {
		uart_putc(27);
		uart_putc('M'); // down
		y--;
	}
}


void vt_scroll_set(uint8_t from, uint8_t to)
{
	uart_putc(27);
	uart_putc('[');
	uart_put8u(from);
	uart_putc(';');
	uart_put8u(to);
	uart_putc('r');
}


void vt_scroll_reset()
{
	uart_putc(27);
	uart_putc('[');
	uart_putc('r');
}



typedef struct {
	uint8_t flags;
	uint8_t fg;
	uint8_t bg;
} vt_style_t;

vt_style_t saved_style;
vt_style_t current_style;

void vt_save()
{
	uart_puts_pgm(PSTR("\x1B[s"));

	saved_style = current_style;
}


void vt_restore()
{
	uart_puts_pgm(PSTR("\x1B[u"));

	current_style = saved_style;
}


/** Disable all text attributes (excluding color) */
void vt_attr_reset()
{
	current_style.flags = 0;

	_vt_reset_attribs_do();
	_vt_apply_style();
}


/** Set color to white on black */
void vt_color_reset()
{
	current_style.fg = VT_WHITE;
	current_style.bg = VT_BLACK;

	_vt_color_do();
}


/** Enable or disable a text attribute */
void vt_attr(uint8_t attribute, bool on)
{
	// flags are powers of two
	// so this can handle multiple OR'd flags
	for(uint8_t c = 1; c <= VT_FAINT; c *= 2) {
		if (attribute & c) {
			if (on) {
				current_style.flags |= c;
			} else {
				current_style.flags &= ~c;
			}
		}
	}

	_vt_apply_style();
}


/** Send style and color commands */
void _vt_apply_style()
{
	_vt_reset_attribs_do();
	_vt_style_do();
	_vt_color_do();
}


/** Set color 0..7 */
void vt_color(uint8_t fg, uint8_t bg)
{
	current_style.fg = fg;
	current_style.bg = bg;
	_vt_color_do();
}


/** Set FG color 0..7 */
void vt_color_fg(uint8_t fg)
{
	current_style.fg = fg;
	_vt_color_do();
}


/** Set BG color 0..7 */
void vt_color_bg(uint8_t bg)
{
	current_style.bg = bg;
	_vt_color_do();
}


/** Send reset command */
inline void _vt_reset_attribs_do()
{
	uart_puts_pgm(PSTR("\x1B[m")); // reset
}


/** Send commands for text attribs */
void _vt_style_do()
{
	if (current_style.flags & VT_BOLD) {
		uart_puts_pgm(PSTR("\x1B[1m"));
	}

	if (current_style.flags & VT_FAINT) {
		uart_puts_pgm(PSTR("\x1B[2m"));
	}

	if (current_style.flags & VT_ITALIC) {
		uart_puts_pgm(PSTR("\x1B[3m"));
	}

	if (current_style.flags & VT_UNDERLINE) {
		uart_puts_pgm(PSTR("\x1B[4m"));
	}

	if (current_style.flags & VT_BLINK) {
		uart_puts_pgm(PSTR("\x1B[5m"));
	}

	if (current_style.flags & VT_REVERSE) {
		uart_puts_pgm(PSTR("\x1B[7m"));
	}
}


/** Send commands for xolor */
void _vt_color_do()
{
	uart_putc(27);
	uart_putc('[');
	uart_put8u(30 + current_style.fg);
	uart_putc(';');
	uart_put8u(40 + current_style.bg);
	uart_putc('m');
}


/** Insert blank lines febore the current line */
void vt_insert_lines(uint8_t count)
{
	uart_putc(27);
	uart_putc('[');
	uart_put8u(count);
	uart_putc('L');
}


/** Delete lines from the current line down */
void vt_delete_lines(uint8_t count)
{
	uart_putc(27);
	uart_putc('[');
	uart_put8u(count);
	uart_putc('M');
}


/** Insert empty characters at cursor */
void vt_insert_chars(uint8_t count)
{
	uart_putc(27);
	uart_putc('[');
	uart_put8u(count);
	uart_putc('@');
}


/** Delete characters at cursor */
void vt_delete_chars(uint8_t count)
{
	uart_putc(27);
	uart_putc('[');
	uart_put8u(count);
	uart_putc('P');
}


void vt_clear()
{
	uart_puts_pgm(PSTR("\x1B[2J"));
}


void vt_erase_forth()
{
	uart_puts_pgm(PSTR("\x1B[K"));
}


void vt_erase_back()
{
	uart_puts_pgm(PSTR("\x1B[1K"));
}


void vt_erase_line()
{
	uart_puts_pgm(PSTR("\x1B[2K"));
}


void vt_erase_above()
{
	uart_puts_pgm(PSTR("\x1B[1J"));
}

void vt_erase_below()
{
	uart_puts_pgm(PSTR("\x1B[J"));
}


void vt_home()
{
	uart_puts_pgm(PSTR("\x1B[H"));
}


/** Initialize helper variables */
void vt_init()
{
	vt_reset();
}


/** Reset state and clear screen */
void vt_reset()
{
	// reset color and attributes
	vt_color_reset();
	vt_attr_reset();
	vt_scroll_reset();

	// clear screen
	vt_clear();

	// go to top left
	vt_home();

	// overwrite saved state
	vt_save();
}
