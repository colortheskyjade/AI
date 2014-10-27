CC = g++
CFLAGS = -O3
EXEC = PuzzleGenerator
	
all: node.o puzzle.o
	$(CC) $(CFLAGS) -o $(EXEC) main.cpp node.o puzzle.o

node.o: node.h node.cpp
	$(CC) -g -Wall -c -o node.o node.cpp

puzzle.o: puzzle.h puzzle.cpp
	$(CC) -g -Wall -c -o puzzle.o puzzle.cpp
	
clean:
	rm -rf $(EXEC) *.o

test:
	make clean		
	make all
	./$(EXEC) 10 10 1 9
	