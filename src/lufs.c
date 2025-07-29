#include "lufs.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

LuFS* lufs_open(const char* filename) {
	FILE* fp = fopen(filename, "rb+");
	if (!fp) return NULL;

	char magic[4];
	if (fread(magic, 1, 4, fp) != 4 || memcmp(magic, "LuFS", 4) != 0) goto lbl_exit;

	uint32_t count;
	if (fread(&count, sizeof(uint32_t), 1, fp) != 1) goto lbl_exit;

	LuFS* fs = malloc(sizeof(LuFS));
	if (!fs) goto lbl_exit;

	fs->fp = fp;
	fs->entry_count = count;
	return fs;
lbl_exit:
	fclose(fp);
	return NULL;
}

void lufs_close(LuFS* fs) {
	if (fs) {
		if (fs->fp) fclose(fs->fp);
		free(fs);
	}
}

int lufs_read_entry(LuFS* fs, uint32_t index, struct FsEntry* out) {
	if (!fs || index >= fs->entry_count) return -1;
	fseek(fs->fp, SECTOR_SIZE + index * ENTRY_SIZE, SEEK_SET);
	return fread(out, 1, ENTRY_SIZE, fs->fp) == ENTRY_SIZE ? 0 : -1;
}

int lufs_read_file_data(LuFS* fs, const struct FsEntry* entry, void* buffer) {
	if (!fs || !entry || !entry->isfile || entry->size == 0) return -1;

	uint8_t* buf = buffer;
	uint32_t block = entry->start_block;
	uint32_t end_block = entry->end_block;

	while (block <= end_block) {
		uint8_t temp[SECTOR_SIZE];
		fseek(fs->fp, block * SECTOR_SIZE, fs->fp);
		fread(temp, 1, SECTOR_SIZE, fs->fp);

		uint16_t start = entry->start_byte;
		uint16_t end = entry->end_byte;

		memcpy(buf, &temp[start], end - start + 1);
	}

	return 0;
}


int lufs_read_children(LuFS* fs, const struct FsEntry* dir, struct FsEntry* out_entries, size_t max_entries) {
	if (!fs || !dir || dir->isfile || !out_entries) return -1;

	size_t start = dir->start_block * SECTOR_SIZE + dir->start_byte;
	size_t end = dir->end_block * SECTOR_SIZE + dir->end_byte;

	size_t total_bytes = end - start + 1;
	size_t entry_count = total_bytes / ENTRY_SIZE;
	if (entry_count > max_entries) entry_count = max_entries;

	uint32_t entry_start_index = (start - SECTOR_SIZE) / ENTRY_SIZE;

	for (size_t i = 0; i < entry_count; i++) {
		if (lufs_read_entry(fs, entry_start_index + i, &out_entries[i]) != 0) return -1;
	}

	return (int)entry_count;
}
