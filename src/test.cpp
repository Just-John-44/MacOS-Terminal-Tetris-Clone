// John Wesley Thompson
// Created: 8/10/2024
// Completed:
// Last edited: 12/30/2024
// test.cpp


#include <iostream>
#include <ncurses.h>
#include <thread>
#include <chrono>
#include "tetris_grid.h"
#include "tetris_tetromino.h"


// This struct is used to keep track of all of the window positions and sizes.
struct window_data {
    WINDOW* grid_win;
    WINDOW* score_win;
    WINDOW* tetromino_win;

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
};

void printTetrisFrame(tetris_grid &);
void initTerminal();
void initWindowData(window_data &, tetris_grid &);
void printTerminalSizeMessage(tetris_grid &);
void printScore(window_data &, int);
void printNextTetromino(window_data &, tetromino*);
void playTetris(window_data &, tetris_grid &);

// TO DO: create a more reliable way to initialize the terminal in 
//        terminal_init so it's changes will affect all translation units.
//        I dont want initscr to be called multiple times, for example.
// TO DO: add the music functions
// TO DO: add the menu functions
// TO DO: make the window sizes dynamic

// DONE: moved the terminal size message into its own function
// DONE: made a struct for managing all of the window data
// DONE: moved the tetris_grid window out of the tetris_grid struct
// DONE: renamed some of the tetris_grid variables
// DONE: created a window for the game score and the next tetromino
// DONE: made a simple system for the game score and completed the next tetromino printing function.
// DONE: modified the row wipe function to return the number of rows wiped
// DONE: improved the code size in the playTetris function.
// DONE: fixed incorrect starting rotation for newly generated tetrominoes.

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
//

int main(){

    initTerminal();

    tetris_grid grid(30, 20);
    window_data win_data;
    initWindowData(win_data, grid);

    printTerminalSizeMessage(grid);

    playTetris(win_data, grid);

    endwin();
}

// playTetris contains the entire game loop for the tetris game.
// Input: the window_data struct for printing, and the tetris grid for playing
//        the game
void playTetris(window_data &wd, tetris_grid &grid){

    int score = 0;

    grid.setCurrTetrominoOnGrid();
    printTetrisFrame(grid);
    grid.printGrid(wd.grid_win);
    printScore(wd, score);
    printNextTetromino(wd, grid.next_tet);

    int in;
    while(1){
        in = getch();
        switch(in){
            
            case 'q':
                endwin();
                exit(0);
            
            case 'a':
                grid.rotateTetromino(CCWISE);
                break;

            case 'd':
                grid.rotateTetromino(CWISE);
                break;

            case 'g':
                grid.generateNextTetromino();
                if (!grid.setCurrTetrominoOnGrid()){
                    endwin();
                    exit(0);
                }
                printNextTetromino(wd, grid.next_tet);
                break;

            case 'w':
                score += 100 * grid.stackWipeCompleteRows();
                grid.stackRowsShift();
                printScore(wd, score);
                break;

            case 's':
                grid.dropTetromino();

            case 'p':
                break;

            case KEY_UP:    
                grid.shiftTetromino(-1, 0);
                break;

            case KEY_DOWN:
                grid.shiftTetromino(1, 0);
                break;

            case KEY_LEFT:
                grid.shiftTetromino(0, -1);
                break;

            case KEY_RIGHT:
                grid.shiftTetromino(0, 1);
                break;
            
        //open menu
        //play/pause game
        }

        grid.printGrid(wd.grid_win);
        printInfo(grid);
        refresh();
    }

}

