OBJ = main.o agent.o food.o utils.o
INCLUDE_DIR=include/
SRC_DIR=src

PKGS=sld2
CFLAGS=-Wall -g -std=gnu11 `pkg-config --cflags sdl2`
LIBS=-lm `pkg-config --libs sdl2` -lSDL2_ttf

gp: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LIBS) 
	$(MAKE) clean
	
main.o: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) $(INCLUDE_DIR:%=-I %) -c $<

agent.o : $(SRC_DIR)/agent.c
	$(CC) $(CFLAGS) $(INCLUDE_DIR:%=-I %) -c $<

food.o : $(SRC_DIR)/food.c
	$(CC) $(CFLAGS) $(INCLUDE_DIR:%=-I %) -c $<

utils.o : $(SRC_DIR)/utils.c
	$(CC) $(CFLAGS) $(INCLUDE_DIR:%=-I %) -c $< 

clean:
	rm -f *.o *.gch
