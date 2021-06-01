
#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <string.h> // strtok, strcpy, etc.
#include <stdlib.h> // malloc, calloc, free, etc.
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
        // partitionData* pd = get_partition_data();
    // int block_id = pd -> dir_block_id;
    int block_id = dir_block_id;
    FILE *ptr;
    ptr = fopen(disk_route, "rb");
    fseek(ptr, block_id*2048 + 1024, SEEK_SET);
    if (!strcmp(mode, "r")) {
    for (int i = 0; i < 64; i++) {
        unsigned char buffer[32];
        fread(buffer, sizeof(buffer), 1, ptr);
        int index_block = buffer[1] << 16 | buffer[2] << 8 | buffer[3];
        if (!buffer[0]){
            continue;
        }
        char actual_filename[28];
        for (size_t i = 4; i < 32; i++)
        {
            actual_filename[i-4] = buffer[i];
            
        }
        printf("NOmbre del file %s\n", actual_filename);
        if (strcmp(actual_filename, filename)){
            osFile* readfile = malloc(sizeof(osFile*));
            readfile -> name = filename;
            readfile -> index_block_relative_dir = index_block;
            int partition_abs_path = block_id*2048 + 1024;
            int index_block_abs_path = partition_abs_path + index_block;
            
            fseek(ptr, index_block_abs_path, SEEK_SET);
            unsigned char ibbuffer[2048]; // index block buffer
            fread(ibbuffer, sizeof(ibbuffer), 1, ptr);
            readfile -> size = ibbuffer[0] << 32 | ibbuffer[1] << 24 | ibbuffer[2] << 16 | ibbuffer[3] << 8 | ibbuffer[4];
            readfile -> number_of_blocks = readfile -> size / 2048;
            printf("Index block %i\n", readfile -> index_block_relative_dir);
            printf("Blocks %i\n", readfile -> number_of_blocks);
            printf("Size %i\n", readfile -> size);


            readfile -> block_pointers = malloc(681*sizeof(int));
            for (int fbpi; fbpi < 681; fbpi++)
            {
                int block_pointer = ibbuffer[fbpi] << 16 | ibbuffer[fbpi + 1] << 8 | ibbuffer[fbpi + 2];
                readfile -> block_pointers[fbpi] = block_pointer;

            }
        }
        
        
        /* code */
    }
    }
    return 0;
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
        printf("NOmbre del file %s\n",actual_filename);
        if (strcmp(actual_filename, filename)){
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

