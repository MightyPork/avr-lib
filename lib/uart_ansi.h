#pragma once

//
// ANSI / VT100 utilities for UART
//

#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include "uart.h"

#define VT_NORMAL 0
#define VT_BOLD 1
#define VT_UNDERLINE 2
#define VT_BLINK 4
#define VT_REVERSE 8
#define VT_HIDDEN 16

/** Jump to a location on the screen */
void vt_goto(uint16_t x, uint16_t y);

/** Move cursor relative to current location */
void vt_move(int16_t x, int16_t y);

/** Move cursor horizontally */
void vt_move_x(int16_t x);

/** Move cursor vertically */
void vt_move_y(int16_t y);

/** Move cursor up y cells */
void vt_up(uint16_t y);

/** Move cursor down y cells */
void vt_down(uint16_t y);

/** Move cursor left x cells */
void vt_left(uint16_t x);

/** Move cursor right x cells */
void vt_right(uint16_t x);

/** Scroll y lines down (like up/down, but moves window if needed) */
void vt_scroll(int16_t down);

/** Set font style */
void vt_style(uint8_t flags);

/** Set color */
void vt_color(uint8_t fg, uint8_t bg);

/** Save cursor position & text attributes */
void vt_save();

/** Restore cursor to saved values */
void vt_restore();

/** Clear the screen */
void vt_clear();

/** Move cursor to top left corner */
void vt_home();
