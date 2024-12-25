// John Wesley Thompson
// Created: 8/10/2024
// Completed:
// Last Edited: 12/24/2024
// tetris_grid.h

#ifndef _TETROMINOS_H
#define _TETROMINOS_H

#include <ncurses.h>
#include <ctime>
#include <vector>
#include "tetris_types.h"
#include "tetris_tetromino.h"
#include "tetris_stack.h"

// TETRIS_GRID CLASS ==========================================================

class tetris_grid {
public:

    tetromino* curr_tet;
    tetromino* next_tet;
    std::vector<std::vector<square> > grid;
    
    // int grid_pos;     
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

    tetris_grid();
    void printInfo(WINDOW*);
    void printGrid(WINDOW*);
    void refreshTetromino(WINDOW*);
    void generateNextTetromino();
    bool setCurrTetrominoOnGrid();
    bool shiftTetromino(int, int); 
    bool rotateTetromino(direction);
    void stackWipeCompleteRows(WINDOW*);
    void stackRowsShift(WINDOW*);

private:
    bool peekShiftTetromino(int, int);
    void removeTetromino();
    void placeTetromino();
    bool colliding();

    bool inBounds(int, int);
};

#endif