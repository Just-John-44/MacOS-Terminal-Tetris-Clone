// John Wesley Thompson
// Created: 8/10/2024
// Completed:
// Last edited: 12/28/2024
// test.cpp


#include <iostream>
#include <thread>
#include <chrono>
#include "tetris_grid.h"

void printTetrisFrame(tetris_grid &);
void terminalInit();
void playTetris(tetris_grid &grid);

// TO DO: create a more reliable way to initialize the terminal in 
//        terminal_init so it's changes will affect all translation units.
//        I dont want initscr to be called multiple times, for example.
// TO DO: add the music functions
// TO DO: add the menu functions
// TO DO: create a separate window for the score

// DONE: added terminal resizing message.
// DONE: introduced timing.
// DONE: moved the square struct to the tetris_types files
// DONE: changed makefile to work with the differences

// FOR DEBUGGING
void printInfo(tetris_grid & grid){

    int y;
    
    y = 40;
    move(y, 0);
    clrtoeol();
    printw("tet_y: %i", grid.tet_y_pos);
    move(y += 2, 0);
    clrtoeol();
    printw("tet_x: %i", grid.tet_x_pos);
    move(y += 2, 0);
    clrtoeol();
    printw("tet_type: %i", grid.curr_tet->type);
    move(y += 2, 0);
    clrtoeol();
    printw("tet_sstride: %i", grid.curr_tet->sstride);
    move(y += 2, 0);
    clrtoeol();
    printw("tet_salength: %i", grid.curr_tet->salength);
}

const int MIN_TERM_LENGTH = 80;
const int MIN_TERM_HEIGHT = 50;
const int GRID_START_Y = 9;
const int GRID_START_X = 3;
const int SCORE_START_Y;
const int SCORE_START_X;

int main(){

    initTerminal();

    tetris_grid grid(GRID_START_Y, GRID_START_X);

    playTetris(grid);

    endwin();
}

void playTetris(tetris_grid & grid){

    grid.setCurrTetrominoOnGrid();
    printTetrisFrame(grid);
    grid.printGrid();

    int in;
    while(1){
        in = getch();
        switch(in){
            
            case 'q':
                endwin();
                exit(0);
            
            case 'a':
                grid.rotateTetromino(CCWISE);
                grid.printGrid();
                break;

            case 'd':
                grid.rotateTetromino(CWISE);
                grid.printGrid();
                break;

            case 'g':
                grid.generateNextTetromino();
                if (!grid.setCurrTetrominoOnGrid()){
                    endwin();
                    exit(0);
                }
                grid.printGrid();
                break;

            case 'w':
                grid.stackWipeCompleteRows();
                grid.stackRowsShift();
                grid.printGrid();
                break;

            case 's':
                grid.dropTetromino();
                grid.printGrid();

            case 'p':
                grid.printGrid();
                break;

            case KEY_UP:    
                grid.shiftTetromino(-1, 0);
                grid.printGrid();
                break;

            case KEY_DOWN:
                grid.shiftTetromino(1, 0);
                grid.printGrid();
                break;

            case KEY_LEFT:
                grid.shiftTetromino(0, -1);
                grid.printGrid();
                break;

            case KEY_RIGHT:
                grid.shiftTetromino(0, 1);
                grid.printGrid();
                break;
            
        //open menu
        //play/pause game
        }

        printInfo(grid);
        refresh();
    }

}

