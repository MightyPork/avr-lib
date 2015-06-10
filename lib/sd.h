#pragma once

//
// SD card low-level I/O utilities
//
// Inspired by:
// http://www.avrfreaks.net/forum/tutc-simple-fat-and-sd-tutorial
//

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

#include "iopins.h"
#include "spi.h"

/** Init SD card on SPI */
bool sd_init();


/**
 * Send a command to the SD card
 *
 * @param cmd command to send
 * @param arg command argument
 * @return return value on success, 0xFF if nothing received back.
 */
uint8_t sd_command(uint8_t cmd, uint32_t arg);


/**
 * Read from a sector into a buffer memory structure.
 *
 * @param sector   sector to read (512 bytes long each)
 * @param read_at  offset within the sector
 * @param buffer   target buffer
 * @param write_at target starting address
 * @param len      number of bytes to read
 * @return true on success
 */
bool sd_read(uint32_t sector, uint16_t read_at, uint8_t * buffer, uint16_t write_at, uint16_t len);


/**
 * Write bytes from a buffer into a sector.
 *
 * @param sector    sector to write (512 bytes long each)
 * @param buffer512 source buffer
 * @return true on success
 */
bool sd_write(uint32_t sector, const uint8_t * buffer512);
