#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>



int main(int argc, char** argv) {

  if(argc != 2)
    {
      printf("Usage: ./triang x\n");
      return 0;
    }

  int rows = atoi(argv[1]);
  int arr[rows][rows];
  arr[0][0] = 1;
  printf("%i\n", arr[0][0]);

  for (int n = 1; n < rows; n++){
    for (int k = 0; k <= n; k++){
      if (k == 0 || k == n){
        arr[n][k] = 1;
        printf("%i ", arr[n][k]);
      } else {
        arr[n][k] = arr[n-1][k-1] + arr[n-1][k];
        printf("%i ", arr[n][k]);
      }
    }
    printf("\n");
  }
  return 0;
}