// printTetrisFrame prints the tetris title, and a border for the tetris grid,
// the score window, and the next tetromino window.
// Input: a tetris_grid struct to the grid size information from
void printTetrisFrame(tetris_grid &grid){

    // the rows of the tetris frame that come after the title
    std::string title_base = "[X]";
    for (int i = 0; i < grid.length; i++){
        title_base += "[ ]";
    }
    title_base += "[X]";

    std::string grid_row = "| |";
    for (int i = 0; i < grid.length; i++){
        grid_row += "   ";
    }
    grid_row += "| |"; 
    
    std::string base_row = "[X]";
    for (int i = 0; i < grid.length; i++){
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
    
    for (int i = 0; i < grid.height; i++){
        printw(grid_row.c_str());

        if (i == 0 || i == grid.height - 1){
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

//
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
}

// printTerminalSizeMessage prompts the user at the beginning of the program to
// expand the terminal window if it is too small to play the game.
// Input: a tetris_grid struct to the grid size information from
void printTerminalSizeMessage(tetris_grid &grid){

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    int min_term_height = grid.height + 30;
    int min_term_length = grid.length * 3 + 30;

    while (max_y < min_term_height ||
           max_x < min_term_length)
    {
        clear();
        move(max_y / 2, max_x / 2 - 26);
        printw("Expand the terminal to at least %i high and %i wide.",
               min_term_height, min_term_length);

        move(max_y / 2 + 1, max_x / 2 - 17);
        printw("Current size: %i high and %i wide.",
               max_y, max_x);

        refresh();
        
        getmaxyx(stdscr, max_y, max_x);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

// printScore prints the tetris game score to the right side of the grid.
// Input: the window data structure and a score to print
void printScore(window_data &wd, int score){

    wmove(wd.score_win, 0, 0);
    wclear(wd.score_win);
    wprintw(wd.score_win, "  SCORE\n -------\n  %i", score);
    wrefresh(wd.score_win);
}

// printNextTetromino prints the next tetromino on the right side of the grid.
// Input: the window data structure and a tetromino pointer
void printNextTetromino(window_data &wd, tetromino* tet){

    wmove(wd.tetromino_win, 0, 0);
    wclear(wd.tetromino_win);

    wprintw(wd.tetromino_win, " NEXT_UP\n -------\n");

    wattron(wd.tetromino_win, COLOR_PAIR(tet->color));
    for (int i = 0; i < tet->sstride; i++){

        if (tet->type == I_TETROMINO){
            wmove(wd.tetromino_win, i + 3, 1);
        } else {
            wmove(wd.tetromino_win, i + 3, 2);
        }

        for (int j = 0; j < tet->sstride; j++){
        
            if (tet->shapeAt(i, j) == EMPTY_SQR){
                wprintw(wd.tetromino_win, "  ");
            } else {
                wprintw(wd.tetromino_win, "X ");
            }
        }
    }
    wattroff(wd.tetromino_win, COLOR_PAIR(tet->color));
    wrefresh(wd.tetromino_win);
}

// initWindowData initializes the window data struct with all windows' location
// and sizes based on a tetris grid.
// Input: the window data structure to be initialized, and a tetris grid to 
// base all of the window locations off of.
void initWindowData(window_data &wd, tetris_grid &grid){

    wd.grid_win_start_y = 9;
    wd.grid_win_start_x = 3;
    wd.grid_win_height = grid.height;
    wd.grid_win_length = grid.length;

    wd.score_win_start_y = wd.grid_win_start_y + 1;
    wd.score_win_start_x = wd.grid_win_start_x + wd.grid_win_length * 3 + 3;
    wd.score_win_height = 5;
    wd.score_win_length = 9;

    wd.tetromino_win_start_y = wd.score_win_start_y + wd.score_win_height;
    wd.tetromino_win_start_x = wd.score_win_start_x;
    wd.tetromino_win_height = wd.grid_win_height - wd.score_win_height - 2;
    wd.tetromino_win_length = wd.score_win_length;

    wd.grid_win = newwin(wd.grid_win_height, wd.grid_win_length * 3, 
                         wd.grid_win_start_y, wd.grid_win_start_x);

    wd.score_win = newwin(wd.score_win_height, wd.score_win_length, 
                         wd.score_win_start_y, wd.score_win_start_x);

    wd.tetromino_win = newwin(wd.tetromino_win_height, wd.tetromino_win_length, 
                         wd.tetromino_win_start_y, wd.tetromino_win_start_x);
    refresh();
}

// void updateWindowData(window_data &wd){



// }