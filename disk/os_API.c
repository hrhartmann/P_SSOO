
#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <string.h> // strtok, strcpy, etc.
#include <stdlib.h> // malloc, calloc, free, etc.

#include "os_API.h"

void os_mbt() {
    int alpha;
}

// unsigned int_to_int(unsigned k) {
//     if (k == 0) return 0;
//     if (k == 1) return 1;                       /* optional */
//     return (k % 2) + 10 * int_to_int(k / 2);
// }

void read_file(char* diskname)
{
    // Read the file
    printf("Example\n");
    printf("Before defining file_pointer\n");
    FILE* file_pointer = fopen(diskname, "rb");

    // Declare a buffer and read the first line
    printf("El BUFFER SIZE ES %d\n", BUFFER_SIZE);
    char buffer[BUFFER_SIZE];
    
    printf("Before fread\n");
    //fgets(buffer, BUFFER_SIZE, file_pointer);
    fread(buffer, sizeof(buffer), 1, file_pointer); //Lee

    printf("Before for\n");
    int act_partition = 0;
    for (int i=0; i < BUFFER_SIZE; i+=8)
    {
        
        printf("New partition %i\n", act_partition);
        act_partition++;
        for (int j=0; j < 8; j++)
        {
            int entry_byte = buffer[i + j];
            if (j == 0)
            {
                printf("%u\n", entry_byte);
                unsigned char mask = 1; // Bit mask
                unsigned char bits[8];
                for (int k = 0; k < 8; k++) {
                    // Mask each bit in the byte and store it
                    bits[k] = (entry_byte & (mask << k)) != 0;
                }
                // For debug purposes, lets print the received data
                for (int l = 0; l < 8; l++) {
                    // if (l == 0 || l == 7)
                    // {
                    //     printf("PRIMER BYTE Bit: %d\n",bits[l]);
                    // }
                    printf("PRIMER BYTE Bit: %d\n",bits[l]);
                    
                }
            }
            // printf("%i\n", entry_byte);
            // printf("%u\n", entry_byte);

            
        }

        // printf("%u\n", buffer[i]);
    }

    // Define the struct and prepare its attributes
    // InputFile* input_file = malloc(sizeof(InputFile));
    //   input_file->lines = calloc(atoi(buffer), sizeof(char**));
    //   input_file->len = atoi(buffer);

    // Parse each line
    //   int index = 0;
    //   while (fgets(buffer, BUFFER_SIZE, file_pointer)) {
    //     input_file->lines[index++] = split_by_sep(buffer, " ");
    //   }

    // Close the file and return the struct
    fclose(file_pointer);
}

