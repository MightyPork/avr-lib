#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

#include "iopins.h"
#include "spi.h"
#include "sd.h"

#define SD_RESET 0x40 // used to make card enter SPI mode
#define SD_GET_STATUS 0x41 // used to check if card left IDLE - should return 0
#define SD_SET_BLOCKLEN 0x50 // used to check if card left IDLE - should return 0
#define SD_READ_BLOCK 0x51 // read single block
#define SD_WRITE_BLOCK 0x58 // write single block


bool sd_inited = false;

bool sd_init()
{
	if (sd_inited) return true;
	sd_inited = true;

	uint8_t i;
	spi_init();

	spi_ss_disable(); // needed for init sequence, first command will enable it again

	// idle for 10 bytes / 80 clocks
	for (i = 0; i < 10; i++)
	{
		spi_write(0xFF);
	}


	// Send "Go to SPI mode" command, which should return "1"
	for (i = 0; i < 100 && sd_command(SD_RESET, 0) != 1; i++)
		_delay_ms(10);

	if (i == 100)
		return false; // timeout


	// CMD1 until card comes out of "idle" mode
	for (i = 0; i < 100 && sd_command(SD_GET_STATUS, 0) != 0; i++)
		_delay_ms(10);

	if (i == 100)
		return false; // timeout


	// f_cpu/8 speed (-> 2 MHz)
	SPSR |= _BV(SPI2X);
	SPCR &= 0xFC | _BV(SPR0);

	// Set block size to 512 bytes (SD card default)
	sd_command(SD_SET_BLOCKLEN, 512);

	return true;
}


uint8_t sd_command(const uint8_t cmd, const uint32_t arg)
{
	spi_ss_enable();

	spi_write(cmd);
	spi_write(arg >> 24);
	spi_write(arg >> 16);
	spi_write(arg >> 8);
	spi_write(arg);
	spi_write(0x95); // CRC for the "init" command, later is ignored

	// Send 8 bytes of 0xFF
	// SD card replies with non-0xFF once it's done processing the command
	uint8_t i, tmp, ret = 0xFF;
	for (i = 0; i < 8; i++)
	{
		tmp = spi_write(0xFF);
		if (tmp != 0xFF)
			ret = tmp;
	}

	spi_ss_disable();

	return ret;
}


bool sd_read(const uint32_t sector, const uint16_t read_at, uint8_t * buffer, const uint16_t write_at, const uint16_t len)
{
	if (read_at + len > 512) return false;

	uint16_t i;

	spi_ss_enable();
	spi_write(SD_READ_BLOCK);
	spi_write(sector >> 15); // sector * 512 >> 24
	spi_write(sector >> 7);  // sector * 512 >> 16
	spi_write(sector << 1);  // sector * 512 >> 8
	spi_write(0);            // sector * 512
	spi_write(0xFF);

	// wait for 0 (ready)
	for (i = 0; i < 100 && spi_write(0xFF) != 0x00; i++);
	if (i == 100)
	{
		spi_ss_disable();
		return false; // timeout
	}


	// wait for 0xFE (data start)
	for (i = 0; i < 100 && spi_write(0xFF) != 0xFE; i++);
	if (i == 100)
	{
		spi_ss_disable();
		return false; // timeout
	}

	// skip "offset" bytes
	for (i = 0; i < read_at; i++)
		spi_write(0xFF);

	// read "len" bytes
	for (i = write_at; i < write_at + len; i++)
		buffer[i] = spi_write(0xFF);

	// skip remaining bytes in the sector
	for (i = read_at + len; i < 512; i++)
		spi_write(0xFF);

	// skip checksum
	spi_write(0xFF);
	spi_write(0xFF);

	spi_ss_disable();

	return true;
}



bool sd_write(const uint32_t sector, const uint8_t * buffer512)
{
	uint16_t i;

	spi_ss_enable();

	spi_write(SD_WRITE_BLOCK);
	spi_write(sector >> 15); // sector * 512 >> 24
	spi_write(sector >> 7);  // sector * 512 >> 16
	spi_write(sector << 1);  // sector * 512 >> 8
	spi_write(0);            // sector * 512
	spi_write(0xFF);

	// wait for 0 (ready)
	for (i = 0; i < 100 && spi_write(0xFF) != 0x00; i++);
	if (i == 100)
	{
		spi_ss_disable();
		return false; // timeout
	}

	// Start of data
	spi_write(0xFE);

	// Data
	for (i = 0; i < 512; i++)
	{
		spi_write(buffer512[i]);
	}

	// Fake CRC
	spi_write(0xFF);
	spi_write(0xFF);

	// Should contain flag that data was accepted
	uint8_t resp = spi_write(0xFF);

	if ((resp & 0x0F) != 0x05)
	{
		// Data not accepted
		spi_ss_disable();
		return false;
	}
	else
	{
		// Data accepted, wait for write complete
		for (i = 0; i < 0xFFFF && spi_write(0xFF) == 0x00; i++);
		if (i == 0xFFFF)
		{
			spi_ss_disable();
			return false; // timeout
		}
	}

	spi_write(0xFF); // 8 clocks
	spi_ss_disable();

	return true;
}
