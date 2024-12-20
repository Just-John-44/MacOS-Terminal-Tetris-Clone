// John Wesley Thompson
// Created: 8/10/2024
// Completed:
// Last Edited: 10/17/2024
// tetris.h

#ifndef _TETROMINOS_H
#define _TETROMINOS_H

#include <ncurses.h>
#include <ctime>
#include <vector>
#include "tetris_stack.h"
#include "tetris_tetromino.h"

// DATA =======================================================================


#define COLOR_PURPLE 8  // All other colors used are defined in the ncurses 
#define COLOR_ORANGE 9  // header file.



enum direction {
    UP,
    DOWN,
    LEFT,
    RIGHT, 
    CWISE,
    CCWISE
};

#ifndef ENUM_COLOR_ID
#define ENUM_COLOR_ID
enum color_id{
    STANDARD = 1,
    ORANGE,
    BLUE,
    PURPLE,
    CYAN,
    GREEN,
    RED,
    YELLOW
};
#endif

// ============================================================================

inline void tetris_init(){
    unsigned int generator_seed = time(0);
    srand(generator_seed);

    curs_set(0);

    if (has_colors()){
        start_color();

        if (can_change_color()){
            
            init_color(COLOR_PURPLE, 626, 125, 940);
            init_color(COLOR_ORANGE, 999, 646, 0);     
            
            init_pair(PURPLE, COLOR_PURPLE, COLOR_BLACK);
            init_pair(ORANGE, COLOR_ORANGE, COLOR_BLACK);
        } 

        init_pair(STANDARD, COLOR_WHITE, COLOR_BLACK);
        init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
        init_pair(CYAN, COLOR_CYAN, COLOR_BLACK);
        init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
        init_pair(RED, COLOR_RED, COLOR_BLACK);
        init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);

        bkgd(COLOR_PAIR(STANDARD));
    }
}

// TETRIS_GRID CLASS ==========================================================

class tetris_grid {
public:

    tetromino curr_tetromino;
    tetromino next_tetromino;
    const int* m_curr_rotation_offset;
    const int* m_curr_arr_start;
    int m_curr_shape_stride;
    int m_curr_shape_arr_len;
    int topmost_square_offset;
    int bottommost_square_offset;
    int leftmost_square_offset;
    int rightmost_square_offset;
    std::vector<std::vector<square> > grid;
    // This is the index at the bottom left of the tetromino thats in the grid
    // int grid_tetromino_location
    
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
    void rotateTetromino(direction);
    void generateNextTetromino();
    void setCurrTetrominoOnGrid();
    void shiftTetromino(int, direction); 

    bool stackRowFull(int);
    void stackWipeCompleteRows(WINDOW*);
    void stackRowsShift(WINDOW*);


private:
    void setCurrTetrominoData();
    bool inBounds(int, int);
    void gridMoveCursor(WINDOW*, int, int);
    void blindShiftTetromino(const int*, int, direction);
    bool canRotate(direction);
    bool canShiftUp(int);
    bool canShiftDown(int);
    bool canShiftLeft(int);
    bool canShiftRight(int);
};

#endif