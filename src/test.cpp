
#include <iostream>
#include "tetris.h"

int main(){

    initscr();
    WINDOW* win;
    win = newwin(48,100,0,0);
    refresh();
    box(win, 0, 0);
    wrefresh(win);

    tetris_init();

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
                // tetris.refreshTetromino(win);
                tetris.printGrid(win);
                tetris.printInfo(win);
                break;

            case 'd':
                tetris.rotateTetromino(CWISE);
                // tetris.refreshTetromino(win);
                tetris.printGrid(win);
                tetris.printInfo(win);
                break;

            case 'g':
                tetris.generateNextTetromino();
                tetris.setCurrTetrominoOnGrid();
                // tetris.refreshTetromino(win);
                tetris.printGrid(win);
                tetris.printInfo(win);
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
                tetris.printInfo(win);
                break;

            case 'p':
                tetris.printGrid(win);
                break;

            case KEY_UP:    
                tetris.shiftTetromino(1, UP);
                // tetris.refreshTetromino(win);
                tetris.printGrid(win);
                tetris.printInfo(win);
                break;

            case KEY_DOWN:
                tetris.shiftTetromino(1, DOWN);
                // tetris.refreshTetromino(win);
                tetris.printGrid(win);
                tetris.printInfo(win);
                break;

            case KEY_LEFT:
                tetris.shiftTetromino(1, LEFT);
                // tetris.refreshTetromino(win);
                tetris.printGrid(win);
                tetris.printInfo(win);
                break;

            case KEY_RIGHT:
                tetris.shiftTetromino(1, RIGHT);
                // tetris.refreshTetromino(win);
                tetris.printGrid(win);
                tetris.printInfo(win);
                break;
            
        //open menu
        //play/pause game
        }
    }
    
    endwin();
}