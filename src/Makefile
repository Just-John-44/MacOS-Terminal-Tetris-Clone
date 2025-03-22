# -*- Makefile -*-

COMP=g++
OPTIONS=-std=c++17 -Wall -lncurses -lpthread -lm -framework CoreFoundation -framework CoreAudio -framework AudioToolbox

all: tetris

tetris: test.cpp tetris_grid.o tetris_tetromino.o tetris_bag.o sound_manager.o
	$(COMP) $(OPTIONS) test.cpp tetris_grid.o tetris_tetromino.o tetris_bag.o sound_manager.o -o tetris

tetris_grid.o: tetris_grid.cpp tetris_grid.h 
	$(COMP) $(OPTIONS) -c tetris_grid.cpp -o tetris_grid.o

tetris_tetromino.o: tetris_tetromino.cpp tetris_tetromino.h
	$(COMP) $(OPTIONS) -c tetris_tetromino.cpp -o tetris_tetromino.o

tetris_bag.o: tetris_bag.cpp tetris_bag.h
	$(COMP) $(OPTIONS) -c tetris_bag.cpp -o tetris_bag.o

sound_manager.o: sound_manager.cpp sound_manager.h
	$(COMP) $(OPTIONS) -c sound_manager.cpp -o sound_manager.o


clean:
	rm -f *.o tetris
