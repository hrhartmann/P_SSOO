
#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <string.h> // strtok, strcpy, etc.
#include <stdlib.h> // malloc, calloc, free, etc.
#include <stdint.h> // Enteros de 64 bits
#include "os_API.h"

char* disk_route = "";
int partition = 0;
int dir_block_id = 0;
int number_of_blocks = 0;


void os_mount(char* diskname, int to_partition){
    disk_route = diskname;
    partition = to_partition;
    unsigned char buffer[1024];
    FILE *ptr;
    ptr = fopen(disk_route,"rb");
    fread(buffer,sizeof(buffer),1,ptr);
    for(int i = 0; i<1024; i+=8){
        int binary[8];
        for(int n = 0; n < 8; n++)
            binary[7-n] = (buffer[i] >> n) & 1;

        if (binary[0] == 1){                                      
            if(buffer[i]-128 == partition){
                int block_number = buffer[i+1] << 16 | buffer[i+2] << 8 | buffer[i+3];
                int ctd = buffer[i+4] << 24 | buffer[i+5] << 16 | buffer[i+6] << 8 | buffer[i+7];
                printf("Numero de bloque %u\n", block_number);
                printf("Cantidad de bloque %u\n", ctd);
                dir_block_id = block_number;
                number_of_blocks = ctd;
            }
        }    
    }
    
    fclose(ptr);


};

// partitionData* get_partition_data()
// {
//     unsigned char buffer[1024];
//     FILE *ptr;
//     ptr = fopen(disk_route,"rb");
//     fread(buffer,sizeof(buffer),1,ptr);
//     for(int i = 0; i<1024; i+=8){
//         int binary[8];
//         for(int n = 0; n < 8; n++)
//             binary[7-n] = (buffer[i] >> n) & 1;

//         if (binary[0] == 1){                                      
//             if(buffer[i]-128 == partition){
//                 int block_number = buffer[i+1] << 16 | buffer[i+2] << 8 | buffer[i+3];
//                 int ctd = buffer[i+4] << 24 | buffer[i+5] << 16 | buffer[i+6] << 8 | buffer[i+7];
//                 printf("Numero de bloque %u\n", block_number);
//                 printf("Cantidad de bloque %u\n", ctd);
//                 partitionData* new_partition_data = malloc(sizeof(partitionData*));
//                 new_partition_data -> id = partition;
//                 new_partition_data -> dir_block_id = block_number;
//                 new_partition_data -> number_of_blocks = ctd;
//                 return new_partition_data;
//             }
//         }    
//     }
    
//     fclose(ptr);
// };

void os_mbt() {
    // Array buffer donde se van a guardar bytes
    unsigned char buffer[1024];
    // Puntero a archivo 
    FILE *ptr;
    // lecutra del disko 
    ptr = fopen(disk_route,"rb");
    // Leemos el disco y asignamos al buffer los bytes. En este caso solo cambiar en buffer el largo para ver cuanto leer. Ahora llemos 1024 bytes
    fread(buffer,sizeof(buffer),1,ptr);

    for(int i = 0; i<1024; i+=8){
        //printf("%u ", buffer[i]);
        // Definimos un array de ints que van a contener los bits
        int binary[8];
        // En este for poblamos el array de bits segun el byte buffer[i] (Acordar que buffer es array de bytes)
        for(int n = 0; n < 8; n++)
            
            binary[7-n] = (buffer[i] >> n) & 1;

        // for(int n = 0; n < 8; n++){
        //     printf("%d", binary[n]);
        // }
        // Revisamos el primer bit
        if (binary[0] == 1){
            printf("Particion %u valida\n", buffer[i]-128);
        }
        //printf("\n");
    
    }
    fclose(ptr);
}


