
#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <string.h> // strtok, strcpy, etc.
#include <stdlib.h> // malloc, calloc, free, etc.
#include <math.h>
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
    
    FILE *ptr;
    ptr = fopen(disk_route,"rb");
    int bit_blocks = ceil((double) number_of_blocks/16384);
    int last_bits = number_of_blocks % 16384;
    
    if (num == 0){
        printf("Imprimir todo\n");
        fseek(ptr, dir_block_id*2048 + 1024 + 2048, SEEK_SET);
        for (int i = 0; i < bit_blocks; i++)
        {
            printf("Bloque bitmap %d\n", i+1);
            unsigned char buffer[2048];
            fread(buffer, sizeof(buffer), 1, ptr);
            for (int j = 0; j < 2048; j++)
            {
                fprintf(stderr, "%02X", buffer[j]);
            }
            printf("\n");
        }
        
    }else{
        fseek(ptr, dir_block_id*2048 + 1024 + num*2048, SEEK_SET);
        unsigned char buffer[2048];
        fread(buffer, sizeof(buffer), 1, ptr);
        for (int i = 0; i < 2048; i++)
        {
            fprintf(stderr, "%02X", buffer[i]);
        }
        printf("\n");      
        
    }
    //char bitmap[131072];
    int used = 0;
    fseek(ptr, dir_block_id*2048 + 1024 + 2048, SEEK_SET);
    for (int i = 0; i < bit_blocks; i++)
    {
        unsigned char buffer[2048];
        fread(buffer, sizeof(buffer), 1, ptr);
        for (int k = 0; k < 2048; k++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (((buffer[k] >> j) & 1) == 1){
                    used ++;
                }
            }           
        }
        
    }
    printf("\n");
    printf("Bloques libres %d\n", used);
    printf("Bloques disponibles %d\n", number_of_blocks - used);

    

    fclose(ptr);

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

