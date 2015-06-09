#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

#include "iopins.h"
#include "spi.h"

/** Init SPI (for SD card communication) */
void spi_init()
{
	// Pin configuration
	as_output(PIN_SS);
	as_output(PIN_MOSI);
	as_output(PIN_SCK);
	as_input_pu(PIN_MISO);

	// Enable SPI, master, clock = F_CPU/128
	SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR0) | _BV(SPR1);
}


/** Write a byte to SPI. Returns received byte. */
uint8_t spi_write(const uint8_t b)
{
	SPDR = b;
	while (!(SPSR & _BV(SPIF)));

	return SPDR;
}

