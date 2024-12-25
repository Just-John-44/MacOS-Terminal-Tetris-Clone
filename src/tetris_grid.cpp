// John Wesley Thompson
// Created: 8/10/2024
// Completed:
// Last Edited: 12/24/2024
// tetris_grid.cpp

// This file includes all of the implentation for the tetromino class and
// tetris_grid class.

#include <iostream>

#include "tetris_grid.h"
#include "tetris_stack.h"
#include "tetris_tetromino.h"


// TO DO: tetris init should create the window that it's displayed in as well.
// TO DO: import the random library for better tetromino generation
// TO DO: improve the refreshTetromino function's speed
// TO DO: create a game over function called topReached()
// TO DO: make the place tetromino on grid function return false or -1 if 
//        a block could not be placed.
// TO DO: create a tetris settings class or struct that has all of the tetris
//        setting data as well as getters and setters
// TO DO: prefix the variables in the tetris_grid class with m
// TO DO: make appropriate functions pass by reference and const
// TO DO: make the terminal print in only black and white if it doesn't support colors.
// TO DO: I should probably make the tetromino arrays square types instead of ints

// ============================================================================
// TETRIS_GRID CLASS 
// ============================================================================

int GRID_HEIGHT = 20;
int GRID_LENGTH = 10;
int GRID_SIZE = GRID_HEIGHT * GRID_LENGTH;

//
tetris_grid::tetris_grid(){

    curr_tet = &tetris_tetrominoes[rand() % 7];
    next_tet = &tetris_tetrominoes[rand() % 7];

    // set non garbage values for the position of the tetromino on the grid
    tet_y_pos = tet_x_pos = 0;

    // This resizes the grid to the default values: a height of 20 and
    // a length of 10.
    grid.resize(GRID_HEIGHT);
    for (int i = 0; i < GRID_HEIGHT; i++){
        grid.at(i).resize(GRID_LENGTH);
    }
}

//
void tetris_grid::printInfo(WINDOW* win){

    int y;
    
    y = 30;
    wmove(win, y, 0);
    wclrtoeol(win);
    wprintw(win, "tet_y: %i", tet_y_pos);
    wmove(win, y += 2, 0);
    wclrtoeol(win);
    wprintw(win, "tet_x: %i", tet_x_pos);
    wmove(win, y += 2, 0);
    wclrtoeol(win);
    wprintw(win, "tet_type: %i", curr_tet->type);
    wmove(win, y += 2, 0);
    wclrtoeol(win);
    wprintw(win, "tet_sstride: %i", curr_tet->sstride);
    wmove(win, y += 2, 0);
    wclrtoeol(win);
    wprintw(win, "tet_salength: %i", curr_tet->salength);

    wrefresh(win);
}

// TERMINAL OUTPUT ============================================================

void tetris_grid::printGrid(WINDOW* grid_win){
    int win_height, win_length;
    getmaxyx(grid_win, win_height, win_length);

    if (win_height < (GRID_HEIGHT) + 2 || 
        win_length < (GRID_LENGTH * 3) + 2)
        return;
    
    wmove(grid_win, 1, 1);
    for (int i = 0; i < GRID_HEIGHT; i++){  
        for (int j = 0; j < GRID_LENGTH; j++){    

            if (grid.at(i).at(j).s_type == EMPTY_SQR){
                wprintw(grid_win, "   ");   
                continue;
            }

            wattron(grid_win, COLOR_PAIR(grid.at(i).at(j).s_color));
            if (grid.at(i).at(j).s_type == TOP_SQR){  
                wprintw(grid_win, "[ ]");
            } else if (grid.at(i).at(j).s_type == BOTTOM_SQR){                                      
                wprintw(grid_win, "[_]");
            }
            // wprintw(grid_win, " (%2i, %i)", grid.at(i).at(j).s_type, grid.at(i).at(j).s_color);
            wattroff(grid_win, COLOR_PAIR(grid.at(i).at(j).s_color));
        }
        wmove(grid_win, i + 2, 1);
    }

    wrefresh(grid_win);
}

