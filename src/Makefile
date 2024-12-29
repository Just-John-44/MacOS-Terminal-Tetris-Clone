# -*- Makefile -*-

COMP=g++
OPTIONS=-std=c++17 -Wall -lncurses

all: tetris

tetris: test.cpp tetris_grid.o tetris_tetromino.o
	$(COMP) $(OPTIONS) test.cpp tetris_grid.o tetris_tetromino.o -o tetris

tetris_grid.o: tetris_grid.cpp tetris_grid.h
	$(COMP) $(OPTIONS) -c tetris_grid.cpp -o tetris_grid.o

tetris_tetromino.o: tetris_tetromino.cpp tetris_tetromino.h
	$(COMP) $(OPTIONS) -c tetris_tetromino.cpp -o tetris_tetromino.o

clean:
	rm -f *.o tetris
