#include <stdio.h>
#include <stdlib.h>

#include "os_API.h"



int main(int argc, char **argv)
{
  char*  input_filename = argv[1]; // dir del disco
  os_mount(input_filename, 4);
  os_ls();

  osFile* osfile = os_open("pepeloni.txt", "r");
  unsigned char buffer[osfile -> size];

  FILE* nombrewea = fopen("output.txt", "w");

  fwrite(buffer, osfile -> size, 1, nombrewea);
  fclose(nombrewea);

  
  // printf("%d\n",os_exists("reary.png"));
  // os_ls();
  
  

};  




