// John Wesley Thompson
// Created: 8/10/2024
// Last Edited: 12/30/2024
// tetris_grid.cpp

// This file includes all of the implementation for the tetromino class and
// tetris_grid class.

#include <iostream>
#include <ncurses.h>

#include "tetris_grid.h"
#include "tetris_tetromino.h"


// ============================================================================
// TETRIS_GRID CLASS 
// ============================================================================

tetris_grid::tetris_grid(int y, int x){

    height = y;
    length = x;

    curr_tet = tetrominoes.select();
    next_tet = tetrominoes.select();

    // set non garbage values for the position of the tetromino on the grid
    tet_y_pos = tet_x_pos = 0;

    // This resizes the grid to the default values: a height of 20 and
    // a length of 10.
    grid.resize(height);
    for (int i = 0; i < height; i++){
        grid.at(i).resize(length);
    }
}

// TERMINAL OUTPUT ============================================================

void tetris_grid::printGrid(WINDOW* win) const {
    // int win_height, win_length;
    // getmaxyx(win, win_height, win_length);

    // if (win_height > height + 2 || 
    //     win_length > length * 3 + 2)
    //     return;
    
    wmove(win, 0, 0);
    for (int i = 0; i < height; i++){  
        for (int j = 0; j < length; j++){    

            if (grid.at(i).at(j).s_type == EMPTY_SQR){
                wprintw(win, "   ");   
                continue;
            }

            wattron(win, COLOR_PAIR(grid.at(i).at(j).s_color));
            if (grid.at(i).at(j).s_type == TOP_SQR){  
                wprintw(win, "[ ]");
            } else if (grid.at(i).at(j).s_type == BOTTOM_SQR){                                      
                wprintw(win, "[_]");
            }
            //wprintw(win, " %i ", grid.at(i).at(j).s_type);
            wattroff(win, COLOR_PAIR(grid.at(i).at(j).s_color));
        }
        wmove(win, i + 1, 0);
    }

    wrefresh(win);
}

// TETRIS STACK MANIPULATION ==================================================

// clearCompleteRows replaces all of the rows on the grid with all non-empty
// values with empty rows, nd then shifts any floating rows downwards.
int tetris_grid::clearCompleteRows(){ 
    bool row_full;
    int wiped_row_ct = 0;
    for (int i = 0; i < height; i++){

        row_full = true;
        for (int j = 0; j < length; j++){
            if (grid.at(i).at(j).s_type == EMPTY_SQR){
                row_full = false;
                break;
            }
        }

        if (!row_full)
            continue;

        // If the row is full, wipe it.
        for (int j = 0; j < length; j++){
            grid.at(i).at(j).setEmpty();
        }
        wiped_row_ct++;
    }

    bool unempty_row_arr[height];
    int unempty_row_count = 0;

    bool row_empty;
    int lowest_empty_row_idx = 0;
    for (int i = 0; i < height; i++){
        row_empty = true;
        for (int j = 0; j < length; j++){
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

    // Do not shift anything down if nothing was cleared.
    if (wiped_row_ct == 0){
        return wiped_row_ct;
    }

    // Shift all of the now floating rows downwards as far as they can go.
    bool floating = false;
    for (int i = height - 1; i >= 0; i--){
        if (unempty_row_arr[i] == 0){
            floating = true;

        } else if (unempty_row_arr[i] == 1 && floating){
            for (int j = 0; j < length; j++){
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
    
    return wiped_row_ct;
}

// MOVEMENT AND MOVEMENT VALIDATION ===========================================

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
// Input: an x and y modifier for the tetromino's grid location and a value that 
//        commits the modification of the tet positions or doesn't
bool tetris_grid::peekShiftTetromino(int y_shift_ct, int x_shift_ct, bool commit){

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

    // If the tet positions shoudl not be modified when the function returns,
    // revert them.
    if (!commit){
        tet_y_pos = prev_tet_y_pos;
        tet_x_pos = prev_tet_x_pos;
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
                peekShiftTetromino(0, -1, true) ||
                peekShiftTetromino(-1, -1, true) ||
                peekShiftTetromino(2, 0, true) ||
                peekShiftTetromino(2, -1, true))
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
                peekShiftTetromino(0, 1, true) ||
                peekShiftTetromino(1, 1, true) ||
                peekShiftTetromino(-2, 0, true) ||
                peekShiftTetromino(-2, 1, true))
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
                peekShiftTetromino(0, 1, true) ||
                peekShiftTetromino(-1, 1, true) ||
                peekShiftTetromino(2, 0, true) ||
                peekShiftTetromino(2, 1, true))
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
                peekShiftTetromino(0, -1, true) ||
                peekShiftTetromino(1, -1, true) ||
                peekShiftTetromino(-2, 0, true) ||
                peekShiftTetromino(-2, -1, true))
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
                peekShiftTetromino(0, -2, true) ||
                peekShiftTetromino(0, 1, true) ||
                peekShiftTetromino(1, -2, true) ||
                peekShiftTetromino(-2, 1, true))
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
                peekShiftTetromino(0, 2, true) ||
                peekShiftTetromino(0, -1, true) ||
                peekShiftTetromino(-1, 2, true) ||
                peekShiftTetromino(2, -1, true))
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
                peekShiftTetromino(0, -1, true) ||
                peekShiftTetromino(0, 2, true) ||
                peekShiftTetromino(-2, -1, true) ||
                peekShiftTetromino(1, 2, true))
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
                peekShiftTetromino(0, 1, true) ||
                peekShiftTetromino(0, -2, true) ||
                peekShiftTetromino(2, 1, true) ||
                peekShiftTetromino(-1, -2, true))
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