// TO DO: improve the speed of this function, some squares are lagging when the 
//        tetromino is moved fast. ALSO... there doesnt need to be a trail 
//        value becuase you can simply print spaces where there are zeroes. I 
//        can probably improve the way it's printed by adding all of the print 
//        coordinates to an array and printing them out by moving down the 
//        y_values instead of having a bunch of calculations between each
//        print.

// This function only reprints the current tetromino
void tetris_grid::refreshTetromino(WINDOW* grid_win){

    int refresh_idx_y = tet_y_pos - 2;
    int refresh_idx_x = tet_x_pos - 2;

    int refresh_area_height = curr_tet->sstride + 4; // +2 on all sides of shape array
    int refresh_area_width = curr_tet->sstride + 4;  //

    for (int i = 0; i < refresh_area_height; i++){
        for (int j = 0; j < refresh_area_width; j++){

            if (!inBounds(refresh_idx_y + i, refresh_idx_x + j)){
                continue;
            } 
            
            // 3 is the width of each printed square, and 1 is added to offset
            // from the border of the ncurses window
            wmove(grid_win, refresh_idx_y + i + 1, (refresh_idx_x + j) * 3 + 1);
            wattron(grid_win, COLOR_PAIR(grid.at(refresh_idx_y + i).at(refresh_idx_x + j).s_color));

            if (grid.at(refresh_idx_y + i).at(refresh_idx_x + j).s_type == TOP_SQR){

                //wattron(grid_win, COLOR_PAIR(grid.at(refresh_idx_y + i).at(refresh_idx_x + j).s_color));
                wprintw(grid_win, "[ ]");
                //wattroff(grid_win, COLOR_PAIR(grid.at(refresh_idx_y + i).at(refresh_idx_x + j).s_color));


            } else if (grid.at(refresh_idx_y + i).at(refresh_idx_x + j).s_type == BOTTOM_SQR){

                //wattron(grid_win, COLOR_PAIR(grid.at(refresh_idx_y + i).at(refresh_idx_x + j).s_color));
                wprintw(grid_win, "[_]");
                //wattroff(grid_win, COLOR_PAIR(grid.at(refresh_idx_y + i).at(refresh_idx_x + j).s_color));


            } else {
                wprintw(grid_win, "   ");
            } 

            wattroff(grid_win, COLOR_PAIR(grid.at(refresh_idx_y + i).at(refresh_idx_x + j).s_color));
            wrefresh(grid_win);
        }
    }
}

// TETRIS STACK MANIPULATION ==================================================

//
void tetris_grid::stackWipeCompleteRows(WINDOW* grid_win){ 
    bool row_full;
    for (int i = 0; i < GRID_HEIGHT; i++){

        row_full = true;
        for (int j = 0; j < GRID_LENGTH; j++){
            if (grid.at(i).at(j).s_type == EMPTY_SQR){
                row_full = false;
                break;
            }
        }

        if (!row_full)
            continue;

        // If the row is full, wipe it.
        for (int j = 0; j < GRID_LENGTH; j++){
            grid.at(i).at(j).setEmpty();
        }
    }
}

//
void tetris_grid::stackRowsShift(WINDOW* grid_win){
    bool unempty_row_arr[GRID_HEIGHT];
    int unempty_row_count = 0;

    bool row_empty;
    int lowest_empty_row_idx = 0;
    for (int i = 0; i < GRID_HEIGHT; i++){
        row_empty = true;
        for (int j = 0; j < GRID_LENGTH; j++){
            if (grid.at(i).at(j).s_type != EMPTY_SQR){
                row_empty = false;
                break;
            }
        }

        if (row_empty){
            lowest_empty_row_idx = i;
            unempty_row_arr[i] = 0;
            
        } else {
            unempty_row_arr[i] = 1;
            unempty_row_count++;
        }
    }


    bool floating = false;
    for (int i = GRID_HEIGHT - 1; i >= 0; i--){
        if (unempty_row_arr[i] == 0){
            floating = true;

        } else if (unempty_row_arr[i] == 1 && floating){
            for (int j = 0; j < GRID_LENGTH; j++){
                grid.at(lowest_empty_row_idx).at(j).s_color = grid.at(i).at(j).s_color;
                grid.at(lowest_empty_row_idx).at(j).s_type = grid.at(i).at(j).s_type;
                grid.at(i).at(j).setEmpty();
            }
            unempty_row_count--;
            lowest_empty_row_idx--; // decrement becuase lower indices are higher
                                    // on the grid. 

        } else if (unempty_row_arr[i] == 1){ // && !floating
            unempty_row_count--;
        }

        if (unempty_row_count == 0)
            break;
    }
}

