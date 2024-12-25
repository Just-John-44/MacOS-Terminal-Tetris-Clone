// John Wesley Thompson
// Created: 
// Completed:
// Last edited: 12/24/2024
// test.cpp


#include <iostream>
#include "tetris_grid.h"


// TO DO: create a more reliable way to initialize the terminal in 
//        terminal_init so it's changes will affect all file translations.
// TO DO: add the music functions
// TO DO: add the menu functions
// 

void terminal_init(){
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

int main(){

    initscr();
    WINDOW* win;
    win = newwin(48, 100, 0, 0);
    refresh();
    box(win, 0, 0);
    wrefresh(win);

    terminal_init();

    tetris_grid tetris;
    refresh();
    tetris.printInfo(win);
    getch();

    tetris.setCurrTetrominoOnGrid();

    tetris.printGrid(win);
    getch();

    int in;
    keypad(win, true);
    while(1){
        in = wgetch(win);
        switch(in){
            //quit game
            case 'q':
                endwin();
                exit(0);
            //movement
            case 'a':
                tetris.rotateTetromino(CCWISE);
                tetris.refreshTetromino(win);
                break;

            case 'd':
                tetris.rotateTetromino(CWISE);
                tetris.refreshTetromino(win);
                break;

            case 'g':
                tetris.generateNextTetromino();
                if (!tetris.setCurrTetrominoOnGrid()){
                    endwin();
                    exit(0);
                }
                // tetris.refreshTetromino(win);
                tetris.printGrid(win);
                break;

            // case 's':
            //     tetris.settleCurrTetromino();
            //     // tetris.refreshTetromino(win);
            //     tetris.printGrid(win);
            //     tetris.printInfo(win);
            //     break;

            case 'w':
                tetris.stackWipeCompleteRows(win);
                tetris.stackRowsShift(win);
                tetris.printGrid(win);
                break;

            case 'p':
                tetris.printGrid(win);
                break;

            case KEY_UP:    
                tetris.shiftTetromino(-1, 0);
                tetris.refreshTetromino(win);
                break;

            case KEY_DOWN:
                tetris.shiftTetromino(1, 0);
                tetris.refreshTetromino(win);
                break;

            case KEY_LEFT:
                tetris.shiftTetromino(0, -1);
                tetris.refreshTetromino(win);
                break;

            case KEY_RIGHT:
                tetris.shiftTetromino(0, 1);
                tetris.refreshTetromino(win);
                break;
            
        //open menu
        //play/pause game
        }

        tetris.printInfo(win);
    }
    
    endwin();
}