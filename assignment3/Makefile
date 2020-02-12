CC = gcc
CFLAGS = -Wall -g
INCLUDES=-I/opt/X11/include
LDFLAGS=-L/opt/X11/lib -lX11 -lm
NAME = galsim

SRC := $(wildcard *.c)
HDR := $(wildcard *.h);
OBJ = $(SRC:.c=.o)

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)

run: all
	./$(NAME) 2 input_data/circles_N_2.gal 2 0.001 1

debug: all
	gdb ./$(NAME)

%.o: %.c %.h
	$(CC) $(CFLAGS) $(INCLUDES) $*.c $*.h -c

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
