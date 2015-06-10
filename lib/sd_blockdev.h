#pragma once

#include "blockdev.h"

/**
 * Flush the sector buffer if it's dirty.
 *
 * Should be called after each sequence of writes,
 * to avoid data loss.
 */
void sdb_flush();

/** Initialize the SD card block device */
bool sdb_init(BLOCKDEV* dev);
