#pragma once
#include <stdint.h>

void fat16_init();
void fat16_list_root();
uint16_t fat16_get_next_cluster(uint16_t cluster);
int fat16_read_file(const char* target_name);
uint16_t fat16_find_free_cluster();
void fat16_mark_cluster(uint16_t cluster, uint16_t value);
int fat16_find_free_dir_entry(uint8_t* out_sector, uint8_t* out_offset);
void fat16_create_file(const char* name, const char* ext, const char* content, uint32_t size);
void fat16_mkdir(const char* dirname);
int fat16_cd(const char* dirname);