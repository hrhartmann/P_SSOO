#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <string.h> // strtok, strcpy, etc.
#include <stdlib.h> // malloc, calloc, free, etc.
#include <math.h>
#include <stdint.h>
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
    //int last_bits = number_of_blocks % 16384;
    //printf("BORRAR LUEGO, EL DIR_BLOCK ID ES %d\n\n",dir_block_id);
    
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
        //printf("DEBUDDGGGGG tenemos  y estoy en %ld\n",ftell(ptr));
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
    printf("Bloques usados %d\n", used);
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
        //printf("NOmbre del file %s\n",actual_filename);
        if (!strcmp(actual_filename, filename)){
            return 1;
        }
        
        
        /* code */
    }
    return 0;

}

void os_ls(){
    // partitionData* pd = get_partition_data();
    // int block_id = pd -> dir_block_id;
    int block_id = dir_block_id;

    
    FILE *ptr;
    ptr = fopen(disk_route, "rb");
    fseek(ptr, block_id*2048 + 1024, SEEK_SET);
    
    for (int i = 0; i < 64; i++){
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
        printf("Nombre del file %s\n",actual_filename);
       
        /* code */
    }


}

void os_delete_partition(int id){

    unsigned char buffer[1024];
    FILE *ptr;
    ptr = fopen(disk_route,"r+b");
    fread(buffer,sizeof(buffer),1,ptr);
    for(int i = 0; i<1024; i+=8){
        int binary[8];
        for(int n = 0; n < 8; n++)
            binary[7-n] = (buffer[i] >> n) & 1;

        if (binary[0] == 1){                                      
            if(buffer[i]-128 == id){
                printf("ENCONTRE PARTICION %d en el i %d\n", buffer[i] - 128, i);
                fseek(ptr, i, SEEK_SET);
                unsigned char newByte = 0x00;
                fwrite(&newByte, sizeof(newByte), 1, ptr);
            }
        }    
    }
    
    fclose(ptr);

}

void os_reset_mbt(){
    unsigned char buffer[1024];
    FILE *ptr;
    ptr = fopen(disk_route,"r+b");
    fread(buffer,sizeof(buffer),1,ptr);
    for(int i = 0; i<1024; i+=8){

        fseek(ptr, i, SEEK_SET);
        unsigned char newByte = 0x00;
        fwrite(&newByte, sizeof(newByte), 1, ptr);  
    }
    
    fclose(ptr);
}

int comp (const void * elem1, const void * elem2){
    //printf("LLEGUE\n");
    // const int* f = *(const int**)elem1;
    // const int* q = *(const int**)elem2;
    const int* f = elem1;
    const int* q = elem2;
    //printf("Se compara %d con %d", f[0], q[0]);
    if (f[0] > q[0]) return 1;
    if (f[0] < q[0]) return -1;
    return 0;
}

