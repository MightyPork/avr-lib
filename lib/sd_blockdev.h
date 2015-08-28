#pragma once

#include "blockdev.h"
#include <stdbool.h>

/** Initialize the SD card block device */
bool sdb_init(BLOCKDEV* dev);
