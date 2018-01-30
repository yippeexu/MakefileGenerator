CC = gcc
CC_FLAGS = -g -Wall -std=c11
all: link main.o arraylist.o interpreter.o maker.o source.o string.o types.o 

main.o: main.c
	$(CC) $(CC_FLAGS) -c main.c
arraylist.o: arraylist.c
	$(CC) $(CC_FLAGS) -c arraylist.c
interpreter.o: interpreter.c
	$(CC) $(CC_FLAGS) -c interpreter.c
maker.o: maker.c
	$(CC) $(CC_FLAGS) -c maker.c
source.o: source.c
	$(CC) $(CC_FLAGS) -c source.c
string.o: string.c
	$(CC) $(CC_FLAGS) -c string.c
types.o: types.c
	$(CC) $(CC_FLAGS) -c types.c
link: main.o arraylist.o interpreter.o maker.o source.o string.o types.o 
	$(CC) $(CC_FLAGS) -o makegen main.o arraylist.o interpreter.o maker.o source.o string.o types.o 
clean:
	rm -f *.o makegen