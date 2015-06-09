#pragma once

#include <stdint.h>
#include <stdbool.h>

/**
 * Abstract block device interface
 *
 * Populate this with pointers to your I/O functions.
 */
typedef struct
{
	// Sequential read
	void (*load)(void* dest, const uint16_t len);
	// Sequential write
	void (*store)(const void* src, const uint16_t len);
	// Sequential byte write
	void (*write)(const uint8_t b);
	// Sequential byte read
	uint8_t (*read)(void);
	// Absolute seek
	void (*seek)(const uint32_t);
	// Relative seek
	void (*rseek)(const int16_t);
} BLOCKDEV;


// -------------------------------

/**
 * File types (values can be used for debug printing).
 * Accessible using file->type
 */
typedef enum
{
	FT_NONE = '-',
	FT_DELETED = 'x',
	FT_SUBDIR = 'D',
	FT_PARENT = 'P',
	FT_LABEL = 'L',
	FT_LFN = '~',
	FT_INVALID = '?', // not recognized weird file
	FT_SELF = '.',
	FT_FILE = 'F'
} FAT16_FT;


// Include definitions of fully internal structs
#include "fat16_internal.h"


/**
 * File handle struct.
 *
 * File handle contains cursor, file name, type, size etc.
 * Everything (files, dirs) is accessed using this.
 */
typedef struct __attribute__((packed))
{
	/**
	 * Raw file name. Starting 0x05 was converted to 0xE5.
	 * To get PRINTABLE file name, use fat16_dispname()
	 */
	uint8_t name[11];

	/**
	 * File attributes - bit field composed of FA_* flags
	 * (internal)
	 */
	uint8_t attribs;

	// 14 bytes skipped (10 reserved, date, time)

	/** First cluster of the file. (internal) */
	uint16_t clu_start;

	/**
	 * File size in bytes.
	 * This is the current allocated and readable file size.
	 */
	uint32_t size;


	// --- the following fields are added when reading ---

	/** File type. */
	FAT16_FT type;


	// --- INTERNAL FIELDS ---

	// Cursor variables. (internal)
	uint32_t cur_abs; // absolute position in device
	uint32_t cur_rel; // relative position in file
	uint16_t cur_clu; // cluster where the cursor is
	uint16_t cur_ofs; // offset within the active cluster

	// File position in the directory. (internal)
	uint16_t clu; // first cluster of directory
	uint16_t num; // file entry number

	// Pointer to the FAT16 handle. (internal)
	const FAT16* fat;
}
FAT16_FILE;


/** Initialize the file system - store into "fat" */
void fat16_init(const BLOCKDEV* dev, FAT16* fat);


/**
 * Open the first file of the root directory.
 * The file may be invalid (eg. a volume label, deleted etc),
 * or blank (type FT_NONE) if the filesystem is empty.
 */
void fat16_root(const FAT16* fat, FAT16_FILE* file);


/**
 * Resolve the disk label.
 * That can be in the Boot Sector, or in the first root directory entry.
 */
char* fat16_disk_label(const FAT16* fat, char* label_out);


// ----------- FILE I/O -------------


/**
 * Move file cursor to a position relative to file start
 * Returns false on I/O error (bad file, out of range...)
 */
bool fat16_seek(FAT16_FILE* file, uint32_t addr);


/**
 * Read bytes from file into memory
 * Returns false on I/O error (bad file, out of range...)
 */
bool fat16_read(FAT16_FILE* file, void* target, uint32_t len);


/**
 * Write into file at a "seek" position.
 * "seek" cursor must be within (0..filesize)
 */
bool fat16_write(FAT16_FILE* file, void* source, uint32_t len);


/**
 * Create a new file in given folder
 *
 * file ... open directory; new file is opened into this handle.
 * name ... name of the new file, including extension
 */
bool fat16_mkfile(FAT16_FILE* file, const char* name);


/**
 * Create a sub-directory of given name.
 * Directory is allocated and populated with entries "." and ".."
 */
bool fat16_mkdir(FAT16_FILE* file, const char* name);


/**
 * Set new file size.
 * Allocates / frees needed clusters, does NOT erase them.
 *
 * Useful mainly for shrinking.
 */
void fat16_resize(FAT16_FILE* file, uint32_t size);


/**
 * Delete a *FILE* and free it's clusters.
 */
bool fat16_rmfile(FAT16_FILE* file);


/**
 * Delete an empty *DIRECTORY* and free it's clusters.
 */
bool fat16_rmdir(FAT16_FILE* file);


/**
 * Delete a file or directory, even FT_LFN and FT_INVALID.
 * Directories are deleted recursively (!)
 */
bool fat16_delete(FAT16_FILE* file);



// --------- NAVIGATION ------------


/** Go to previous file in the directory (false = no prev file) */
bool fat16_prev(FAT16_FILE* file);


/** Go to next file in directory (false = no next file) */
bool fat16_next(FAT16_FILE* file);


/**
 * Open a subdirectory denoted by the file.
 * Provided handle changes to the first entry of the directory.
 */
bool fat16_opendir(FAT16_FILE* dir);


/**
 * Open a parent directory. Fails in root.
 * Provided handle changes to the first entry of the parent directory.
 */
bool fat16_parent(FAT16_FILE* file);


/** Jump to first file in this directory */
void fat16_first(FAT16_FILE* file);


/**
 * Find a file with given "display name" in this directory.
 * If file is found, "dir" will contain it's handle.
 * If file is NOT found, the handle points to the last entry of the directory.
 */
bool fat16_find(FAT16_FILE* dir, const char* name);


// -------- FILE INSPECTION -----------

/** Check if file is a valid entry, or long-name/label/deleted */
bool fat16_is_regular(const FAT16_FILE* file);


/**
 * Resolve a file name, trim spaces and add null terminator.
 * Returns the passed char*, or NULL on error.
 */
char* fat16_dispname(const FAT16_FILE* file, char* disp_out);


/**
 * Convert filename to zero-padded fixed length one
 * Returns the passed char*.
 */
char* fat16_rawname(const char* disp_in, char* raw_out);

