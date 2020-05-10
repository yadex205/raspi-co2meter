# -*- tab-width : 8 -*-

CC      = gcc
CFLAGS  = -O3 -Wall
CLIBS   = -lwiringPi
OBJCOPY = objcopy
SRC     = lcd.c ccs811.c co2meter.c
SRCOBJ  = $(SRC:%.c=%.o)
DATA    = fonts.dat
DATAOBJ = $(DATA:%.dat=%.o)
OBJ     = $(SRCOBJ) $(DATAOBJ)
PROGRAM = co2meter

all: $(PROGRAM)

$(PROGRAM): $(OBJ)
	$(CC) $(OBJ) $(CLIBS) -o $@

$(SRCOBJ): %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(DATAOBJ): %.o: %.dat
	$(OBJCOPY) -I binary -O elf32-littlearm -B arm $< $@

.PHONY: clean
clean:
	rm -f *.o main
