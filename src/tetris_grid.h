// John Wesley Thompson
// Created: 8/10/2024
// Last Edited: 12/30/2024
// tetris_grid.h

#ifndef _TETROMINOS_H
#define _TETROMINOS_H

#include <ncurses.h>
#include <ctime>
#include <vector>
#include <random>
#include "tetris_types.h"
#include "tetris_tetromino.h"

// TETRIS_GRID CLASS ==========================================================

// These are the normal tetris dimensions.
const int DEFAULT_HEIGHT = 20;
const int DEFAULT_LENGTH = 10;

class tetris_grid {
public:

    int height;
    int length;

    tetromino* curr_tet;
    tetromino* next_tet;
    std::vector<std::vector<square> > grid;
     
    int tet_y_pos,
        tet_x_pos;     
    
    tetromino tetris_tetrominoes[7] = 
    {
        tetromino(L_TETROMINO),
        tetromino(J_TETROMINO),
        tetromino(T_TETROMINO),
        tetromino(I_TETROMINO),
        tetromino(S_TETROMINO),
        tetromino(Z_TETROMINO),
        tetromino(O_TETROMINO)
    };

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

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> dist;

    bool peekShiftTetromino(int, int);
    void removeTetromino();
    void placeTetromino();
    bool colliding();
};

#endif