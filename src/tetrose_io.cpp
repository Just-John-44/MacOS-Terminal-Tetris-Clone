// John Wesley Thompson
// Created: 9/27/2025
// Last Edited: 9/27/2025
// tetrose_io.cpp


#include "tetrose_io.h"
#include <thread>


// initTerminal initializes all of the ncurses functions including colors for
// the terminal. 
void initTerminal(){

    initscr();
    curs_set(0);
    noecho();
    nodelay(stdscr, true);
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

// initWindowData initializes the window data struct with all windows' location
// and sizes based on a tetrose grid.
// Input: the window data structure to be initialized, and a tetrose grid to 
// base all of the window locations off of.
void initWindowData(window_data &wd, tetrose_grid &grid){

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
    wd.tetromino_win_height = 7;
    wd.tetromino_win_length = 9;

    wd.round_win_start_y = wd.tetromino_win_start_y + wd.tetromino_win_height;
    wd.round_win_start_x = wd.score_win_start_x;
    wd.round_win_height = 5;
    wd.round_win_length = 9;

    wd.grid_win = newwin(wd.grid_win_height, wd.grid_win_length * 3, 
                         wd.grid_win_start_y, wd.grid_win_start_x);

    wd.score_win = newwin(wd.score_win_height, wd.score_win_length, 
                         wd.score_win_start_y, wd.score_win_start_x);

    wd.tetromino_win = newwin(wd.tetromino_win_height, wd.tetromino_win_length, 
                         wd.tetromino_win_start_y, wd.tetromino_win_start_x);

    wd.round_win = newwin(wd.round_win_height, wd.round_win_length, 
                         wd.round_win_start_y, wd.round_win_start_x);
                  
    refresh();
}

// printTerminalSizeMessage prompts the user at the beginning of the program to
// expand the terminal window if it is too small to play the game.
// Input: a tetrose_grid struct to the grid size information from
void printTerminalSizeMessage(tetrose_grid &grid){

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    int min_term_height = grid.height + 15;
    int min_term_length = grid.length * 3 + 25;

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

// printScore prints the tetrose game score to the right side of the grid.
// Input: the window data structure and a score to print
void printScore(window_data &wd, int score){

    wmove(wd.score_win, 0, 0);
    wclear(wd.score_win);
    wprintw(wd.score_win, "  SCORE\n -------\n %07i", score);
    wrefresh(wd.score_win);
}

// printtetroseFrame prints the tetrose title, and a border for the tetrose grid,
// the score window, and the next tetromino window.
// Input: a tetrose_grid struct to the grid size information from
void printtetroseFrame(tetrose_grid &grid){

    // the rows of the tetrose frame that come after the title
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

    // buffers that go after the grid rows following the tetrose title
    std::string post_title_base_buffer = "[ ][ ][ ][X]\n";
    std::string post_grid_row_buffer1 = "---------| |\n";
    std::string post_grid_row_buffer2 = "         | |\n";
    std::string post_base_row_buffer = "[X][X][X][X]\n";
    
    // the tetrose title
    move(0, 0);
    printw(
        ",______________________________________________,\n"
        "|  _____|_____|_____|_____|_____|_____|_____   |\n"
        "| [_, ,_] ,___]_, ,_]  _  ] ,_, ] ,___] ,___]  |\n"
        "|   | | | [__ | | | | [_] | | | | [___| [__    |\n"
        "|   | | | ,__]| | | | ,  _] | | [___, ] ,__]   |\n"
        "|   | | | [___| | | | |\\ \\| [_] |___] | [___   |\n"
        "|   [_] [_____] [_] [_] \\_\\_____]_____]_____]  |\n"
        "[_______|_____|_____|_____|_____|_____|________]\n"
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
        } else if (tet->type == O_TETROMINO){
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

// printRound simply prints the round in a window specified in the window_data
// struct. 
// Input: a window_data struct and the round as an integer
void printRound(window_data &wd, int round){
    
    wmove(wd.round_win, 0, 0);
    wclear(wd.round_win);

    wprintw(wd.round_win, "  ROUND\n -------\n   %03i", round);
    wrefresh(wd.round_win);

}

// fancy end of game message
void printGameOver(window_data &wd){
    clear();
    printw(
        ",_____________________________,\n"
        "|  _____|_____|__   __|_____  |\n"
        "| [ ,___]  _  ]  \\ /  ] ,___] |\n"
        "| | | __| [_] | , v , | [__   |\n"
        "| | |[, ] ,_, | |\\_/| | ,__]  |\n"
        "| | [_] | | | | |   | | [___  |\n"
        "| [_____]_] [_]_]   [_]_____] |\n"
        "|_____________________________|\n"
        "|  _____|_     _|_____|_____  |\n"
        "| [ ,_, ] |   | | ,___]  _  ] |\n"
        "| | [ ] | |   | | [__ | [_] | |\n"
        "| | [ ] |\\ \\ / /| ,__]| ,  _] |\n"
        "| | [_] | \\ v / | [___| |\\ \\  |\n"
        "| [_____]  \\_/  |_____]_] \\_\\ |\n"
        "[_______|_______|_____|_______]\n"
        "\n"
        " press any key to end the game "
    );

    refresh();
}


