#pragma once

#include <stdint.h>

#include "iopins.h"

#define PIN_MISO 12
#define PIN_MOSI 11
#define PIN_SCK 13
#define PIN_SS 10

/** Set SS to active state (LOW) */
#define spi_ss_enable()  pin_low(PIN_SS)

/** Set SS to disabled state (HIGH) */
#define spi_ss_disable() pin_high(PIN_SS)


/** Init SPI (for SD card communication) */
void spi_init();


/**
 * Write / read a byte to SPI.
 *
 * @param ch the written byte
 * @return received byte
 */
uint8_t spi_write(uint8_t b);

