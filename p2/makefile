CC = g++
CFLAGS = -O3
EXEC = PuzzleGenerator
	
all: node.o
	$(CC) $(CFLAGS) -o $(EXEC) main.cpp node.o

node.o: node.h node.cpp
	$(CC) -g -Wall -c -o node.o node.cpp
	
clean:
	rm -rf $(EXEC) *.o

test:
	make clean		
	make all
	./$(EXEC) 5 5 2 4
	