#include <stdint.h>
#include <stdbool.h>

#include "sd_blockdev.h"
#include "sd_fat.h"
#include "fat16.h"

FAT16 _fat;
BLOCKDEV _dev;

bool sdfat_inited = false;

bool sdfat_init()
{
	if (sdfat_inited) return true;
	sdfat_inited = true;

	if (!sdb_init(&_dev)) return false;
	if (!fat16_init(&_dev, &_fat)) return false;

	return true;
}


void sdfat_root(FAT16_FILE* file)
{
	fat16_root(&_fat, file);
}


void sdfat_disk_label(char* str)
{
	fat16_disk_label(&_fat, str);
}
