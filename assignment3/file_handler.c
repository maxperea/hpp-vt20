#include <stdlib.h>
#include <stdio.h>
#include "file_handler.h"

/** 
 * The main purpose of this file is to
 * read data from file and create structure
 * and write to file.
 *
 **/

particle* read_file(char* file_name, int n)
{
  FILE *fp;
  
  fp = fopen(file_name, "r");

  if(fp == NULL)
    {
      perror("Error opening file!\n");
      exit(0);
    }

  particle* data = malloc(n*sizeof(particle));  /* Allocating for array of data */

  double test;
  /* Read in each particle */
  for(int i = 0; i < n; i++)
    {
      if(!fread((data+i), 1, 6*sizeof(double), fp))
        {
          printf("Error reading\n");  /* If fscanf returns 0 no data has been read */
        }
    }

  fclose(fp);
  return data;
}

void write_file(char *file_name, particle* data, int n)
{
  FILE *fp = fopen(file_name, "w");

  for(int i = 0; i < n; i++)
    {
      if(!fwrite((data+i), 1, 6*sizeof(double), fp))
        {
          printf("Error reading\n");  /* If fscanf returns 0 no data has been read */
        }
    }

  fclose(fp);
}
