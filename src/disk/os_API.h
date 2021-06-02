
#pragma once

// Define compile-time constants
#define MAX_SPLIT 255
// #define BUFFER_SIZE 2048
#define BUFFER_SIZE 1024
extern char* disk_route;
extern int partition;
extern int dir_block_id;
extern int number_of_blocks;

struct os_file {
    char* name;
    char* mode;
    int index_block_relative_dir;
    int size;
    int number_of_blocks;
    int* block_pointers;
    int last_pointer;
    int last_byte_read;
};
typedef struct os_file osFile;

int os_exists(char* filename);
void os_mount(char* diskname, int partition);
osFile* os_open(char *filename, char *mode);
void os_mbt();
void os_bitmap(unsigned num);
void read_file(char* diskname);
unsigned int_to_int(unsigned k);
int os_read(osFile* file_desc, void* buffer, int nbytes);