
#include <stdio.h>
#include <stdlib.h>

#include "os_API.h"



int main(int argc, char **argv)
{
  char*  input_filename = argv[1]; // dir del disco
  os_mount(input_filename, 4);
  os_mbt();
  
  printf("%d\n",os_exists("reary.png"));
  os_ls();
  os_delete_partition(5);
  //os_reset_mbt();
  //os_mbt();
  os_create_partition(5, 60000);
  os_mount(input_filename, 5);
  os_bitmap(1);
  os_ls();

}


