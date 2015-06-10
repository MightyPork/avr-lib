#pragma once

//
// FAT-on-SD helpers
//

#include "fat16.h"

/** Initialize FAT16 filesystem on a SPI-connected SD card */
bool sdfat_init();

/** Get first file of the root folder. */
void sdfat_root(FAT16_FILE* file);

/** Get a disk label. Str should have 12 chars. */
void sdfat_disk_label(char* str);

/** Flush the SD buffer (alis of sdb_flush()) */
#define sdfat_flush() sdb_flush()
