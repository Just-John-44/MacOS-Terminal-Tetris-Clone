// John Wesley Thompson
// Created: 8/10/2024
// Last Edited: 12/30/2024
// tetrose_grid.h

#ifndef _TETROMINOS_H
#define _TETROMINOS_H

#include <ncurses.h>
#include <ctime>
#include <vector>
#include "tetrose_types.h"
#include "tetrose_tetromino.h"
#include "tetrose_bag.h"

// tetrose_GRID CLASS ==========================================================

// These are the normal tetrose dimensions.
const int DEFAULT_HEIGHT = 20;
const int DEFAULT_LENGTH = 10;

class tetrose_grid {
public:

    int height;
    int length;

    tetromino* curr_tet;
    tetromino* next_tet;
    
    tetrose_grid(int y = DEFAULT_HEIGHT, int x = DEFAULT_LENGTH);
    void printGrid(WINDOW* win) const;
    void generateNextTetromino();
    bool setCurrTetrominoOnGrid();
    bool shiftTetromino(int, int); 
    bool rotateTetromino(direction);
    void dropTetromino();
    int clearCompleteRows();

private:

    int tet_y_pos,
        tet_x_pos;

    std::vector<std::vector<square> > grid;

    bag tetrominoes;

    bool peekShiftTetromino(int, int, bool);
    void removeTetromino();
    void placeTetromino();
    bool colliding() const;
};

#endif