
#pragma once

// Define compile-time constants
#define MAX_SPLIT 255
// #define BUFFER_SIZE 2048
#define BUFFER_SIZE 1024
extern char* disk_route;

struct os_File {
    char* name;
};


void os_mbt();
void read_file(char* diskname);
unsigned int_to_int(unsigned k);