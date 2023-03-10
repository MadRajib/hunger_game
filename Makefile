PKGS=sld2
CFLAGS=-Wall -g -std=gnu11 -pedantic `pkg-config --cflags sdl2`
LIBS=-lm `pkg-config --libs sdl2`
OBJ = main.o agent.o food.o utils.o
HEADERS = style.h list.h

gp: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LIBS) 
	$(MAKE) clean
	
main.o: main.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

agent.o : agent.c agent.h
	$(CC) $(CFLAGS) -c $<

food.o : food.c food.h
	$(CC) $(CFLAGS) -c $<

utils.o : utils.c utils.h
	$(CC) $(CFLAGS) -c $< 

clean:
	rm -f *.o *.gch
