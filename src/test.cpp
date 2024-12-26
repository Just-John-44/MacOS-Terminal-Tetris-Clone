// John Wesley Thompson
// Created: 8/10/2024
// Completed:
// Last edited: 12/26/2024
// test.cpp


#include <iostream>
#include "tetris_grid.h"

void printTetrisFrame(tetris_grid &);

// TO DO: create a more reliable way to initialize the terminal in 
//        terminal_init so it's changes will affect all file translations.
// TO DO: add the music functions
// TO DO: add the menu functions

// TO DO: make the tetris frame dynamic so that it prints correctly when
//        the tetris grid is expanded.

// TO DO: wrap some of the functions at the beginning fo the program to
//        make what is happening more understandable.
// TO DO: remove the tetris_stack files because there is a bunch of unused code
// TO DO: integrate tetris' window into its own class

void terminal_init(){

    curs_set(0);
    noecho();

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

    refresh();
}

int main(){

    initscr();
    WINDOW* win;
    tetris_grid tetris;
    win = newwin(43, 100, 8, 2);
    refresh();
    wrefresh(win);

    terminal_init();

    refresh();
    printInfo(tetris);
    getch();

    tetris.setCurrTetrominoOnGrid();
    
    printTetrisFrame(tetris);

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
                tetris.refreshTetromino(win);
                break;

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

        printInfo(tetris);
    }
    
    endwin();
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
}