// MOVEMENT AND MOVEMENT VALIDATION ===========================================

// bool tetris_grid::canShiftUp(int shift_count){
//
//     for (int col = 0; col < curr_tet->sstride; col++){
//         for (int row = 0; row < curr_tet->sstride; row++){
//                 // if there is a square and
//             if (curr_tet->shape_arr[row][curr_tet->current_rotation + col] == 0){
//                 continue;
//             } 
//                 // the shifted square is out of bounds
//             if (!inBounds(tet_y_pos + row - shift_count, tet_x_pos + col) || 
//                 // there is a square above the square
//                 grid.at(tet_y_pos + row - shift_count).at(tet_x_pos + col).s_type != EMPTY_SQR)
//             {
//                 return false;
//             } else {
//                 // skip to the next line to avoid checking if the tetromino is colliding with itself.
//                 break;
//             }
//         }
//     }
//     return true;
// }
//
// bool tetris_grid::canShiftDown(int shift_count){
//
//     for (int col = 0; col < curr_tet->sstride; col++){
//         for (int row = curr_tet->sstride - 1; row >= 0; row--){
//     
//             if (curr_tet->shape_arr[row][curr_tet->current_rotation + col] == 0){
//                 continue;
//             } 
//
//                 // the shifted square is out of bounds
//             if (!inBounds(tet_y_pos + row + shift_count, tet_x_pos + col) ||
//                 // there is a square beneath the square 
//                 grid.at(tet_y_pos + row + shift_count).at(tet_x_pos + col).s_type != EMPTY_SQR)
//             {
//                 return false;
//             } else {
//                 // skip to the next line to avoid checking if the tetromino is colliding with itself.
//                 break;
//             }
//         }
//     }
//     return true;
// }
//
// bool tetris_grid::canShiftLeft(int shift_count){
//
//     bool left_edge_checked;
//     for (int row = 0; row < curr_tet->sstride; row++){
//         left_edge_checked = false;
//
//         for (int col = 0; col < curr_tet->sstride; col++){             
//             if (curr_tet->shape_arr[row][curr_tet->current_rotation + col] == 0){
//                 continue;
//             }
//
//             if (!inBounds(tet_y_pos + row, tet_x_pos + col - shift_count)){
//                 return false;
//             }
//
//             // If the leftmost square on this row has not been checked for a future collision,
//             // check it.
//             if (!left_edge_checked &&
//                 grid.at(tet_y_pos + row).at(tet_x_pos + col - shift_count).s_type != EMPTY_SQR)
//             {
//                 return false;
//
//             } else {
//                 // the leftmost edge should be checked if the previous if statement executed.
//                 left_edge_checked = true;
//             }  
//         }
//     }
//     return true;
// }
//
// bool tetris_grid::canShiftRight(int shift_count){
//
//     bool right_edge_checked;
//     for (int row = 0; row < curr_tet->sstride; row++){
//         right_edge_checked = false;
//
//         for (int col = curr_tet->sstride - 1; col >= 0; col--){
//
//             if (curr_tet->shape_arr[row][curr_tet->current_rotation + col] == 0){
//                 continue;
//             }
//
//             // If the rightmost square on this row has been checked for a future collision, 
//             // skip to the next row.
//             if (right_edge_checked){
//                 break;
//             }
//
//             if (!inBounds(tet_y_pos + row, tet_x_pos + col + shift_count) || 
//                 grid.at(tet_y_pos + row).at(tet_x_pos + col + shift_count).s_type != EMPTY_SQR)
//             {    
//                 return false;
//             }
//
//             // the rightmost edge should be checked if the previous if statement executed.
//             right_edge_checked = true;
//         }
//     }
//
//     return true;
// }

