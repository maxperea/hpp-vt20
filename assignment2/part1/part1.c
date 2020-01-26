#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int factorial(int n){
  if (n <= 0)
    return 1;
  return n * factorial(n-1);
}

int over(int n, int k){
  return factorial(n)/(factorial(k)*factorial(n-k));
}

int main(int argc, char** argv) {

  if(argc != 2)
    {
      printf("Usage: ./triang x\n");
      return 0;
    }

  // Assuming argument is int
  int rows = atoi(argv[1]);
  for (int n = 0; n < rows; n++){
    for (int k = 0; k <= n; k++){
      int num = over(n,k);
      printf("%i ", num);
    }
    printf("\n");
  }
  return 0;
}
