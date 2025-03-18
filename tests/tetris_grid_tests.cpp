// This is to test and debug the tetris_grid class

#include <iostream>
//#include <ncurses.h>
#include "tetris_grid.h"

int main(){

    initscr();
    tetris_grid grid;

    grid.setCurrTetrominoOnGrid();

    for (int i = 0; i < grid.GRID_HEIGHT; i++){
        for (int j = 0; j < grid.GRID_LENGTH; j++){
            std::cout << grid.grid.at(i).at(j).s_type << " ";
        }
        std::cout << "\r\n";
    }


    return 0;
}