void os_create_partition(int id, int size){
    // Encontrar espacios ocupados
    int used[128][2];
    int num = 0;
    unsigned char buffer[1024];
    FILE *ptr;
    ptr = fopen(disk_route,"r+b");
    fread(buffer,sizeof(buffer),1,ptr);
    for(int i = 0; i<1024; i+=8){
        int binary[8];
        for(int n = 0; n < 8; n++)
            binary[7-n] = (buffer[i] >> n) & 1;

        if (binary[0] == 1){      
            if (id == buffer[i]-128){
                printf("Particion ya existe\n");
                return;
            }                                
           
            int block_number = buffer[i+1] << 16 | buffer[i+2] << 8 | buffer[i+3];
            int ctd = buffer[i+4] << 24 | buffer[i+5] << 16 | buffer[i+6] << 8 | buffer[i+7];
            num++;
            used[i/8][0] = block_number;
            used[i/8][1] = ctd ;
            
        }    
    }
    used[num][0] = 0;
    used[num][1] = 0;
    num ++;
    used[num][0] = 2097152;
    used[num][1] = 0;
    num ++;
    qsort(used, num,sizeof(used[0]), comp);
    //printf("DESPUES DEL SORT\n");
    for (int i = 0; i < num; i++)
    {
        printf("Ocupado desde %d hasta %d\n", used[i][0], used[i][0] + used[i][1]);
    }
    

    for (int i = 0; i < num-1; i++)
    {
        
        int from = used[i][0] + used[i][1];
        int to = used[i+1][0];
        int available = to - from;
        if (available >= size) {
            printf("va entre %d y %d \n", from, from + size);
            //break;

            for(int i = 0; i<1024; i+=8){
                int binary[8];
                for(int n = 0; n < 8; n++)
                    binary[7-n] = (buffer[i] >> n) & 1;

                if (binary[0] == 0){                                    
                    // creamos una entrada en mbt con los elementos necesarios.
                    fseek(ptr, i, SEEK_SET);
                    unsigned char newByte = 128+id;
                    fwrite(&newByte, sizeof(newByte), 1, ptr); 
                    unsigned char pos[3] = {(from>>16) & 0xFF,(from>>8) & 0xFF,(from>>0) & 0xFF};
                    for (size_t i = 0; i < 3; i++)
                    {
                        //printf("Byte de from %u\n", pos[i]);
                        fwrite(&pos[i], sizeof(pos[i]), 1, ptr); 
                    }
                    unsigned char sizeb[4] = {(size>>24) & 0xFF,(size>>16) & 0xFF,(size>>8) & 0xFF,(size>>0) & 0xFF};
                    for (size_t i = 0; i < 4; i++)
                    {
                        //printf("Byte de size %u\n", sizeb[i]);
                        fwrite(&sizeb[i], sizeof(sizeb[i]), 1, ptr); 
                    }
                    // Ahora instanciamos la particion en los bloques de memoria
                    fseek(ptr, from*2048 + 1024, SEEK_SET);
                    //printf("FROm es %d\n", from);
                    // Lleno de 0 el primer byte de entradas de directorio
                    for (int i = 0; i < 2048; i++)
                    {
                        unsigned char newByte = 0x00;
                        fwrite(&newByte, sizeof(newByte), 1, ptr); 
                    }
                    // Consistencia al bitmap, primer bloque es directorio: 1, los bloques de bitmap son 1.
                    //fseek(ptr, from*2048 + 1024 + 2048, SEEK_SET);
                    int num_bitmap =  ceil((double) size/16384);
                    int cuent = 128;
                    if (num_bitmap == 8){
                        unsigned char newByte = 0xFF;
                        fwrite(&newByte, sizeof(newByte), 1, ptr); 
                        unsigned char newByte2 = 128;
                        fwrite(&newByte2, sizeof(newByte2), 1, ptr); 


                    }else{
                        for (int i = 0; i < num_bitmap; i++){
                            cuent+= pow(2,6-i);
                            //printf("ITERACION %d CUENT ES %d Y SE LE SUMA %d\n", i, cuent, 2^(6-i));
                        }
                        //printf("DEBUDDGGGGG tenemos cuent = %d y num_bitmap = %d y estoy en %ld\n", cuent, num_bitmap, ftell(ptr));
                        unsigned char newByte = cuent;
                        fwrite(&newByte, sizeof(newByte), 1, ptr);

                    }
                
                    break;                    
                }    
            }
            break;

        }
    }
  
    fclose(ptr);  
}

