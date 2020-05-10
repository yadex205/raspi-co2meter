# -*- tab-width : 8 -*-

CC      = gcc
CFLAGS  = -O3 -Wall
CLIBS   = -lwiringPi
SRC     = co2meter.c
OBJ     = $(SRC:%.c=%.o)
PROGRAM = co2meter

all: $(PROGRAM)

$(PROGRAM): $(OBJ)
	$(CC) $(OBJ) $(CLIBS) -o $@

$(OBJ): %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f *.o main