// dropTetromino shifts the current tetromino as far down as it can go on the
// grid.
void tetris_grid::dropTetromino(){
    removeTetromino();

    while(peekShiftTetromino(1, 0, true));

    placeTetromino();
}

// tetrominoDropped is used to determine if the current tetromino is shifted as
// far down as it can be in that moment.
// Output: true if it cannot be shifted down and false otherwise
bool tetris_grid::tetrominoDropped(){

    return !peekShiftTetromino(1, 0, false);
}

// colliding checks to see that if the current tetromino were to be on the
// grid, if it would be out of bounds or colliding with any squares. 
bool tetris_grid::colliding() const {
    
    // Return true if the tetris block colliding with a border.
    if (tet_y_pos + curr_tet->topmost_sqr < 0 || 
        tet_y_pos + curr_tet->bottommost_sqr >= height ||
        tet_x_pos + curr_tet->leftmost_sqr < 0 || 
        tet_x_pos + curr_tet->rightmost_sqr >= length)
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

// generateNextTetromino sets the current tetromino pointer equal to the next
// tetromino pointer and assigns the next tetromino pointer to a random
// tetromino.
void tetris_grid::generateNextTetromino(){
    curr_tet->resetRotation();
    curr_tet = next_tet;
    
    if (tetrominoes.empty()){
        tetrominoes.refill();
    }
    
    next_tet = tetrominoes.select();
}

// setCurrTetrominoOnGrid assigns the tetromino values to the top of the grid 
// where each tetromino is supposed to start.
bool tetris_grid::setCurrTetrominoOnGrid(){

    tet_x_pos = (length - curr_tet->sstride) / 2;
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

    for (int i = 0; i < curr_tet->sstride; i++){
        for (int j = 0; j < curr_tet->sstride; j++){

            // Skip the square values that are out of bounds.
            if (curr_tet->shapeAt(i, j) == EMPTY_SQR){
                continue;
            }

            grid.at(tet_y_pos + i).at(tet_x_pos + j).s_type = (square_type)curr_tet->shapeAt(i, j);
            grid.at(tet_y_pos + i).at(tet_x_pos + j).s_color = curr_tet->color;
        }
    }

    return true;
}