GCC=gcc
GCCFLAGS=-Iinclude -Wall -g -ansi -pedantic `sdl-config --cflags`
LDFLAGS=-lSDL -lm
OBJS=main.o wadread.o simplegfx.o animation.o unit.o delay.o input.o
BINARY=defense

default: all

%.o: %.c
	$(GCC) $(GCCFLAGS) -c -o $@ $<

all: $(OBJS)
	$(GCC) -o $(BINARY) $(OBJS) $(LDFLAGS)

clean:
	-rm -f $(OBJS) $(BINARY)

