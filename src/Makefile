# -*- Makefile -*-

COMP=g++ 
STD_OPTIONS=-std=c++17 -Wall
NCURSES=-lncurses -lpthread
AUDIO_OPTIONS=-framework CoreFoundation -framework CoreAudio -framework AudioToolbox

all: tetris


tetris: main.cpp tetris_grid.o tetris_tetromino.o tetris_bag.o sound_manager.o sound.o tetris_io.o
	$(COMP) $(STD_OPTIONS) $(NCURSES) $(AUDIO_OPTIONS) main.cpp tetris_grid.o tetris_tetromino.o tetris_bag.o sound_manager.o sound.o tetris_io.o -o tetris

tetris_grid.o: tetris_grid.cpp tetris_grid.h 
	$(COMP) $(STD_OPTIONS) -c tetris_grid.cpp -o tetris_grid.o

tetris_tetromino.o: tetris_tetromino.cpp tetris_tetromino.h
	$(COMP) $(STD_OPTIONS) -c tetris_tetromino.cpp -o tetris_tetromino.o

tetris_bag.o: tetris_bag.cpp tetris_bag.h
	$(COMP) $(STD_OPTIONS) -c tetris_bag.cpp -o tetris_bag.o

sound.o: sound.cpp sound.h s_and_sm_common.h
	$(COMP) $(STD_OPTIONS) -c sound.cpp -o sound.o

sound_manager.o: sound_manager.cpp sound_manager.h s_and_sm_common.h
	$(COMP) $(STD_OPTIONS) -c sound_manager.cpp -o sound_manager.o

tetris_io.o: tetris_io.cpp tetris_io.h
	$(COMP) $(STD_OPTIONS) -c tetris_io.cpp -o tetris_io.o

clean:
	rm -f *.o tetris