void os_bitmap(unsigned num){
    unsigned char buffer[1024];
    FILE *ptr;
    ptr = fopen(disk_route,"rb");
    
    for(int i = 0; i<1024; i+=8){
        int binary[8];
        for(int n = 0; n < 8; n++)
            binary[7-n] = (buffer[i] >> n) & 1;

        if (binary[0] == 1){                                      
            if(buffer[i]-128 == partition){
                int block_number = buffer[i+1] << 16 | buffer[i+2] << 8 | buffer[i+3];
                int ctd = buffer[i+4] << 24 | buffer[i+5] << 16 | buffer[i+6] << 8 | buffer[i+7];
                printf("num0 %u\n", buffer[i]);
                printf("num1 %u\n", buffer[i+1]);
                printf("num2 %u\n", buffer[i+2]);
                printf("num3 %u\n", buffer[i+3]);
                printf("Numero de bloque %u\n", block_number);
                printf("num4 %u\n", buffer[i+4]);
                printf("num5 %u\n", buffer[i+5]);
                printf("num6 %u\n", buffer[i+6]);
                printf("num7 %u\n", buffer[i+7]);
                printf("Cantidad de bloque %u\n", ctd);
            }



        }    
    }
    

    if (num == 0){
        printf("Imprimir bitmaps entero\n");
    }else{
        //fseek(ptr, block_number*2048 + 1024, SEEK_SET);
        // // Me encuentro en el bloque directorio
        
    }
    fclose(ptr);

}