// shiftTetromino simply moves the current tetromino's data in a specific
// on the grid. If the shift is unsuccessful, the tetromino's data is replaced 
// where it before the function call. This function only changes the data and
// does not reprint the tetromino.
// Input: how far it should shift on the grid, and the direction it should
//        shift
// Output: true if the shift was succesful, and false if the shift was not
bool tetris_grid::shiftTetromino(int y_shift_ct, int x_shift_ct){


    // move grid_pos in the appropriate direction
    int prev_tet_y_pos = tet_y_pos;
    int prev_tet_x_pos = tet_x_pos;

    // replace all squares in tetromino with empty values.
    removeTetromino();
    
    // Find next tetromino position.
    tet_y_pos += y_shift_ct;
    tet_x_pos += x_shift_ct;

    // If there is a collision, undo the previous action and return false
    // indicating that the shift was unsuccessful.
    if (colliding()){
        tet_y_pos = prev_tet_y_pos;
        tet_x_pos = prev_tet_x_pos;

        placeTetromino();
        
        return false;
    }

    // Reassign tetromino's values.
    placeTetromino();

    return true;
}

// peekShiftTetromino shifts the location values of the tetromino on the grid.
// It does not change any of the grid values in any way, and it returns true if
// the shift is successful.
// Input: an x and y modifier for the tetromino's grid location
bool tetris_grid::peekShiftTetromino(int y_shift_ct, int x_shift_ct){

    int prev_tet_y_pos = tet_y_pos;
    int prev_tet_x_pos = tet_x_pos;

    // move grid_pos in the appropriate direction
    tet_y_pos += y_shift_ct;
    tet_x_pos += x_shift_ct;

    if (colliding()){
        tet_y_pos = prev_tet_y_pos;
        tet_x_pos = prev_tet_x_pos;
        return false;
    }

    return true;
}

