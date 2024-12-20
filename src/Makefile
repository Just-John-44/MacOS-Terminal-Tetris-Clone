# -*- Makefile -*-

COMP=g++
OPTIONS=-std=c++17 -Wall -lncurses

all: tetris

tetris: test.cpp tetris.o tetris_stack.o tetris_tetromino.o
	$(COMP) $(OPTIONS) test.cpp tetris.o tetris_stack.o tetris_tetromino.o -o tetris

tetris.o: tetris.cpp tetris.h
	$(COMP) $(OPTIONS) -c tetris.cpp -o tetris.o

tetris_stack.o: tetris_stack.cpp tetris_stack.h
	$(COMP) $(OPTIONS) -c tetris_stack.cpp -o tetris_stack.o

tetris_tetromino.o: tetris_tetromino.cpp tetris_tetromino.h
	$(COMP) $(OPTIONS) -c tetris_tetromino.cpp -o tetris_tetromino.o

clean:
	rm -f *.o tetris