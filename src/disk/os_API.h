
#pragma once

// Define compile-time constants
#define MAX_SPLIT 255
// #define BUFFER_SIZE 2048
#define BUFFER_SIZE 1024
extern char* disk_route;
extern int partition;
extern int dir_block_id;
extern int number_of_blocks;

// struct partition_data {
//     int id;
//     int dir_block_id;
//     int number_of_blocks;
// };
// typedef struct partition_data partitionData;

struct os_File {
    char* name;
};

int os_exists(char* filename);
void os_mount(char* diskname, int partition);
void os_mbt();
void os_bitmap(unsigned num);
void os_ls();
void os_create_partition(int id, int size);
void os_delete_partition(int id);
void os_reset_mbt();

void read_file(char* diskname);
unsigned int_to_int(unsigned k);