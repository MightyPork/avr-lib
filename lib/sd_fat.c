#include <stdint.h>
#include <stdbool.h>

#include "sd_blockdev.h"
#include "sd_fat.h"
#include "fat16.h"

FAT16 _fat;
BLOCKDEV _dev;


static STREAM _s;
STREAM* sdf_stream = &_s;

FFILE* stream_file;
bool stream_active = false;

void stream_tx(uint8_t b)
{
	if (!stream_active) return;
	ff_write(stream_file, &b, 1);
}


uint8_t stream_rx()
{
	if (!stream_active) return 0;

	uint8_t b;
	ff_read(stream_file, &b, 1);
	return b;
}

void sdf_open_stream(FFILE* file)
{
	stream_active = true;
	stream_file = file;
}


bool sdfat_inited = false;

bool sdf_init()
{
	if (sdfat_inited) return true;
	sdfat_inited = true;

	if (!sdb_init(&_dev)) return false;
	if (!ff_init(&_dev, &_fat)) return false;

	sdf_stream->rx = &stream_rx;
	sdf_stream->tx = &stream_tx;

	return true;
}


void sdf_root(FFILE* file)
{
	ff_root(&_fat, file);
}


void sdf_disk_label(char* str)
{
	ff_disk_label(&_fat, str);
}
