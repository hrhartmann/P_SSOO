
#include <stdio.h>
#include <stdlib.h>

#include "os_API.h"



int main(int argc, char **argv)
{
  char*  input_filename = argv[1]; // dir del disco
  os_mount(input_filename, 3);
  os_mbt();
  os_bitmap(0x45);
  // printf("%d\n",os_exists("reary.png"));

  // Abrimos un archivo del disco
  osFile* file = os_open("jork.png", "r");

  // Copiamos su contenido a un archivo real
  if (file) {
    FILE *write_ptr;
    write_ptr = fopen("output.png","w"); 

    unsigned char buffer[file -> size];
    int read_bytes = os_read(file, buffer, file -> size);
    // for (int i = 0; i < file -> size; i++) {
    //   printf("%i, %i\n", i, buffer[i]);
    //   fprintf(write_ptr, "%i ", buffer[i]);
    // }
    printf("Bytes read: %i", read_bytes);
    fwrite(buffer, 1,file -> size,write_ptr); 
    fclose(write_ptr);

  }  




}


