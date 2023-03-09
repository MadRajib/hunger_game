PKGS=sld2
CFLAGS=-Wall -g -std=c11 -pedantic `pkg-config --cflags sdl2`
LIBS=-lm `pkg-config --libs sdl2`

gp: main.c style.h utils.h utils.c list.h
	$(CC) $(CFLAGS) -o gp main.c utils.c $(LIBS)
