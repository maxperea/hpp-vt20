#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct day_list{
  struct day_list *next;
  int index;
  float min;
  float max;
} list_t;

void delete_day(list_t *list, int index){
  list_t *current = list;
  list_t *previous = list;
  while(current != NULL && current->index != index){
    previous = current;
    current = current->next;
  }
  if (current == NULL){
    printf("Could not find entry.\n");
    return;
  }
  previous->next = current->next;
  free(current);
}


void insert_day(list_t *list, int index, float min, float max){
  list_t *current = list;
  list_t *previous = list;
  while(current != NULL && current->index < index){
    previous = current;
    current = current->next;
  }
  list_t *new = calloc(sizeof(list_t),1);
  new->index = index;
  new->min = min;
  new->max = max;
  new->next = current;
  previous->next = new;
}

void print_list(list_t *list){
  printf("day\tmin\t\tmax\n");
  list_t *current = list;
  while (current->next){
    printf("%i\t%f\t%f\n", current->next->index, current->next->min, current->next->max);
    current = current->next;
  }
}

int main() {

  list_t *first = calloc(sizeof(list_t),1);

  insert_day(first, 1, 5.0, 15.2);
  insert_day(first, 5, 6.0, 12.2);
  insert_day(first, 3, 2.0, 11.2);
  print_list(first);
  delete_day(first, 3);
  print_list(first);
  delete_day(first, 1);
  delete_day(first, 5);
  print_list(first);

  free(first);

  return 0;
}
