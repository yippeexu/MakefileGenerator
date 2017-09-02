CC = gcc
CC_VER = -std=c++98
CC_FLAGS = -g -std=c++98

all: example.o main

example.o: example.c
	$(CC) $(CC_FLAGS) -c example.c

main: main.c
	$(CC) -o test $(CC_FLAGS) main.c example.o 

clean:
	rm *.o main