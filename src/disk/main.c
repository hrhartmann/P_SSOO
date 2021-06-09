#include <stdio.h>
#include <stdlib.h>

#include "os_API.h"



int main(int argc, char **argv)
{
  char*  input_filename = argv[1]; // dir del disco
  printf("Monatmos\n");
  os_mount(input_filename, 4);
  os_mbt();

  os_ls();
  // if (os_exists("pepeloni.txt") == 1){
  //   printf("El archivo pepeloni.txt existe\n");
  // }else{
  //   printf("El archivo no existe\n");
  // }

  // if (os_exists("pepeloni2.txt") == 1){
  //   printf("El archivo pepeloni2.txt existe\n");
  // }else{
  //   printf("El archivo no existe\n");
  // }

  // os_bitmap(0);


  // printf("Creamos una particion de size 500000, osbervamos donde se coloca\n");
  // os_create_partition(5, 50000);
  // os_delete_partition(5);

  // printf("Creamos una particion de size 1000000, osbervamos donde se coloca\n");
  // os_create_partition(5, 100000);
  // printf("Montamos la particion 5\n");
  // os_mount(input_filename, 5);
  // //os_delete_partition(5);

  // os_bitmap(0);






  //os_reset_mbt() FINAAAAAAAAAAAAAL


  


  


  osFile* osfile = os_open("mercedes_benz.mp4", "r");
  unsigned char buffer[osfile -> size];
  int n = os_read(osfile, buffer, osfile->size);
  //int n = os_read(osfile, buffer, 20000000);
  printf("Se leyeron %d bytes\n", n);
  FILE* filenew = fopen("outpu2t.mp4", "w");
  fwrite(buffer, osfile -> size, 1, filenew);
  fclose(filenew);
  os_close(osfile);


  osFile* osfile3 = os_open("pepeloni2.txt", "w");
  
  unsigned char buffer3[10000];


  for (int i = 0; i < 10000; i++)
  {
    unsigned char a = 'a';
    buffer3[i] = a;
  }
  os_write(osfile3, buffer3, 10000);
  //os_save();
  os_close(osfile3);

  if (os_exists("pepeloni2.txt") == 1){
    printf("El archivo pepeloni2.txt existe\n");
  }else{
    printf("El archivo no existe\n");
  }

  osFile* osfile2 = os_open("pepeloni2.txt", "r");
  unsigned char buffer2[osfile2 -> size];
  int n2 = os_read(osfile2, buffer2, osfile2->size);
  //int n = os_read(osfile, buffer, 20000000);
  printf("Se leyeron %d bytes\n", n2);
  FILE* filenew2 = fopen("pepeloni2.txt", "w+b");
  fwrite(buffer, osfile2 -> size, 1, filenew2);
  fclose(filenew2);
  os_close(osfile2);

  os_rm("pepeloni2.txt");



  
  

  
  // // printf("%d\n",os_exists("reary.png"));
  // // os_ls();
  
  

};  