// rotateTetromino rotates tetrominoes according to SRS 
// (Super Rotation System) it wraps the tetromino::rotate function and adds
// wall kicking conditions so that the tetromino can rotate if there is space
// for it to shift to first. This function changes the grid data and does not
// reprint the tetromino.
// Input: a direction for rotation
bool tetris_grid::rotateTetromino(direction dir){

    if (curr_tet->type == O_TETROMINO){
        return false;
    }

    int rotation_0 = 0;
    int rotation_1 = curr_tet->sstride;
    int rotation_2 = curr_tet->sstride * 2;
    int rotation_3 = curr_tet->sstride * 3;

    //storing these values for collision detection and reverting if needed.
    int prev_rotation = curr_tet->current_rotation;
    direction opposite_dir = dir == CWISE ? CCWISE : CWISE;

    removeTetromino();
    curr_tet->rotate(dir);

    bool can_rotate = false;

    // Define the shifting rules for all tetromino shapes but the I_TETROMINO.
    if (curr_tet->type != I_TETROMINO){
        
        if ((prev_rotation == rotation_0 && dir == CWISE) ||
            (prev_rotation == rotation_2 && dir == CCWISE))
        {
            if (!colliding() ||
                peekShiftTetromino(0, -1) ||
                peekShiftTetromino(-1, -1) ||
                peekShiftTetromino(2, 0) ||
                peekShiftTetromino(2, -1))
            {
                can_rotate = true;

            } else {
                curr_tet->rotate(opposite_dir);
                can_rotate = false;
            }

        } else if ((prev_rotation == rotation_1 && dir == CCWISE) ||
                   (prev_rotation == rotation_1 && dir == CWISE))
        {
            if (!colliding() ||
                peekShiftTetromino(0, 1) ||
                peekShiftTetromino(1, 1) ||
                peekShiftTetromino(-2, 0) ||
                peekShiftTetromino(-2, 1))
            {
                can_rotate = true;

            } else {
                curr_tet->rotate(opposite_dir);
                can_rotate = false;
            }

        } else if ((prev_rotation == rotation_2 && dir == CWISE) ||
                   (prev_rotation == rotation_0 && dir == CCWISE))
        {
            if (!colliding() ||
                peekShiftTetromino(0, 1) ||
                peekShiftTetromino(-1, 1) ||
                peekShiftTetromino(2, 0) ||
                peekShiftTetromino(2, 1))
            {
                can_rotate = true;

            } else {
                curr_tet->rotate(opposite_dir);
                can_rotate = false;
            }

        } else if ((prev_rotation == rotation_3 && dir == CCWISE) ||
                   (prev_rotation == rotation_3 && dir == CWISE))
        {
            if (!colliding() ||
                peekShiftTetromino(0, -1) ||
                peekShiftTetromino(1, -1) ||
                peekShiftTetromino(-2, 0) ||
                peekShiftTetromino(-2, -1))
            {
                can_rotate = true;

            } else {
                curr_tet->rotate(opposite_dir);
                can_rotate = false;
            }

        } 
        
    // Define the shifting rules for the I_TETROMINO.
    } else { // curr_tet->type == I_TETROMINO

        if ((prev_rotation == rotation_0 && dir == CWISE) ||
            (prev_rotation == rotation_3 && dir == CCWISE))
        {
            if (!colliding() ||
                peekShiftTetromino(0, -2) ||
                peekShiftTetromino(0, 1) ||
                peekShiftTetromino(1, -2) ||
                peekShiftTetromino(-2, 1))
            {
                can_rotate = true;

            } else {
                curr_tet->rotate(opposite_dir);
                can_rotate = false;
            }

        } else if ((prev_rotation == rotation_1 && dir == CCWISE) ||
                   (prev_rotation == rotation_2 && dir == CWISE))
        {
            if (!colliding() ||
                peekShiftTetromino(0, 2) ||
                peekShiftTetromino(0, -1) ||
                peekShiftTetromino(-1, 2) ||
                peekShiftTetromino(2, -1))
            {
                can_rotate = true;

            } else {
                curr_tet->rotate(opposite_dir);
                can_rotate = false;
            }

        } else if ((prev_rotation == rotation_1 && dir == CWISE) ||
                   (prev_rotation == rotation_0 && dir == CCWISE))
        {
            if (!colliding() ||
                peekShiftTetromino(0, -1) ||
                peekShiftTetromino(0, 2) ||
                peekShiftTetromino(-2, -1) ||
                peekShiftTetromino(1, 2))
            {
                can_rotate = true;

            } else {
                curr_tet->rotate(opposite_dir);
                can_rotate = false;
            }

        } else if ((prev_rotation == rotation_2 && dir == CCWISE) ||
                   (prev_rotation == rotation_3 && dir == CWISE))
        {
            if (!colliding() ||
                peekShiftTetromino(0, 1) ||
                peekShiftTetromino(0, -2) ||
                peekShiftTetromino(2, 1) ||
                peekShiftTetromino(-1, -2))
            {
                can_rotate = true;

            } else {
                curr_tet->rotate(opposite_dir);
                can_rotate = false;
            }
        } 
    }

    placeTetromino();
    return can_rotate;
}

