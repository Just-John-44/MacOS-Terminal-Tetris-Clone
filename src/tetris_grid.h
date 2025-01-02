// John Wesley Thompson
// Created: 8/10/2024
// Last Edited: 12/30/2024
// tetris_grid.h

#ifndef _TETROMINOS_H
#define _TETROMINOS_H

#include <ncurses.h>
#include <ctime>
#include <vector>
#include "tetris_types.h"
#include "tetris_tetromino.h"
#include "tetris_bag.h"

// TETRIS_GRID CLASS ==========================================================

// These are the normal tetris dimensions.
const int DEFAULT_HEIGHT = 20;
const int DEFAULT_LENGTH = 10;

class tetris_grid {
public:

    int height;
    int length;

    bag tetrominoes;

    tetromino* curr_tet;
    tetromino* next_tet;
    std::vector<std::vector<square> > grid;
     
    int tet_y_pos,
        tet_x_pos;     

    tetris_grid(int y = DEFAULT_HEIGHT, int x = DEFAULT_LENGTH);
    void printGrid(WINDOW* win);
    void generateNextTetromino();
    bool setCurrTetrominoOnGrid();
    bool shiftTetromino(int, int); 
    bool rotateTetromino(direction);
    void dropTetromino();
    int stackWipeCompleteRows();
    void stackRowsShift();

private:

    

    bool peekShiftTetromino(int, int);
    void removeTetromino();
    void placeTetromino();
    bool colliding();
};

#endif