#pragma once

//
// ANSI / VT100 utilities for UART
//
// To use this, first call uart_init(baud) and vt_init()
// To print stuff on the screen, use uart_puts() etc from uart.h
//

#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include "uart.h"



// INIT

/** Initialize helper variables */
void vt_init();

/** Reset state and clear screen */
void vt_reset();



// CURSOR MOVE

/** Move cursor to top left corner */
void vt_home();

/** Jump to a location on the screen */
void vt_goto(uint8_t x, uint8_t y);

/** Jump to given X, keep Y */
void vt_goto_x(uint8_t x);

/** Jump to given Y, keep X */
void vt_goto_y(uint8_t y);

/** Move cursor relative to current location */
void vt_move(int8_t x, int8_t y);

/** Move cursor horizontally */
void vt_move_x(int8_t x);

/** Move cursor vertically */
void vt_move_y(int8_t y);

/** Move cursor up y cells */
void vt_up(uint8_t y);

/** Move cursor down y cells */
void vt_down(uint8_t y);

/** Move cursor left x cells */
void vt_left(uint8_t x);

/** Move cursor right x cells */
void vt_right(uint8_t x);



// SCROLLING

/** Scroll y lines down (like up/down, but moves window if needed) */
void vt_scroll(int8_t down);

/** Set scrolling region (lines) */
void vt_scroll_set(uint8_t from, uint8_t to);


/** Sets scrolling region to the entire screen. */
void vt_scroll_reset();


// COLOR

#define VT_BLACK 0
#define VT_RED 1
#define VT_GREEN 2
#define VT_YELLOW 3
#define VT_BLUE 4
#define VT_MAGENTA 5
#define VT_CYAN 6
#define VT_WHITE 7

/** Set color 0..7 */
void vt_color(uint8_t fg, uint8_t bg);

/** Set FG color 0..7 */
void vt_color_fg(uint8_t fg);

/** Set BG color 0..7 */
void vt_color_bg(uint8_t bg);

/** Set color to white on black */
void vt_color_reset();



// STYLES

#define VT_BOLD 1
#define VT_UNDERLINE 2
#define VT_BLINK 4
#define VT_REVERSE 8
#define VT_ITALIC 16
#define VT_FAINT 32

/** Enable or disable a text attribute */
void vt_attr(uint8_t attribute, bool on);

/** Disable all text attributes (excluding color) */
void vt_attr_reset();



// SAVE & RESTORE

/** Save cursor position & text attributes */
void vt_save();

/** Restore cursor to saved values */
void vt_restore();



// MODIFY


/** Insert blank lines febore the current line */
void vt_insert_lines(uint8_t count);

/** Delete lines from the current line down */
void vt_delete_lines(uint8_t count);

/** Insert empty characters at cursor */
void vt_insert_chars(uint8_t count);

/** Delete characters at cursor */
void vt_delete_chars(uint8_t count);



// ERASING

/** Clear the screen */
void vt_clear();

/** Erase to the end of line */
void vt_erase_forth();

/** Erase line to cursor */
void vt_erase_back();

/** Erase entire line */
void vt_erase_line();

/** Erase screen below the line */
void vt_erase_above();

/** Erase screen above the line */
void vt_erase_below();