osFile* os_open(char* filename, char* mode) {
    // printf("=====os_open=====\n");
    // Creación del osFile
    osFile* os_file = malloc(sizeof(osFile));
    os_file -> last_byte_read = -1;

    // El archivo existe y se quiere leer
    if (os_exists(filename) && !strcmp(mode, "r")) {
        // printf("Base of partition: %i\n", dir_block_id)
        os_file -> mode = mode;

        FILE *ptr;
        ptr = fopen(disk_route, "rb");
        fseek(ptr, dir_block_id*2048 + 1024, SEEK_SET);

        int relative_block_id;
        
        for (int i = 0; i < 64; i++) {
            unsigned char buffer[32];
            fread(buffer, sizeof(buffer), 1, ptr);
            if (!buffer[0]) {
                continue;
            }
            char actual_filename[28];
            for (size_t i = 4; i < 32; i++) {
                actual_filename[i-4] = buffer[i];
            }
            if (!strcmp(actual_filename, filename)) {
                // printf("Nombre del file: %s\n",actual_filename);
                // Número del bloque índice del archivo relativo a la partición.
                relative_block_id = (buffer[1] << 16) | (buffer[2] << 8) | buffer[3];
                break;
            }
        }
        // printf("Relative block id of index block of file: %i\n", relative_block_id);
        os_file -> index_block_relative_dir = relative_block_id;

        // Número del bloque índice del archivo en términos absolutos respecto al disco
        // int absolute_block_id = relative_block_id + dir_block_id;
        // printf("Absolute block id of index block of file: %i\n", absolute_block_id);

        unsigned char index_buffer[2048];
        fseek(ptr, (relative_block_id)*2048 + 1024, SEEK_SET);
        fread(index_buffer, sizeof(index_buffer), 1, ptr);

        // printf("%i \n", (relative_block_id)*2048 + 1024);
        // for (int i = 0; i < 5; i++) {
        //     printf("%d\n", index_buffer[i]);
        // }

        uint64_t size_bytes[5];
        for (int i = 0; i < 5; i++) {
            size_bytes[i] = (uint64_t) index_buffer[i];
        }

        os_file -> index_block_relative_dir = relative_block_id;

        // Tamaño del archivo en bytes.
        int file_size = size_bytes[0] << 32 | size_bytes[1] << 24 | size_bytes[2] << 16 | size_bytes[3] << 8 | size_bytes[4];
        // printf("File size (bytes): %i\n", file_size);
        os_file -> size = file_size;
        
        // Tamaño del archivo en bloques.
        int file_size_blocks = (file_size + 2048 - 1) / 2048;
        // printf("File size (blocks): %i\n", file_size_blocks);
        os_file -> number_of_blocks = file_size_blocks;


        os_file -> block_pointers = malloc(sizeof(int)*681);
        for (int i = 0; i < 681; i++) {
            if (i >= os_file -> number_of_blocks) {
                os_file -> block_pointers[i] = (int) -1;
                // printf("%i ", os_file -> block_pointers[i]);
            } else {
                int actual_block_pointer = index_buffer[i*3+5] << 16 | index_buffer[i*3+1+5] << 8 | index_buffer[i*3+2+5];
                // printf("%i ", actual_block_pointer);
                os_file -> block_pointers[i] = actual_block_pointer + dir_block_id;
            }
            // printf("%i %i %i \n", index_buffer[i*3+5], index_buffer[i*3+1+5], index_buffer[i*3+2+5]);
        }
        fclose(ptr);


    } else if(!os_exists(filename) && !strcmp(mode, "w")) {
        // El archivo no existe y se quiere escribir
        os_file -> name = filename;
        os_file -> mode = mode;
        int number_of_bitmap_blocks = (number_of_blocks + (2048*8) -1)/(2048*8);

        // printf("Mode: w\n");
        // printf("Base of partition: %i\n", dir_block_id);
        // printf("BLOCKS: %i\n", number_of_blocks);
        // printf("NUMBER OF BITMAP BLOCKS: %i\n", number_of_bitmap_blocks);

        FILE *ptr;
        ptr = fopen(disk_route, "r+b");
        // fseek(ptr, dir_block_id*2048 + 1024, SEEK_SET);
        // int relative_block_id;

        // Buscar un bloque vacío para poner el bloque índice del archivo.
        int blocks_checked = 0;
        int block_to_use = 0;
        for (int i = 0; i < number_of_bitmap_blocks; i++) {
            int actual_block = dir_block_id + 1 + i;
            fseek(ptr, actual_block*2048 + 1024, SEEK_SET);
            unsigned char buffer[2048];
            fread(buffer, sizeof(buffer), 1, ptr);
            for (int byte = 0; byte < 2048; byte++) {
                unsigned char byte_read = buffer[byte];
                for (int bit = 0; bit < 8 ; bit++) {
                    unsigned char bit_read = ((byte_read >> (7-bit)) & 0x01);
                    // printf("%i ", bit_read);
                    blocks_checked += 1;
                    if (!bit_read && blocks_checked <= number_of_blocks) {
                        block_to_use = byte*8 + bit;
                        // printf("BYTESS: %i\n", (block_to_use >> 16) & 0xff);
                        // printf("BYTESS: %i\n", (block_to_use >> 8) & 0xff);
                        // printf("BYTESS: %i\n", (block_to_use) & 0xff);

                        byte_read ^= (1 << (7-bit));
                        // unsigned char new_bit_read = ((byte_read >> (7-bit)) & 0x01);
                        // printf("NEW BIT: %i \n", new_bit_read);
                        fseek(ptr, actual_block*2048 + 1024 + byte, SEEK_SET);
                        fputc(byte_read, ptr);
                        break;
                    }

                }
                if (block_to_use) {
                    break;
                }
                // printf("%i ", byte_read);
            }
            if (block_to_use) {
                break;
            }
        }
        if (!block_to_use) {
            printf("Partition full. Unable to create new file.\n");
            return NULL;
        }

        // Configuramos el bloque de directorio en el disco
        fseek(ptr, dir_block_id*2048 + 1024, SEEK_SET);
        for (int i = 0; i < 64; i++) {
            unsigned char buffer[32];
            fread(buffer, sizeof(buffer), 1, ptr);
            if (buffer[0]) {
                continue;
            }
            // fseek a la primera posición vacía del bloque directorio
            fseek(ptr, (dir_block_id*2048) + 1024 + 32 * i, SEEK_SET);
            // Cambiamos la validez de la posición.
            fputc(1, ptr);
            // Agregamos los 3 bytes de posición relativa del archivo.
            fputc((block_to_use >> 16) & 0xff, ptr);
            fputc((block_to_use >> 8) & 0xff, ptr);
            fputc((block_to_use) & 0xff, ptr);
            // Agregamos el nombre del archivo.
            for (int j = 0; j < 28; j++) {
                if (j < strlen(filename)) {
                    fputc(filename[j], ptr);
                } else {
                    fputc(0, ptr);
                }
            }
            break;
        }

        fseek(ptr, (block_to_use*2048) + 1024, SEEK_SET);
        for (int i = 0; i < 5; i++) {
            fputc(0, ptr);
        }
        // Poblamos el osFile correspondiente
        os_file -> index_block_relative_dir = block_to_use;
        os_file -> size = 0;
        os_file -> number_of_blocks = 0;
        os_file -> block_pointers = malloc(sizeof(int)*681);
        for (int i = 0; i < 681; i++) {
            os_file -> block_pointers[i] = (int) -1;
        } 

        fclose(ptr);

    } else if(os_exists(filename) && !strcmp(mode, "w")) {
        printf("File already exists.\n");
        return NULL;
    } else {
        // Ninguna de las opciones anteriores
        printf("Trying to write on existing file or file not found.\n");
        return NULL;
    }

    return os_file;
}

