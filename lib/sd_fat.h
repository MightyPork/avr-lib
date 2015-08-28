#pragma once

//
// FAT-on-SD helpers.
//
// This can be used for convenience, as it does all the init for you
// and hides the implementation. All regular ff_* functions will work on the FFILE.
//

#include <stdint.h>

#include "fat16.h"
#include "stream.h"

/** Initialize FAT16 filesystem on a SPI-connected SD card */
bool sdf_init();

/** Get first file of the root folder. */
void sdf_root(FFILE* file);

/** Get a disk label. Str should have 12 chars. */
void sdf_disk_label(char* str);

extern STREAM* sdf_stream;

/**
 * Open a stream for a file. There can be only one stream at a time.
 *
 * The stream will operate at the current file's cursor, just like
 * ff_read and ff_write.
*/
void sdf_open_stream(FFILE* file);
