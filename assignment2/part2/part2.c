#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int main() {

  FILE *fp = fopen("little_bin_file", "r");

  if(fp == NULL)
    {
      perror("Error opening file!\n");
      exit(0);
    }

  int i;
  double d;
  char c;
  float f;

  fread(&i, 1, sizeof(int), fp);
  fread(&d, 1, sizeof(double), fp);
  fread(&c, 1, sizeof(char), fp);
  fread(&f, 1, sizeof(float), fp);

  printf("%d\n%f\n%c\n%f\n", i,d,c,f);

  fclose(fp);
  
  return 0;
}
