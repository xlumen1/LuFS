#ifndef LUFS_H
#define LUFS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#define ENTRY_SIZE 49
#define ENTRIES_PER_SECTOR 9
#define SECTOR_SIZE 512

struct FsEntry {
	char name[32];
	uint32_t size;
	uint8_t isfile;
	uint32_t start_block;
	uint32_t end_block;
	uint16_t start_byte;
	uint16_t end_byte;
} __attribute__((packed));

typedef struct {
	FILE* fp;
	uint32_t entry_count;
} LuFS;

LuFS* lufs_open(const char* filename);
void lufs_close(LuFS* fs);
int lufs_read_entry(LuFS* fs, uint32_t index, struct FsEntry* out);
int lufs_read_file_data(LuFS* fs, const struct FsEntry* entry, void* buffer);
int lufs_read_children(LuFS* fs, const struct FsEntry* dir, struct FsEntry* out_entries, size_t max_entries);
#endif

