#include <stdlib.h>
#include <stdio.h>
#include "file_handler.h"

int main(int argc, char* argv[])
{
  char* filename = argv[1];

  read_file(filename, 2);
}