// colliding checks to see that if the current tetromino were to be on the
// grid, if it would be out of bounds or colliding with any squares. 
bool tetris_grid::colliding(){
    
    // Return true if the tetris block colliding with a border.
    if (tet_y_pos + curr_tet->topmost_sqr < 0 || 
        tet_y_pos + curr_tet->bottommost_sqr >= GRID_HEIGHT ||
        tet_x_pos + curr_tet->leftmost_sqr < 0 || 
        tet_x_pos + curr_tet->rightmost_sqr >= GRID_LENGTH)
    {
        return true;
    }

    // Return true if the tetromino is colliding with another tetromino.
    for (int i = 0; i < curr_tet->sstride; i++){
        for (int j = 0; j < curr_tet->sstride; j++){
            if (curr_tet->shapeAt(i, j) != EMPTY_SQR &&
                grid.at(tet_y_pos + i).at(tet_x_pos + j).s_type != EMPTY_SQR)
            {
                return true;
            }
        }
    }

    return false;
}

// removeTetromino replaces the values on the grid that correspond to the
// tetromino's values with empty squares. IT DOES NOT CHECK FOR OUT OF BOUNDS
// OR COLLISIONS.
void tetris_grid::removeTetromino(){

    for (int i = 0; i < curr_tet->sstride; i++){
        for (int j = 0; j < curr_tet->sstride; j++){
            if (curr_tet->shapeAt(i, j) != EMPTY_SQR)
            {
                grid.at(tet_y_pos + i).at(tet_x_pos + j).setEmpty();
            }
        }
    }

}

// placeTetromino replaces the values at the tetris grid's tetromino position
// with the values of the current tetromino. IT DOES NOT CHECK FOR OUT OF
// BOUNDS OR COLLISIONS.
void tetris_grid::placeTetromino(){

    int square_val = 0;
    for (int i = 0; i < curr_tet->sstride; i++){
        for (int j = 0; j < curr_tet->sstride; j++){        
            if (curr_tet->shapeAt(i, j) != EMPTY_SQR)
            {
                square_val = curr_tet->shapeAt(i, j);
                grid.at(tet_y_pos + i).at(tet_x_pos + j).s_type = (square_type)square_val;
                grid.at(tet_y_pos + i).at(tet_x_pos + j).s_color = curr_tet->color;
            }
        }
    }
}

//
void tetris_grid::generateNextTetromino(){
    curr_tet = next_tet;
    next_tet = &tetris_tetrominoes[rand() % 7];
}

//
bool tetris_grid::setCurrTetrominoOnGrid(){

    tet_x_pos = (GRID_LENGTH - curr_tet->sstride) / 2;
    tet_y_pos = -curr_tet->topmost_sqr; // this sets it to the highest value in the 
                                        // shape array if it were to be placed onto 
                                        // the grid with the highest square of the
                                        // tetromino on row zero.
                                        //
                                        // ex.) -2|     0 0 0 0 <- top of shape array
                                        //      -1|     0 0 0 0
                                        // -------|-----------------
                                        //       0|     2 2 2 2 <- highest_square_offset
                                        //       1|     0 0 0 0    (from top of shape array)
                                        //       2|
                                        // 
                                        // tet_y_pos would be - 2

    if (colliding()){
        return false;
    }

    // I replaced may 2d array accesses with pointers in an attempt to shrink
    // code size, but I'm not too sure if it made it any more readable. Also,
    // to make the code more readable, I separated the out of bounds condition
    // from the others, so i could arrange the variable assignments nicely.
    int arr_val;
    square* grid_val;
    for (int i = 0; i < curr_tet->sstride; i++){
        for (int j = 0; j < curr_tet->sstride; j++){

            // Skip the square values that are out of bounds.
            if (!inBounds(tet_y_pos + i, tet_x_pos + j)){
                continue;
            }

            arr_val = curr_tet->shapeAt(i, j);
            grid_val = &grid.at(tet_y_pos + i).at(tet_x_pos + j);

            if (arr_val > 0 && 
                grid_val->s_type == EMPTY_SQR)
            {
                grid_val->s_type = (square_type)arr_val;
                grid_val->s_color = curr_tet->color;
            }
        }
    }

    return true;
}

//
bool tetris_grid::inBounds(int y, int x){
    if (y >= 0 && y < GRID_HEIGHT &&
        x >= 0 && x < GRID_LENGTH)
    {
        return true;
    }
    
    return false;
}