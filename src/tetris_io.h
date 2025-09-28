// John Wesley Thompson
// Created: 9/27/2025
// Last Edited: 9/27/2025
// tetris_io.h

#ifndef TETRIS_IO_H
#define TETRIS_IO_H

#include <ncurses.h>
#include "tetris_grid.h"

// This struct is used to keep track of all of the window positions and sizes.
struct window_data {
    WINDOW* grid_win;
    WINDOW* score_win;
    WINDOW* tetromino_win;
    WINDOW* round_win;

    int grid_win_start_y;
    int grid_win_start_x;
    int grid_win_height;
    int grid_win_length;

    int score_win_start_y;
    int score_win_start_x;
    int score_win_height;
    int score_win_length;

    int tetromino_win_start_y;
    int tetromino_win_start_x;
    int tetromino_win_height;
    int tetromino_win_length;

    int round_win_start_y;
    int round_win_start_x;
    int round_win_height;
    int round_win_length;
};

void initTerminal();
void initWindowData(window_data &, tetris_grid &);
void printTerminalSizeMessage(tetris_grid &);
void printScore(window_data &, int);
void printTetrisFrame(tetris_grid&);
void printNextTetromino(window_data &, tetromino*);
void printRound(window_data &, int);
void printGameOver(window_data &wd);




#endif // TETRIS_IO_H