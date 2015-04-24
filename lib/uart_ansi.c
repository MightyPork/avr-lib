#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

#include "uart.h"
#include "uart_ansi.h"


void vt_goto(uint16_t x, uint16_t y)
{
	uart_putc(27);
	uart_putc('[');
	uart_putu(x);
	uart_putc(';');
	uart_putu(y);
	uart_putc('H');
}


void vt_move(int16_t x, int16_t y)
{
	vt_move_x(x);
	vt_move_y(y);
}


void vt_move_x(int16_t x)
{
	if (x < 0) {
		vt_left(-x);
	} else {
		vt_right(x);
	}
}


void vt_move_y(int16_t y)
{
	if (y < 0) {
		vt_up(-y);
	} else {
		vt_down(y);
	}
}


void vt_up(uint16_t y)
{
	if (y == 0) return;
	uart_putc(27);
	uart_putc('[');
	uart_putu(y);
	uart_putc('A');
}


void vt_down(uint16_t y)
{
	if (y == 0) return;
	uart_putc(27);
	uart_putc('[');
	uart_putu(y);
	uart_putc('B');
}


void vt_left(uint16_t x)
{
	if (x == 0) return;
	uart_putc(27);
	uart_putc('[');
	uart_putu(x);
	uart_putc('D');
}


void vt_right(uint16_t x)
{
	if (x == 0) return;
	uart_putc(27);
	uart_putc('[');
	uart_putu(x);
	uart_putc('C');
}


void vt_scroll(int16_t y)
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


void vt_save()
{
	uart_putc(27);
	uart_putc(7);
}


void vt_restore()
{
	uart_putc(27);
	uart_putc(8);
}


void vt_style(uint8_t flags)
{
	if (flags == VT_NORMAL) {
		uart_puts("\x1B[m"); // reset
		return;
	}

	if (flags & VT_BOLD) {
		uart_puts("\x1B[1m");
	}

	if (flags & VT_UNDERLINE) {
		uart_puts("\x1B[4m");
	}

	if (flags & VT_BLINK) {
		uart_puts("\x1B[5m");
	}

	if (flags & VT_REVERSE) {
		uart_puts("\x1B[7m");
	}

	if (flags & VT_HIDDEN) {
		uart_puts("\x1B[8m");
	}
}


void vt_color(uint8_t fg, uint8_t bg)
{
	uart_putc(27);
	uart_putc('[');
	uart_putu(fg);
	uart_putc(';');
	uart_putu(bg);
	uart_putc('m');
}


void vt_clear()
{
	uart_puts("\x1B[2J");
}


void vt_home()
{
	uart_puts("\x1B[H");
}
