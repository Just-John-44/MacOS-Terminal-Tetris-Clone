// John Wesley Thompson
// Created: 8/10/2024
// Last edited: 9/27/2024
// main.cpp

#ifdef __APPLE__
    #define MA_NO_RUNTIME_LINKING
#endif

#define MINIAUDIO_IMPLEMENTATION

#include <iostream>
#include <ncurses.h>
#include <chrono>
#include <thread>
#include "tetris_grid.h"
#include "tetris_tetromino.h"
#include "sound_manager.h"
#include "tetris_io.h"

#define START_UPDATE_SPEED_MS 500

typedef std::chrono::high_resolution_clock::time_point time_point;
typedef std::chrono::milliseconds duration_ms;

struct game_data {
    int curr_round_cleared_rows;
    unsigned int score;
    int round;
    time_point last_refresh;
    duration_ms shift_wait_ms;
    bool user_input;
    bool user_input_shifted_down;
    bool game_over;
};

unsigned int calcScore(int, int);
bool updateRound(int &, int &);
void increaseGameSpeed(game_data &);
void playTetris(tetris_grid &, game_data &, window_data &, sound_manager &);
void initGameData(game_data &);
void getUserInput(tetris_grid &, game_data &, sound_manager &);
void updateGameAndRefresh(tetris_grid &, game_data &, window_data &, sound_manager &);

// TO DO: put the sound class into its own file.
// TO DO: make sound manager less dynamic. It can simply be designed to hold
//        the necessary number of sound effects and have enums for each. I think
//        I tried to make things way too robust for a program like this.
// TO DO: make the game end like it's supposed to.
// TO DO: create a more reliable way to initialize the terminal in. 
//        terminal_init so its changes will affect all translation units.
//        I dont want initscr to be called multiple times, for example.
// TO DO: make the terminal print in only black and white if it doesn't support colors.
// TO DO: fix game volume

// FOR DEBUGGING
void printInfo(tetris_grid & grid, game_data &gd){

    int y;
    
    y = 40;
    move(y, 0);
    clrtoeol();
    printw("total rows cleared: %i", gd.curr_round_cleared_rows);
    move(y += 2, 0);
    clrtoeol();
    printw("game speed ms: %i", gd.shift_wait_ms);
}

int main(){

    initTerminal();

    tetris_grid grid;
    window_data win_data;
    game_data game_data;
    sound_manager sound_manager;

    initWindowData(win_data, grid);
    initGameData(game_data);

    printTerminalSizeMessage(grid);

    playTetris(grid, game_data, win_data, sound_manager);

    printGameOver(win_data);

    nodelay(stdscr, FALSE);
    getch();
    endwin();

    return 0;
}

// getUserInput simply handles all immediate input from the user. This includes
// tetromino shifts, and game quit conditions.
// Input: a tetris_grid for manipulating the game, and a game_data for writing
//        game input data to
void getUserInput(tetris_grid &grid, game_data &gd, sound_manager &sm){

    int in; 
    in = getch();

    switch(in){

        case 'q':
            gd.game_over = true;
            break;

        case 'a':
            grid.rotateTetromino(CCWISE);
            break;

        case 's':
            grid.dropTetromino();
            gd.user_input_shifted_down = true;
            sm.playSound(HARD_DROP);
            break;

        case KEY_UP:
        case 'd':
            grid.rotateTetromino(CWISE);
            break;

        case KEY_DOWN:
            gd.user_input_shifted_down = true;
            break;

        case KEY_LEFT:
            grid.shiftTetromino(0, -1);
            break;

        case KEY_RIGHT:
            grid.shiftTetromino(0, 1);
            break;
    }

    gd.user_input = true;
}

