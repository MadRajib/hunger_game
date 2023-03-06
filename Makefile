PKGS=sld2
CLFAGS=-Wall -ggdb -std=c11 -pedantic `pkg-config --cflags sdl2`
LIBS=-lm `pkg-config --libs sdl2`

gp: main.c style.h utils.h utils.c
	$(CC) $(CFLAGS) -o gp main.c utils.c $(LIBS)
