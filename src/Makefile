# -*- Makefile -*-

COMP = g++ 
STD_OPTIONS = -std=c++17 -Wall
NCURSES = -lncurses -lpthread
AUDIO_OPTIONS = -framework CoreFoundation -framework CoreAudio -framework AudioToolbox

OBJS = tetrose_grid.o tetrose_tetromino.o tetrose_bag.o sound_manager.o sound.o tetrose_io.o

all:tetrose 


tetrose: main.cpp $(OBJS)
	$(COMP) $(STD_OPTIONS) $(NCURSES) $(AUDIO_OPTIONS) main.cpp $(OBJS) -o tetrose 

tetrose_grid.o: tetrose_grid.cpp tetrose_grid.h 
	$(COMP) $(STD_OPTIONS) -c tetrose_grid.cpp -o tetrose_grid.o

tetrose_tetromino.o: tetrose_tetromino.cpp tetrose_tetromino.h
	$(COMP) $(STD_OPTIONS) -c tetrose_tetromino.cpp -o tetrose_tetromino.o

tetrose_bag.o: tetrose_bag.cpp tetrose_bag.h
	$(COMP) $(STD_OPTIONS) -c tetrose_bag.cpp -o tetrose_bag.o

sound.o: sound.cpp sound.h s_and_sm_common.h
	$(COMP) $(STD_OPTIONS) -c sound.cpp -o sound.o

sound_manager.o: sound_manager.cpp sound_manager.h s_and_sm_common.h
	$(COMP) $(STD_OPTIONS) -c sound_manager.cpp -o sound_manager.o

tetrose_io.o: tetrose_io.cpp tetrose_io.h
	$(COMP) $(STD_OPTIONS) -c tetrose_io.cpp -o tetrose_io.o

.PHONY: clean
clean:
	rm -f *.o tetrose 
