CC = gcc
CFLAGS = -Wall -g -fopenmp -O3 -ffast-math 
INCLUDES=-I/opt/X11/include
LDFLAGS=-L/opt/X11/lib -lX11 -lm
NAME = galsim
#ARG = 4 input_data/sun_and_planets_N_4.gal 100000 0.00001 0.4 1
ARG = 2000 input_data/ellipse_N_02000.gal 100 0.00001 0.2119 0 4

SRC := $(wildcard *.c)
HDR := $(wildcard *.h)
OBJ = $(SRC:.c=.o)

#all: $(SRC) $(HDR) $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)

run: all
	./$(NAME) $(ARG)

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
