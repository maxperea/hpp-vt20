#ifndef FILE_HANDLER
#define FILE_HANDLER

#include "galsim.h"

particle* read_file(char* file_name, int n);
void write_file(char *file_name, particle* data, int n);

#endif
