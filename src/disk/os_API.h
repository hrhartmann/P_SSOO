
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
    long int size;
    int number_of_blocks;
    int* block_pointers;
    int last_pointer;
    int last_byte_read;
    int is_open;
    int dir_block_entry;
    // int last_block_free_bytes;
};
typedef struct os_file osFile;




void os_ls();
void os_create_partition(int id, int size);
void os_delete_partition(int id);
void os_reset_mbt();
int comp (const void * elem1, const void * elem2);

int os_exists(char* filename);
void os_mount(char* diskname, int partition);
osFile* os_open(char *filename, char *mode);
void os_mbt();
void os_bitmap(unsigned num);
void read_file(char* diskname);
unsigned int_to_int(unsigned k);
int os_read(osFile* file_desc, void* buffer, int nbytes);

void save_file(osFile* file_desc, int saved_space);
int os_close(osFile* file_desc);
int os_write(osFile* file_desc, void* buffer, int nbytes);
int getset_empty_block();
int os_rm(char* filename);