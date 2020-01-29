#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct day_list{
  struct day_list *next;
  int index;
  float min, max;
} list_t;

void delete_day(list_t *list, int index){
  list_t *current = list->next;
  list_t *previous = list;
  while(current != NULL && current->index != index){
    previous = current;
    current = current->next;
  }
  if (current == NULL){
    printf("Could not find entry to delete.\n");
    return;
  }
  previous->next = current->next;
  free(current);
}


void insert_day(list_t *list, int index, float min, float max){
   if(index > 31 || index < 1)
    {
      printf("Invalid input day, no entry inserted.\n");
    }
  list_t *current = list->next;
  list_t *previous = list;
  while(current != NULL && current->index < index){
    previous = current;
    current = current->next;
  }

  list_t *new = calloc(sizeof(list_t),1);
  new->index = index;
  new->min = min;
  new->max = max;

  if(current == NULL || current->index > index)
    {
      new->next = current;
      previous->next = new;
    }
  else
    {
      new->next = current->next;
      previous->next = new;
      free(current);
    }
}

void print_list(list_t *list){
  printf("day\tmin\t\tmax\n");
  list_t *current = list;

  while (current->next){
    printf("%i\t%f\t%f\n", current->next->index, current->next->min, current->next->max);
    current = current->next;
  } 
}

void free_list(list_t *root)
{
  list_t *current = root;
  list_t *prev;

  while(current != NULL)
    {
      prev = current;
      current = current->next;
      free(prev);
    }
}

void flush_input()
{
  int c;
  do {
    c = getchar();
  } while(c != '\n' && c != EOF);
}

bool get_input(double* input, int n)
{
  
  for(int i = 0; i < n; i++)
    {
      if (scanf(" %lf", &input[i]) != 1)
        {
          printf("Invalid input.\n");
          flush_input();
          return false;
        }
    }
  flush_input();
  return true;
}

int main()
{
  list_t *root = calloc(sizeof(list_t),1);

  char cmd;
  double input[4];

  while(true)
    {
      printf("Enter command: ");
      scanf("%c", &cmd);

      cmd = toupper(cmd);
      
      switch(cmd)
        {
        case 'A':
          if(get_input(input, 3))
            insert_day(root, input[0], input[1], input[2]);
          break;

        case 'D':
          if(get_input(input, 1))
            delete_day(root, (int)input[0]);
          break;

        case 'P':
          print_list(root);
          flush_input();
          break;

        case 'Q':
          flush_input();
          free_list(root);
          return 0;

        default:
          printf("Invalid input. \n");
        }
    }
}
