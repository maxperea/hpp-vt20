CC = gcc
CFLAGS = -Wall -g
LDFLAGS = 
NAME = january

SRC := $(wildcard *.c)
HDR := $(wildcard *.h);
OBJ = $(SRC:.c=.o)

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)

run: $(NAME)
	./$(NAME)

debug: all
	gdb ./$(NAME)

%.o: %.c %.h
	$(CC) $(CFLAGS) $*.c $*.h -c

%.o: %.c
	$(CC) $(CFLAGS) $< -c

valgrind: all
	valgrind --leak-check=yes ./$(NAME)

clean:
	rm -f $(OBJ) $(NAME) *~
	rm -f *#*
	rm -f *.gch
	rm -f *.gcda
	rm -f *.gcno


.PHONY : valgrind clean run run_test all
