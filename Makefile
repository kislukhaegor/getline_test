CC=g++
FLAGS=-c -Wall -Wextra

SRC=project/src/
INCLUDE=project/include/

all: main

main: main.o utils.o
	$(CC) main.o utils.o -o main.out

main.o: $(SRC)main.c $(INCLUDE)utils.h
	$(CC) -c $(FLAGS) -I$(INCLUDE) $(SRC)main.c

utils.o: $(SRC)utils.c $(INCLUDE)utils.h
	$(CC) -c $(FLAGS) -I$(INCLUDE) $(SRC)utils.c

clean:
	rm -rf *.o main.out