int os_read(osFile* file_desc, void* buffer, int nbytes) {
    // Primer byte a leer
    int actual_byte_to_read = file_desc -> last_byte_read + 1;

    // Bytes leídos
    int nbytes_read = 0;
    
    // printf("Bytes to read: %i\n", nbytes);

    FILE *ptr;
    ptr = fopen(disk_route, "rb");

    while (nbytes_read < nbytes && actual_byte_to_read < file_desc -> size) {
        // Número de bloque de datos a leer del archivo
        int actual_block_number = actual_byte_to_read / 2048;
        // Byte del archivo a leer
        int actual_byte_on_block_number = actual_byte_to_read % 2048;
        // Bloque real a leer del disco
        int actual_block = file_desc -> block_pointers[actual_block_number];
        // printf("Block number: %i \n", actual_block_number);
        // printf("Byte on block number: %i \n", actual_byte_on_block_number);
        // printf("Actual block: %i \n", actual_block);

        // Se lee un byte a la vez y se escribe en la dirección apuntada por buffer
        unsigned char read_buffer;
        int index = actual_block*2048 + 1024 + actual_byte_on_block_number;
        fseek(ptr, index, SEEK_SET);
        // printf("BYTE: %i \n", file_desc -> block_pointers[actual_block_number]*2048 + 1024 + actual_byte_on_block_number);
        fread(&read_buffer, 1, 1, ptr);
        ((unsigned char*)buffer)[nbytes_read] = read_buffer;
        // printf("BR: %d\n", read_buffer);

        // Aumentamos los contadores
        nbytes_read += 1;
        file_desc -> last_byte_read += 1;
        actual_byte_to_read += 1;
    }

    fclose(ptr);
    return nbytes_read;
}

int os_exists(char* filename){
    // partitionData* pd = get_partition_data();
    // int block_id = pd -> dir_block_id;
    int block_id = dir_block_id;

    
    FILE *ptr;
    ptr = fopen(disk_route, "rb");
    fseek(ptr, block_id*2048 + 1024, SEEK_SET);
    
    for (int i = 0; i < 64; i++)
    {
        unsigned char buffer[32];
        fread(buffer, sizeof(buffer), 1, ptr);
        if (!buffer[0]){
            continue;
        }
        char actual_filename[28];
        for (size_t i = 4; i < 32; i++)
        {
            actual_filename[i-4] = buffer[i];
            
        }
        // printf("NOmbre del file %s\n",actual_filename);
        if (!strcmp(actual_filename, filename)){
            return 1;
        }
        
        
        /* code */
    }
    return 0;
}


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

