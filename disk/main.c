
#include <stdio.h>
#include <stdlib.h>

#include "os_API.h"

int main(int argc, char **argv)
{
  char*  input_filename = argv[1]; // dir del disco
  read_file(input_filename);

}

unsigned int_to_int(unsigned k) {
    if (k == 0) return 0;
    if (k == 1) return 1;                       /* optional */
    return (k % 2) + 10 * int_to_int(k / 2);
}
