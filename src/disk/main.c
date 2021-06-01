
#include <stdio.h>
#include <stdlib.h>

#include "os_API.h"



int main(int argc, char **argv)
{
  char*  input_filename = argv[1]; // dir del disco
  os_mount(input_filename, 3);
  os_mbt();
  os_bitmap(0x45);
  printf("%d\n",os_exists("reary.png"));

}


