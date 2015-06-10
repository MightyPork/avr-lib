#include <stdint.h>
#include <stdbool.h>

#include "sd_blockdev.h"
#include "sd.h"

// helpers
void load_sector(const uint32_t addr);
void store_sector();
void handle_cursor_ov();

// blockdev methods
void dev_load(void* dest, const uint16_t len);
void dev_store(const void* src, const uint16_t len);
uint8_t dev_read();
void dev_write(const uint8_t b);
void dev_seek(const uint32_t addr);
void dev_rseek(const int16_t offset);
void dev_flush();


/** Sector buffer */
uint8_t buff[512];

/** Address of the buffered sector */
uint32_t buff_addr;

/** Buffer needs to be flushed before next read */
bool buff_dirty = false;

/** Buffer holds a valid sector */
bool buff_valid = false;

/** seek cursor */
uint32_t cursor_sec;
uint16_t cursor_offs;


/** Flush the buffer, if it's dirty */
void dev_flush()
{
	if (buff_dirty)
	{
		store_sector();
		buff_dirty = false;
	}
}


void load_sector(const uint32_t addr)
{
	// do not load if already loaded
	if (buff_valid && buff_addr == addr)
	{
		return;
	}

	dev_flush();

	// read entire sector
	sd_read(addr, 0, buff, 0, 512);
	buff_valid = true;
	buff_addr = addr;
}


void store_sector()
{
	// Do not store if not laoded.
	if (!buff_dirty) return;
	if (!buff_valid) return;

	sd_write(buff_addr, buff);
}


/**
 * Handle cursor overflow.
 * MUST ABSOLUTELY NOT load/store buffer or change buffer addr!
*/
inline void handle_cursor_ov()
{
	if (cursor_offs >= 512)
	{
		cursor_sec++;
		cursor_offs = 0;
	}
}



void dev_write(const uint8_t b)
{
	load_sector(cursor_sec);

	// dirty only if changed
	if (buff[cursor_offs] != b)
	{
		buff[cursor_offs++] = b;
		buff_dirty = true;
	}
	else
	{
		cursor_offs++;
	}

	handle_cursor_ov();
}


uint8_t dev_read()
{
	load_sector(cursor_sec);
	const uint8_t b = buff[cursor_offs++];

	handle_cursor_ov();

	return b;
}


void dev_load(void* dest, const uint16_t len)
{
	for (uint16_t a = 0; a < len; a++)
	{
		*((uint8_t*)dest++) = dev_read();
	}
}


void dev_store(const void* src, const uint16_t len)
{
	for (uint16_t a = 0; a < len; a++)
	{
		dev_write(*((uint8_t*)src++));
	}
}


void dev_seek(const uint32_t addr)
{
	// compute sector and offset counters
	cursor_sec = addr >> 9;
	cursor_offs = addr & 0x1FF;
}


void dev_rseek(const int16_t offset)
{
	// add WITHIN the same sector
	if (offset > 0 && cursor_offs + offset < 512)
	{
		cursor_offs += offset;
		return;
	}

	// subtract WITHIN the same sector
	if (offset < 0 && ((uint16_t)(-offset) <= cursor_offs))
	{
		cursor_offs += offset;
		return;
	}

	// abs addr change
	dev_seek(((cursor_sec << 9) + cursor_offs) + offset);
}


/** Init SD card block device */
bool sdb_init(BLOCKDEV* dev)
{
	if (!sd_init()) return false;

	dev->load = &dev_load;
	dev->store = &dev_store;
	dev->read = &dev_read;
	dev->write = &dev_write;
	dev->seek = &dev_seek;
	dev->rseek = &dev_rseek;
	dev->flush = &dev_flush;

	return true;
}