// updateGameAndRefresh handles all of the game update conditions and window
// refreshes. This is the function that automatically shifts the tetromino
// downwards at increasing speeds and updates all of the game windows.
// Input: a tetris_grid for manipulating a game, a game_data for reading and 
//        writing game data to, and a window_data for refreshing the windows
void updateGameAndRefresh(tetris_grid &grid, game_data &gd, window_data &wd, sound_manager &sm){

    bool shifted;
    time_point now = std::chrono::high_resolution_clock::now();

    if (gd.last_refresh == std::chrono::high_resolution_clock::time_point::min()){
        gd.last_refresh = now;
    }

    if (now - gd.last_refresh >= gd.shift_wait_ms || 
        gd.user_input_shifted_down){

        shifted = grid.shiftTetromino(1, 0);

        if (!shifted){

            if (!gd.user_input_shifted_down){
                sm.playSound(SOFT_DROP);
            }

            // Clear all complete rows.
            int rows_cleared = grid.clearCompleteRows();

            // Set the next tetromino up.
            grid.generateNextTetromino();
            
            gd.game_over = !grid.setCurrTetrominoOnGrid();

            printNextTetromino(wd, grid.next_tet);
            
            // Update the score if it needs to be updated.
            if (rows_cleared > 0){
                gd.curr_round_cleared_rows += rows_cleared;
                gd.score += calcScore(rows_cleared, gd.round);
                printScore(wd, gd.score);

                if (rows_cleared == 4){
                    sm.playSound(TETRIS);
                }
            }  
            
            // Update the round if it needs to be updated.
            if (updateRound(gd.round, gd.curr_round_cleared_rows)){
                printRound(wd, gd.round);
                increaseGameSpeed(gd);
                sm.playSound(LEVEL_UP);
            }
            
        }

        gd.last_refresh = now;
        gd.user_input_shifted_down = false;
    }

    // Refresh the screen if any change has been made to the grid.
    if (gd.user_input || shifted){
        grid.printGrid(wd.grid_win);
        printInfo(grid, gd);
        refresh();     
        gd.user_input = false; 
    }
}

// playTetris contains the entire game loop for the tetris game.
// Input: the window_data struct for printing, and the tetris grid for playing
//        the game
void playTetris(tetris_grid &grid, game_data &gd, window_data &wd, sound_manager &sm){

    grid.setCurrTetrominoOnGrid();
    printTetrisFrame(grid);
    grid.printGrid(wd.grid_win);
    printScore(wd, gd.score);
    printNextTetromino(wd, grid.next_tet);
    printRound(wd, gd.round);
    
    sm.on();
    sm.playSound(THEME);
    
    while (!gd.game_over){
        getUserInput(grid, gd, sm);
        updateGameAndRefresh(grid, gd, wd, sm);
        std::this_thread::sleep_for(duration_ms(25));
    }
}

// initGameData initializes the game_data structure to have beginning game 
// conditions. The shift interval is set to every 500 milliseconds.
// Input: a game_data struct
void initGameData(game_data &gd){
    gd.score = 0;
    gd.round = 0;
    gd.curr_round_cleared_rows = 0;
    gd.last_refresh = std::chrono::high_resolution_clock::time_point::min();
    gd.shift_wait_ms = duration_ms(500);
    gd.user_input = false;
    gd.user_input_shifted_down = false;
    gd.game_over = false;
}

// calcScore returns a number that is equal to the points the player earned
// after a row is cleared.
// Input: the number of lines cleared and the round they were cleared on
// Output: an unsigned int with the value of the points calculated
unsigned int calcScore(int lines_cleared, int round){

    if (lines_cleared == 0){
        return 0;
    } else if (lines_cleared == 1){
        return 40 * (round + 1);
    } else if (lines_cleared == 2){
        return 100 * (round + 1);
    } else if (lines_cleared == 3){
        return 300 * (round + 1);
    } else { // lines_cleared == 4
        return 1200 * (round + 1);
    }
}

// updateRound increases the round of the game based on the total number of
// lines cleared in the game so far. It updates the round every 10 rows
// cleared.
// Input: the current_round, and the total number of lines cleared in the game
// Output: a modified round variable and a true value if the round was
//         increased
bool updateRound(int &round, int &total_lines_cleared){

    if (total_lines_cleared >= 10)
    {
        total_lines_cleared -= 10;
        round++;
        return true;
    }

    return false;
}

// increaseGameSpeed does just what it says. It decreases the time between each
// down shift event by the game. It does this based on the round that the game
// is on. 
// Input: a game_data structure for finding the shift wait time and the round
void increaseGameSpeed(game_data &gd){

    gd.shift_wait_ms = gd.shift_wait_ms - duration_ms(gd.round * 5);
}