osFile* os_open(char* filename, char* mode) {
    // printf("=====os_open=====\n");
    // Creación del osFile
    osFile* os_file = malloc(sizeof(osFile));
    os_file -> last_byte_read = -1;
    os_file -> is_open = 1;

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
            // printf("The direction for the file is: %d\n", buffer[0]);
          
                char actual_filename[28];
                for (size_t i = 4; i < 32; i++) {
                    actual_filename[i-4] = buffer[i];
                }
                if (!strcmp(actual_filename, filename)) {
                    os_file -> dir_block_entry = i*32;
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

void save_file(osFile* file_desc, int saved_space)
{
    FILE *ptr;
    ptr = fopen(disk_route,"r+b");

    printf("saving file...\n");

    //     int partition_abs_path = dir_block_id*2048 + 1024;
    int index_block_abs_path = (file_desc -> index_block_relative_dir*2048) + 1024;
    fseek(ptr, index_block_abs_path, SEEK_SET);
    // unsigned char ibbuffer[2048]; // index block buffer
    // fread(ibbuffer, sizeof(ibbuffer), 1, ptr);
    // readfile -> size = ibbuffer[0] << 32 | ibbuffer[1] << 24 | ibbuffer[2] << 16 | ibbuffer[3] << 8 | ibbuffer[4];
    long int new_size = saved_space + file_desc -> size;
    unsigned char size_buffer[5] = {(new_size>>32) & 0xFF, (new_size>>24) & 0xFF,(new_size>>16) & 0xFF,(new_size>>8) & 0xFF,(new_size>>0) & 0xFF};
    for (int k=0; k < 5; k++)
    {
        // logica para guardar un int size en 5 bytes
        fwrite(&size_buffer[k], sizeof(size_buffer[k]), 1, ptr);
    }

    for (int i=0; i < 681; i++)
    {
        fseek(ptr, index_block_abs_path + 5 + i, SEEK_SET); // seek block pointer
        int bp = file_desc -> block_pointers[i];
        unsigned char block_pointer[3] = {(bp>>16) & 0xFF,(bp>>8) & 0xFF,(bp>>0) & 0xFF};
        for (int u=0; u < 3; u++)
        {          
            fwrite(&block_pointer[u], sizeof(block_pointer[u]), 1, ptr);
        }
    }
    fclose(ptr);
};

int os_close(osFile* file_desc)
{
     if (file_desc -> is_open)
     {
         file_desc -> is_open = 0;
         free(file_desc -> block_pointers);
         free(file_desc);
         return 1;
     } else {
         // error el archivo estaba ya cerrado
         printf("The file was already closed. \n");
         return 0;
     }
}

int os_write(osFile* file_desc, void* buffer, int nbytes)
{
    // if (file_desc -> mode == "w" )
    // unsigned char buffer = buffer;
    unsigned char* etherhead = (unsigned char*)buffer;
    int blocks_to_use = ceil( (double)nbytes / 2048);
    int bytes_to_write = nbytes;
    FILE *ptr;
    ptr = fopen(disk_route,"r+b");
    // int leftover_bytes = 0;
    printf("Blocks to use: %d\n", blocks_to_use);
    for (int i=0; i < blocks_to_use; i++)
    {
        if (bytes_to_write <= 0)
            {
                int written_bytes = nbytes;
                // file_desc -> last_block_free_bytes = leftover_bytes;
                save_file(file_desc, written_bytes);
                fclose(ptr);
                return written_bytes;
            } 
        int empty_block_relative_dir = getset_empty_block();
        printf("empty relative dir: %d\n", empty_block_relative_dir);
        if (empty_block_relative_dir > 0)
        {
            // printf("Before block pointers def\n");
            int file_num_of_blocks = file_desc -> number_of_blocks;
            file_desc -> block_pointers[file_num_of_blocks] = empty_block_relative_dir;
            file_desc -> number_of_blocks++;
            // printf("After block pointers def\n");
            // save the pointer
            
            if (file_desc -> number_of_blocks >= 681)
            {
                // error max file size reached
                int written_bytes = nbytes - bytes_to_write;
                // file_desc -> last_block_free_bytes = leftover_bytes;
                save_file(file_desc, written_bytes);
                fclose(ptr);
                return written_bytes;
            }
            int index_abs_path = (file_desc -> index_block_relative_dir*2048) + 1024;
            // int index_block_abs_path = partition_abs_path + (file_desc -> index_block_relative_dir);
            int abs_dir = index_abs_path + 2048*empty_block_relative_dir;
            fseek(ptr, abs_dir, SEEK_SET); // this block is empty
            int block_bytes_to_write = 0;
            // int lbfb = file_desc -> last_block_free_bytes;
            
            // if (lbfb) {
            //     // write the leftover of the previous block
            //     abs_dir -= lbfb;
            //     if (lbfb >= bytes_to_write)
            //     {
            //         block_bytes_to_write = bytes_to_write;
            //         leftover_bytes += lbfb - bytes_to_write;
            //     } else {
            //         block_bytes_to_write = lbfb;
            //     }
            //     // file_desc -> last_block_free_bytes = 0;
            // } else 
            
            if (bytes_to_write > 2048)
            {
                block_bytes_to_write = 2048;
            } else {
                block_bytes_to_write = bytes_to_write;
                // leftover_bytes = 2048 - bytes_to_write;
            }
            for (int wbyte=0; wbyte < block_bytes_to_write; wbyte++)
                {
                    printf("writing bytes...\n");
                    fseek(ptr, abs_dir + wbyte, SEEK_SET);
                    unsigned char newByte = etherhead[nbytes - bytes_to_write]; //error here -> fix
                    fwrite(&newByte, sizeof(newByte), 1, ptr);
                    bytes_to_write -= 1;
                }

        } else {
            // error failed to get an empty block, finishes
            int written_bytes = nbytes - bytes_to_write;
            // file_desc -> last_block_free_bytes = leftover_bytes;
            save_file(file_desc, written_bytes);
            fclose(ptr);
            return written_bytes;

        }
    }
    
    int written_bytes = nbytes - bytes_to_write;
    // file_desc -> last_block_free_bytes = leftover_bytes;
    save_file(file_desc, written_bytes);
    fclose(ptr);
    return written_bytes;

}

int getset_empty_block()
{
    FILE *ptr;
    ptr = fopen(disk_route, "rb");

    for (int bitmap_block=0; bitmap_block < (number_of_blocks / 16384) + 1; bitmap_block++) 
    {
        unsigned char buffer[2048];
        int partition_abs_path = dir_block_id*2048 + 1024;
        int abs_bitmap_path = partition_abs_path + 2048 + (bitmap_block * 2048);
        fseek(ptr, abs_bitmap_path, SEEK_SET); // bitmap

        fread(buffer,sizeof(buffer),1,ptr);
        for (int i=0; i < 2048; i++)
        {
            int binary[8];

            for(int n = 0; n < 8; n++)
                binary[7-n] = (buffer[i] >> n) & 1;

            for(int j = 0; j < 8; j++)
            {
                if (!binary[j])
                {
                    int empty_block = (bitmap_block*16384) + (8*i) + j;
                    // set bitmap value to 1                
                    binary[j] = 1;
                    fseek(ptr, partition_abs_path + 2048 + empty_block, SEEK_SET);
                    int value = 0;
                    int twopower = 1;
                    for (int l=7; l >= 0; l--)
                    {
                        value += binary[l] * twopower;
                        twopower = twopower * 2;
                    }
                    unsigned char newByte = value;
                    fwrite(&newByte, sizeof(newByte), 1, ptr);
                    fclose(ptr);
                    return empty_block;
                }
            }
        }
      }
    fclose(ptr);
    return -1;
};

int os_rm(char* filename)
{
    osFile* rm_file = os_open(filename, "r");
    FILE *ptr;
    ptr = fopen(disk_route,"r+b");
    int partition_abs_path = dir_block_id*2048 + 1024;
    int file_dir_block_entry = partition_abs_path + (rm_file -> dir_block_entry);
    fseek(ptr, file_dir_block_entry, SEEK_SET); // directory block -> file entry
    printf("Removing file\n");
    unsigned char newByte = 0x00; // set first byte to 0
    fwrite(&newByte, sizeof(newByte), 1, ptr);

    printf("After write bytes\n");

    for (int bitmap_block = 0; bitmap_block < ceil((float)number_of_blocks / 16384); bitmap_block++)
    {
        if (rm_file -> number_of_blocks == 0)
        {
            fclose(ptr);
            return 1;
        }
        printf("Before bitmaspopathj\n");
        int bitmap_path = partition_abs_path + 2048 + (bitmap_block*2048); 
        fseek(ptr, bitmap_path, SEEK_SET); // bitmap block
        unsigned char buffer[2048];
        fread(buffer,sizeof(buffer),1,ptr); // read bitmap
        int file_blocks_left = rm_file -> number_of_blocks;
        printf("After bitmaspopathj\n");

        for (int i=0; i < file_blocks_left; i++)
        {
            int block_pointer = rm_file -> block_pointers[i];
            int bitmap_size =  bitmap_block*2048;
            if (block_pointer > bitmap_size && block_pointer < bitmap_size + 2048)
            {    
                int bitmap_byte = block_pointer / 8;
                fseek(ptr, bitmap_path + bitmap_byte, SEEK_SET);
                fread(buffer,sizeof(buffer),1,ptr); // read bitmap
                int block_pointer_bit = block_pointer % 8; // hh error check if this works
                int binary[8];
                for(int n = 0; n < 8; n++)
                    {
                        int bitmap_byte_value = bitmap_byte - (bitmap_block * 2048);
                        printf("bitmap_byte_value: %d", bitmap_byte_value);
                    binary[7-n] = (buffer[bitmap_byte] >> n) & 1;
                    }

                binary[block_pointer_bit] = 0;
                // unsigned char newByte = binary;
                int value = 0;
                int twopower = 1;
                for (int l=7; l >= 0; l--)
                {
                    value += binary[l] * twopower;
                    twopower = twopower * 2;
                }
                unsigned char newByte = value;
                fwrite(&newByte, sizeof(newByte), 1, ptr); // hh error test if this works
                rm_file -> number_of_blocks--;
            }
        }
    }
    fclose(ptr);
    return 1;
}