void printTetrisFrame(tetris_grid &grid){

    // the rows of the tetris frame that come after the title
    std::string title_base = "[X]";
    for (int i = 0; i < grid.GRID_LENGTH; i++){
        title_base += "[ ]";
    }
    title_base += "[X]";

    std::string grid_row = "| |";
    for (int i = 0; i < grid.GRID_LENGTH; i++){
        grid_row += "   ";
    }
    grid_row += "| |"; 
    
    std::string base_row = "[X]";
    for (int i = 0; i < grid.GRID_LENGTH; i++){
        base_row += "[X]";
    }
    base_row += "[X]"; 

    // buffers that go after the grid rows following the tetris title
    std::string post_title_base_buffer = "[ ][ ][ ][X]\n";
    std::string post_grid_row_buffer1 = "---------| |\n";
    std::string post_grid_row_buffer2 = "         | |\n";
    std::string post_base_row_buffer = "[X][X][X][X]\n";


    // buffers that go after each line of the tetris title
    int tetris_title_length = 41;
    int tetris_title_buffer_length = grid_row.length() + 
                                     post_grid_row_buffer1.length() - 
                                     tetris_title_length;

    std::string post_title_buffer_r1 = "";
    for (int i = 0; i < tetris_title_buffer_length - 2; i++){

        post_title_buffer_r1 += "_";
    }
    post_title_buffer_r1 += ",\n";

    std::string post_title_buffer_middle = "";
    for (int i = 0; i < tetris_title_buffer_length - 2; i++){
        
        post_title_buffer_middle += " ";
    }
    post_title_buffer_middle += "|\n";

    std::string post_title_buffer_r8 = "";
    for (int i = 0; i < tetris_title_buffer_length - 2; i++){
        
        post_title_buffer_r8 += "_";
    }
    post_title_buffer_r8 += "]\n";
    
    // the tetris title
    move(0, 0);
    printw(
        ",________________________________________%s"
        "|  _____|_____|_____|_____|_____|_____  |%s"
        "| [_, ,_] ,___]_, ,_]  _  ]_, ,_] ,___] |%s"
        "|   | | | [__ | | | | [_] | | | | [___  |%s"
        "|   | | | ,__]| | | | ,  _] | | [___, ] |%s"
        "|   | | | [___| | | | |\\ \\|_] [_|___] | |%s"
        "|   [_] [_____] [_] [_] \\_\\_____]_____] |%s"
        "[_______|_____|_____|_____|_____|_______|%s",
        post_title_buffer_r1.c_str(),
        post_title_buffer_middle.c_str(),
        post_title_buffer_middle.c_str(),
        post_title_buffer_middle.c_str(),
        post_title_buffer_middle.c_str(),
        post_title_buffer_middle.c_str(),
        post_title_buffer_middle.c_str(),
        post_title_buffer_r8.c_str()
    );

    printw(title_base.c_str());
    printw(post_title_base_buffer.c_str());
    
    for (int i = 0; i < grid.GRID_HEIGHT; i++){
        printw(grid_row.c_str());

        if (i == 0 || i == grid.GRID_HEIGHT - 1){
            printw(post_grid_row_buffer1.c_str());
        } else {
            printw(post_grid_row_buffer2.c_str());
        }
        
    }

    for (int i = 0; i < 3; i++){
        printw(base_row.c_str());
        printw(post_base_row_buffer.c_str());
    }

    refresh();
}

void initTerminal(){

    initscr();
    curs_set(0);
    noecho();
    keypad(stdscr, true);

    if (has_colors()){
        start_color();

        if (can_change_color()){
            
            init_color(COLOR_PURPLE, 626, 125, 940);
            init_color(COLOR_ORANGE, 999, 646, 0); 
            init_color(COLOR_L_BLUE, 000, 000, 999); 
            init_color(COLOR_L_YELLOW, 777, 777, 000);    
            
            init_pair(PURPLE, COLOR_PURPLE, COLOR_BLACK);
            init_pair(ORANGE, COLOR_ORANGE, COLOR_BLACK);
            init_pair(L_BLUE, COLOR_L_BLUE, COLOR_BLACK);
            init_pair(L_YELLOW, COLOR_L_YELLOW, COLOR_BLACK);
        } 

        init_pair(STANDARD, COLOR_WHITE, COLOR_BLACK);
        init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
        init_pair(CYAN, COLOR_CYAN, COLOR_BLACK);
        init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
        init_pair(RED, COLOR_RED, COLOR_BLACK);
        init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);

        bkgd(COLOR_PAIR(STANDARD));
    }

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    while (max_y < MIN_TERM_HEIGHT ||
           max_x < MIN_TERM_LENGTH)
    {
        clear();
        move(max_y / 2, max_x / 2 - 26);
        printw("Expand the terminal to at least %i high and %i wide.",
               MIN_TERM_HEIGHT, MIN_TERM_LENGTH);

        move(max_y / 2 + 1, max_x / 2 - 17);
        printw("Current size: %i high and %i wide.",
               max_y, max_x);

        refresh();
        
        getmaxyx(stdscr, max_y, max_x);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}