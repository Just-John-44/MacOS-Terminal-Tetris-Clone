// John Wesley Thompson
// Created:
// Completed:
// tetris.cpp

// This file includes all of the implentation for the block class and
// tetris_grid class.


#include "tetris.h"


// TO DO: import the random library for better block generation
// TO DO: create a gridRefresh function that only reprints the block
// TO DO: create a row clear function snd a row check function
// TO DO: create a game over function called topReached()
// TO DO: rename varables to more intuitive names and call the stack the stack
//        and blocks tetriminos
// TO DO: create a tetris settings class or strucct that has al of the tetris
//        setting data as wella s getters and setters
// TO DO: move all of the functions that would be more intuitive in the block
//        class from the tetris grid class and into the block class


// ============================================================================
// BLOCK CLASS 
// ============================================================================

block::block(block_type bt){

    switch(bt){
        case L_BLOCK:   
            m_shape_arr_ptr = L_BLOCK_ARR;
            m_shape_stride = 3;

            m_color = COLOR_MAGENTA;
            break;

        case J_BLOCK: 
            m_shape_arr_ptr = J_BLOCK_ARR;
            m_shape_stride = 3;
            
            m_color = COLOR_BLUE;
            break;

        case T_BLOCK: 
            m_shape_arr_ptr = T_BLOCK_ARR;
            m_shape_stride = 3;
            
            m_color = COLOR_BLUE;
            break;

        case I_BLOCK: 
            m_shape_arr_ptr = I_BLOCK_ARR;
            m_shape_stride = 4;

            m_color = COLOR_CYAN;
            break;

        case S_BLOCK: 
            m_shape_arr_ptr = S_BLOCK_ARR;
            m_shape_stride = 3;

            m_color = COLOR_GREEN;
            break;

        case Z_BLOCK: 
            m_shape_arr_ptr = Z_BLOCK_ARR;
            m_shape_stride = 3;

            m_color = COLOR_RED;
            break;

        case O_BLOCK: 
            m_shape_arr_ptr = O_BLOCK_ARR;
            m_shape_stride = 3;
            
            m_color = COLOR_YELLOW;
            break;
    }
}

// This function returns the address of the m_shape_arr_ptr
const int* block::shapeArray(){
    return m_shape_arr_ptr;
}

int block::shapeStride(){
    return m_shape_stride;
}

int block::color(){
    return m_color;
}

// ============================================================================
// TETRIS_GRID CLASS 
// ============================================================================

int out_of_bounds_buffer = 2;

int GRID_HEIGHT = 20;
int GRID_LENGTH = 10 + out_of_bounds_buffer * 2;
int GRID_SIZE = GRID_HEIGHT * GRID_LENGTH;

int LEFT_WALL_X = 1;
int RIGHT_WALL_X = GRID_LENGTH - 2;

tetris_grid::tetris_grid(){

    int curr_block_index = rand() % 7; // if it is assigned here it's always 2 for some reason.
    curr_block_index = rand() % 7;
    int next_block_index = rand() % 7;

    mvprintw(30, 0, "curr_block_index %i: ", curr_block_index);
    mvprintw(31, 0, "next_block_index %i: ", next_block_index);

    curr_block = &tetris_blocks[curr_block_index];
    next_block = &tetris_blocks[next_block_index];
    rotation_offset = curr_block->shapeArray();

    grid.resize(GRID_HEIGHT * GRID_LENGTH);

    grid_search = 0;

    for (int i = 0; i < GRID_SIZE; i++){
        if (i % GRID_LENGTH == LEFT_WALL_X ||
            i % GRID_LENGTH == RIGHT_WALL_X)
        {
            grid.at(i) = 5;
        }
    }

}

#ifdef DEBUG
void tetris_grid::printInfo(WINDOW* win){

    int c_stride = curr_block->shapeStride();
    const int* c_arr_start = curr_block->shapeArray();
    // const int* n_arr_start = next_block->shapeArray();

    int y, x;
    y = x = 30;
    wmove(win, y, x);

    for (int i = 0; i < c_stride; i++){
        for (int j = 0; j < c_stride; j++){
            wprintw(win, "%i ", *(rotation_offset + j + (i * c_stride * POSSIBLE_ROTATIONS)));
        }
        wmove(win, ++y, x);
    }

    wmove(win, y += 2, x);
    wclrtoeol(win);
    wprintw(win, "c_arr_s: %i", c_arr_start);
    wmove(win, ++y, x);
    wclrtoeol(win);
    wprintw(win, "r_o:     %i", rotation_offset);
    wmove(win, y += 2, x);
    wclrtoeol(win);
    wprintw(win, "c_highest: %i", highest_square_offset);
    wmove(win, ++y, x);
    wclrtoeol(win);
    wprintw(win, "c_lowest:  %i", lowest_square_offset);
    wmove(win, ++y, x);
    wclrtoeol(win);
    wprintw(win, "c_leftmost:  %i", leftmost_square_offset);
    wmove(win, ++y, x);
    wclrtoeol(win);
    wprintw(win, "c_rightmost:  %i", rightmost_square_offset);
    wmove(win, ++y, x);
    wclrtoeol(win);
    wprintw(win, "c_lowest - 12: %i", lowest_square_offset - 12);
    wmove(win, ++y, x);
    wclrtoeol(win);
    wprintw(win, "c_lowest - 24: %i", lowest_square_offset - 24);
    wmove(win, y += 2, x);
    wclrtoeol(win);
    wprintw(win, "gbl: %i", 0); // not used anymore
    
    int y2 = 30;
    wclrtoeol(win);
    wprintw(win, "gsch: %i", grid_search);
    wmove(win, y2 += 2, 0);
    wprintw(win, "lc: %i   ", l_collisions);
    wmove(win, ++y2, 0);
    wprintw(win, "rc: %i   ", r_collisions);
    wmove(win, ++y2, 0);
    wprintw(win, "tc: %i   ", t_collisions);
    wmove(win, ++y2, 0);
    wprintw(win, "bc: %i   ", b_collisions);
    wmove(win, ++y2, 0);
    wprintw(win, "cc: %i", c_collision);
    wmove(win, ++y2, 0);
    wprintw(win, "ccy: %i   ", c_collision_y); 
    wmove(win, ++y2, 0);
    wprintw(win, "ccx: %i   ", c_collision_x);
    


    wrefresh(win);
}
#endif

// TERMINAL OUTPUT ============================================================

void tetris_grid::gridMoveCursor(WINDOW* grid_win, int y, int x){
    wmove(grid_win, y, x * 3);
}

// TO DO: make blocks print in color an donly print the grid indices that are 
//        within bounds.
void tetris_grid::printGrid(WINDOW* grid_win){
    int win_height, win_length;
    getmaxyx(grid_win, win_height, win_length);

    if (win_height < (GRID_HEIGHT) + 2 || 
        win_length < (GRID_LENGTH * 3) + 2)
        return;

    wmove(grid_win, 1, 1);
    for (int i = 0; i < GRID_HEIGHT; i++){  
        for (int j = 0; j < GRID_LENGTH; j++){     
            if (grid.at(j + i * GRID_LENGTH) == 1){
                wprintw(grid_win, "[ ]");
            } else if (grid.at(j + i * GRID_LENGTH) == 2){
                wprintw(grid_win, "[_]");
            } else if (grid.at(j + i * GRID_LENGTH) == -1){ // This code won't be useful 
                wprintw(grid_win, "[X]");                   // when i create a refresh
            } else if (grid.at(j + i * GRID_LENGTH) == 5){
                wprintw(grid_win, "[X]");
            } else {                                        // fuction because -1s won't
                wprintw(grid_win, "   ");                   // need to be reprinted.
            }
            // wprintw(grid_win, " %i ", grid.at(j + i * GRID_LENGTH));
        }
        wmove(grid_win, i + 2, 1);
    }
    wrefresh(grid_win);
}

// MOVEMENT AND MOVEMENT VALIDATION ===========================================

// TO DO: simplify this monstrosity
bool tetris_grid::canRotate(const int* test_rotation_offset){

    const int* array_start = curr_block->shapeArray();
    int shape_stride = curr_block->shapeStride();

    if (array_start == O_BLOCK_ARR) 
        return false;

    int shape_arr_length = shape_stride * POSSIBLE_ROTATIONS;

    // Create a pointer to the next rotation position and test for a valid rotation.
    const int* next_rotation_offset = rotation_offset;
    if (rotation_offset == array_start + 
        (shape_stride * (POSSIBLE_ROTATIONS - 1))){
        next_rotation_offset = array_start;
    } else {
        next_rotation_offset = rotation_offset + shape_stride;
    }

    calcLeftandRightSquare(next_rotation_offset);   // These are used to find out THESE NEED TO BE RESET IF THE FUNCTION RETURNS FALSE
    calcTopandBottomSquare(next_rotation_offset);   // if any edges of the shape
                                                    // are colliding or out of 
                                                    // bounds.

    c_collision = false;    // This info is used later to find which  
    c_collision_x = 0;      // way the block should shift if there is 
    c_collision_y = 0;      // only a center collision.

    l_collisions = 0;       // This info is used to determine where the 
    r_collisions = 0;       // block is colliding and to determine which
    t_collisions = 0;       // way the block will shift.
    b_collisions = 0;       // 

    
    for (int i = 0; i < shape_stride; i++){
        for (int j = 0; j < shape_stride; j++){

            if (*(next_rotation_offset + j + i * shape_arr_length) <= 0)
                continue;       // There's no square here.

            // This checks for a top out of bounds.
            if (grid_search + j + i * GRID_LENGTH < -1){
                continue;       // Do nothing and let the block rotate.
                                // It's annoying to wait for the block to
                                // fall far enough before you can rotate it.

            // This checks for a bottom out of bounds.
            } else if (grid_search + j + i * GRID_LENGTH > GRID_SIZE){

                b_collisions++; // There could still be a left collision or 
                                // right collision in this case, but if the bottom
                                // of the shape is out of bounds, it should only 
                                // upwards.

                if (i != 0 && i != shape_stride - 1 && 
                    j != 0 && j != shape_stride - 1)
                {
                    c_collision = true;    
                }

            // center collision on the left wall
            } else if (j == leftmost_square_offset + 1 && (grid_search + j + i * GRID_LENGTH) % GRID_LENGTH == LEFT_WALL_X)
            {
                c_collision = true;
                c_collision_y = i;
                c_collision_x = j;

            // center collision on the right wall
            } else if (j == rightmost_square_offset - 1 && (grid_search + j + i * GRID_LENGTH) % GRID_LENGTH == RIGHT_WALL_X)
            {
                c_collision = true;
                c_collision_y = i;
                c_collision_x = j;
                       
            // This checks to see if the block is colliding with the wall on the right side
            } else if (j == rightmost_square_offset && ((grid_search + j + i * GRID_LENGTH) % GRID_LENGTH >= RIGHT_WALL_X ||
                       grid_search + j + i * GRID_LENGTH == RIGHT_WALL_X)) // This line is redundant but I'm keeping it for readability.
            {
                r_collisions++;

                if (i != 0 && i != shape_stride - 1 && 
                    j != 0 && j != shape_stride - 1)
                {
                    c_collision = true;    
                }

            // This checks to see if the block is colliding with the wall on the left side
            } else if (j == leftmost_square_offset && ((grid_search + j + i * GRID_LENGTH) % GRID_LENGTH <= LEFT_WALL_X ||
                       grid_search + j + i * GRID_LENGTH == LEFT_WALL_X))
            {
                l_collisions++;

                if (i != 0 && i != shape_stride - 1 && 
                    j != 0 && j != shape_stride - 1)
                {
                    c_collision = true;    
                }
            
            } else if (grid.at(grid_search + j + i * GRID_LENGTH) == -1){
                // Increment all edge collisions if there are any.
                if (i == highest_square_offset){
                    t_collisions++;
                }
                if (i == lowest_square_offset){
                    b_collisions++;
                }
                if (j == leftmost_square_offset){
                    l_collisions++;
                } 
                if (j == rightmost_square_offset){
                    r_collisions++;
                }

                // Set center collision true if there is one.
                if (i != 0 && i != shape_stride - 1 && 
                    j != 0 && j != shape_stride - 1)
                {
                    c_collision = true;
                    c_collision_y = i;
                    c_collision_x = j;
                    
                }
            }
        }
    }

    bool can_rotate = true;

    if (c_collision){

        if (t_collisions && b_collisions){
            if (canShiftRight(next_rotation_offset, 2))
                shiftBlock(2, RIGHT);
            else 
                can_rotate = false;

        } else if (l_collisions && r_collisions){
            can_rotate = false;

        } else if (l_collisions){
            if (canShiftRight(next_rotation_offset, 2))
                shiftBlock(2, RIGHT);
            else 
                can_rotate = false;

        } else if (r_collisions){

            if (canShiftLeft(next_rotation_offset, 2))
                shiftBlock(2, LEFT);
            else    
                can_rotate = false;

        } else {
            if ((c_collision_x + 1) <= shape_stride / 2){
                l_collisions++;
            } else {
                r_collisions++;
            }
        }
     
    } else if (l_collisions){
        if (t_collisions && l_collisions < t_collisions){
            can_rotate = false; // A block should not be shifted down for a rotation.

        } else if (b_collisions && l_collisions <= b_collisions){
            // if l_collisions and b_collisions are equal, bottom collisions are prioritized
            // shift up
            if (canShiftUp(next_rotation_offset, 1)){
                shiftBlock(1, UP);
                
            } else if (canShiftRight(next_rotation_offset, 1)){
                shiftBlock(1, RIGHT);
                
            } else {
                can_rotate = false;
            }
        } else if (c_collision){
            printf("HELLOO");
            if (canShiftRight(next_rotation_offset, 2)){
                shiftBlock(2, RIGHT);
                
            } else {
                can_rotate = false;
            }
        } else {    // Just left collisions
            if (canShiftRight(next_rotation_offset, 1)){
                shiftBlock(1, RIGHT);

            } else {
                can_rotate = false;
            }
        }
    } else if (r_collisions){
        // If there are more top collisions than right collisions and there are top collisions,
        // return because the player should not be able to shift anywhere.
        if (t_collisions && r_collisions < t_collisions){
            can_rotate = false; // A block should not be shifted down for a rotation.
        
        // If there are more or equal bottom collisions compared to right collisions,
        // prioritise the bottom collisions and shift up.
        } else if (b_collisions && r_collisions <= b_collisions){
            // If r_collisions and b_collisions are equal, bottom collisions are prioritized.
            // shift up
            if (canShiftUp(next_rotation_offset, 1)){
                shiftBlock(1, UP);
                
            } else if (canShiftLeft(next_rotation_offset, 1)){
                shiftBlock(1, LEFT);
                
            } else {
                can_rotate = false;
            }
        } else if (c_collision){
            if (canShiftLeft(next_rotation_offset, 2)){
                shiftBlock(2, LEFT);
                
            } else {
                can_rotate = false;
            }

        } else { // just right collisions
            if (canShiftLeft(next_rotation_offset, 1)){
                shiftBlock(1, LEFT);
                
            } else {
                can_rotate = false;
            }
        }
        
    } else if (t_collisions){
        if (!b_collisions)
            can_rotate = false;

        else if (l_collisions){
            if (c_collision && canShiftRight(next_rotation_offset, 2)){
                shiftBlock(2, RIGHT);
            } else if (canShiftRight(next_rotation_offset, 1)){
                shiftBlock(1, RIGHT);
            } else {
                can_rotate = false;
            }
        } else if (r_collisions){
            if (c_collision && canShiftLeft(next_rotation_offset, 2)){
                shiftBlock(2, LEFT);
            } else if (canShiftLeft(next_rotation_offset, 1)){
                shiftBlock(1, LEFT);
            } else {
                can_rotate = false;
            }
        }

    } else if (b_collisions){ // if there is only a bottom collision

        // shift up because there are only bottom collisions
        if (!c_collision && canShiftUp(next_rotation_offset, 1)){
            shiftBlock(1, UP);
            
        } else if (c_collision && canShiftUp(next_rotation_offset, 2)){
            shiftBlock(2, UP);
            
        } else {
            can_rotate = false;
        }

    } 

    if (!can_rotate){
        calcLeftandRightSquare(rotation_offset);
        calcTopandBottomSquare(rotation_offset);
        return false;
    } else {
        return true;
    }
}

// The following rotation functions move the rotation offset forwards
// or backwards in the shape array and rotate the values on the grid.

// TO DO: convert all of the rotate functions into 1 function
void tetris_grid::rotateR(){

    const int* array_start = curr_block->shapeArray();
    int shape_stride = curr_block->shapeStride();
    
    int shape_arr_length = shape_stride * POSSIBLE_ROTATIONS;
    

    // Erase the block's old rotation from the grid.
    for (int i = 0; i < shape_stride; i++){
        for (int j = 0; j < shape_stride; j++){
            if (grid_search + j + i * GRID_LENGTH >= 0 &&
                grid.at(grid_search + j + i * GRID_LENGTH) > 0)
            {    
                grid.at(grid_search + j + i * GRID_LENGTH) = 0;
            }
        }
    }

    // Assign new rotation offset
    if (rotation_offset == array_start + 
        (shape_stride * (POSSIBLE_ROTATIONS - 1))){
        rotation_offset = array_start;
    } else {
        rotation_offset = rotation_offset + shape_stride;
    }

    // Print the block's new rotation to the grid.
    for (int i = 0; i < shape_stride; i++){
        for (int j = 0; j < shape_stride; j++){
            if (grid_search + j + i * GRID_LENGTH >= 0)
            {        
                if (*(rotation_offset + j + i * shape_arr_length) == 1)
                    grid.at(grid_search + j + i * GRID_LENGTH) = 1;
                else if (*(rotation_offset + j + i * shape_arr_length) == 2)
                    grid.at(grid_search + j + i * GRID_LENGTH) = 2;
            }
        }
    }
}

void tetris_grid::rotateL(){

    const int* array_start = curr_block->shapeArray();
    int shape_stride = curr_block->shapeStride();
    
    if (array_start == O_BLOCK_ARR) 
        return;

    int shape_arr_length = shape_stride * POSSIBLE_ROTATIONS;
    //int grid_search = grid_block_location - (shape_stride - 1) * GRID_LENGTH;

    // erase all blocks 1s and 2s
    for (int i = 0; i < shape_stride; i++){
        for (int j = 0; j < shape_stride; j++){
            if (grid_search + j + i * GRID_LENGTH >= 0 &&
                grid.at(grid_search + j + i * GRID_LENGTH) > 0)
            {
                grid.at(grid_search + j + i * GRID_LENGTH) = 0;
            }
        }
    }

    if (rotation_offset == array_start){
        rotation_offset += (shape_stride * (POSSIBLE_ROTATIONS - 1));
    } else {
        rotation_offset -= shape_stride;
    }

    // reassign blocks 1s from gbl up
    for (int i = 0; i < shape_stride; i++){
        for (int j = 0; j < shape_stride; j++){
            if (grid_search + j + i * GRID_LENGTH >= 0)
            {        
                if (*(rotation_offset + j + i * shape_arr_length) == 1)
                    grid.at(grid_search + j + i * GRID_LENGTH) = 1;
                else if (*(rotation_offset + j + i * shape_arr_length) == 2)
                    grid.at(grid_search + j + i * GRID_LENGTH) = 2;
            }
        }
    }
}


// TO DO: convert all of the canShift functions into 1 function 
bool tetris_grid::canShiftUp(const int* test_rotation_offset, int shift_count){
    int shape_stride = curr_block->shapeStride();
    int shape_arr_length = shape_stride * POSSIBLE_ROTATIONS;

    for (int col = 0; col < shape_stride; col++){
        for (int row = 0; row < shape_stride; row++){
                // if there is a square and
            if (*(test_rotation_offset + col + row * shape_arr_length) > 0 &&
                // the square is on the top row of the grid and in bounds or
                ((grid_search + col + (row - shift_count) * GRID_LENGTH < 0 && grid_search + col + row * GRID_LENGTH >= 0) ||
                // there is a square above the square
                (grid_search + col + (row - shift_count) * GRID_LENGTH >= 0 && grid.at(grid_search + col + (row - shift_count) * GRID_LENGTH) == -1)))
            {
                return false;
            }
        }
    }
    return true;
}

bool tetris_grid::canShiftDown(const int* test_rotation_offset, int shift_count){
    int shape_stride = curr_block->shapeStride();
    int shape_arr_length = shape_stride * POSSIBLE_ROTATIONS;

    for (int col = 0; col < shape_stride; col++){
        for (int row = shape_stride - 1; row >= 0; row--){
                // if there is a square and
            if (*(test_rotation_offset + col + row * shape_arr_length) > 0 &&
                // the square is on the bottom row and in bounds or
                ((grid_search + col + (row + shift_count) * GRID_LENGTH >= GRID_SIZE && grid_search + col + row * GRID_LENGTH < GRID_SIZE) ||
                // there is a square beneath the square
                (grid_search + col + (row + shift_count) * GRID_LENGTH < GRID_SIZE && 
                grid_search + col + (row + shift_count) * GRID_LENGTH > 0 &&
                grid.at(grid_search + col + (row + shift_count) * GRID_LENGTH) == -1)))
            {
                return false;
            }
        }
    }
    return true;
}

bool tetris_grid::canShiftLeft(const int* test_rotation_offset, int shift_count){
    int shape_stride = curr_block->shapeStride();
    int shape_arr_length = shape_stride * POSSIBLE_ROTATIONS;

    calcLeftandRightSquare(test_rotation_offset);

    for (int i = 0; i < shape_stride; i++){
        for (int j = 0; j < shape_stride; j++){
                // if there is a square and
            if (*(test_rotation_offset + j + i * shape_arr_length) > 0 &&
                // if the square is on the left edge of the grid, and in bounds
                (((grid_search + (leftmost_square_offset - shift_count) + i * GRID_LENGTH) % GRID_LENGTH <= LEFT_WALL_X && grid_search + j + i * GRID_LENGTH >= 0) ||
                // the square is in the 0th position or 
                // (still on left edge, but if its zero the previous condition
                // tests false because (0 - 1) % GRID_LENGTH == 1
                // and 1 != GRID_LENGTH - 1)
                grid_search + j + i * GRID_LENGTH == 0 ||
                // there is a square to the left of the square
                (grid_search + (j - shift_count) + i * GRID_LENGTH >= 0 && grid.at(grid_search + (j - shift_count) + i * GRID_LENGTH) == -1)))
            {
                return false;
            }
        }
    }
    return true;
}

bool tetris_grid::canShiftRight(const int* test_rotation_offset, int shift_count){
    int shape_stride = curr_block->shapeStride();
    int shape_arr_length = shape_stride * POSSIBLE_ROTATIONS;

    calcLeftandRightSquare(test_rotation_offset);

    for (int i = 0; i < shape_stride; i++){
        for (int j = shape_stride - 1; j >= 0; j--){
            if (*(test_rotation_offset + j + i * shape_arr_length) == 0)
                continue;
                // If the square is on the right edge of the grid and in bounds or
            if (((grid_search + (rightmost_square_offset + shift_count) + i * GRID_LENGTH) % GRID_LENGTH >= RIGHT_WALL_X && grid_search + j + i * GRID_LENGTH >= 0) ||
                // there is a square to the right of the square
                (grid_search + (j + shift_count) + i * GRID_LENGTH >= 0 && grid.at(grid_search + (j + shift_count) + i * GRID_LENGTH) == -1))
            {
                return false;
            }
        }
    }
    return true;
}

void tetris_grid::shiftBlock(int shift_count, direction dir){
    int shape_stride = curr_block->shapeStride();
    int shape_arr_length = shape_stride * POSSIBLE_ROTATIONS;

    // erase all block's values
    for (int i = 0; i < shape_stride; i++){
        for (int j = 0; j < shape_stride; j++){
            if ((grid_search + j + i * GRID_LENGTH) >= 0 &&
                (grid_search + j + i * GRID_LENGTH) < GRID_SIZE &&
                (grid.at(grid_search + j + i * GRID_LENGTH) > 0))
            {
                grid.at(grid_search + j + i * GRID_LENGTH) = 0;
            }
        }
    }
        
    // move grid_search in the appropriate direction
    if (dir == UP){
        grid_search -= (shift_count * GRID_LENGTH);
    } else if (dir == DOWN){
        grid_search += (shift_count * GRID_LENGTH);
    } else if (dir == LEFT){
        grid_search -= shift_count;
    } else { //dir == RIGHT
        grid_search += shift_count;
    }
    
    // reassign block's values
    int square_val = 0;
    for (int i = 0; i < shape_stride; i++){
        for (int j = 0; j < shape_stride; j++){
            if ((grid_search + j + i * GRID_LENGTH) >= 0 &&
                (grid_search + j + i * GRID_LENGTH) < GRID_SIZE &&
                grid.at(grid_search + j + i * GRID_LENGTH) == 0)
            {        
                if (*(rotation_offset + j + i * shape_arr_length) > 0){
                    square_val = *(rotation_offset + j + i * shape_arr_length);
                    grid.at(grid_search + j + i * GRID_LENGTH) = square_val;
                }
            }
        }
    }
}

// void tetris_grid::shiftDown(int shift_count){

//     int shape_stride = curr_block->shapeStride();
//     int shape_arr_length = shape_stride * POSSIBLE_ROTATIONS;

//     // grid_search = grid_block_location - (shape_stride - 1) * GRID_LENGTH;

//     // erase all block's values
//     for (int i = 0; i < shape_stride; i++){
//         for (int j = 0; j < shape_stride; j++){
//             if ((grid_search + j + i * GRID_LENGTH) >= 0 &&
//                 (grid_search + j + i * GRID_LENGTH) < GRID_SIZE &&
//                 (grid.at(grid_search + j + i * GRID_LENGTH) > 0))
//             {
//                 grid.at(grid_search + j + i * GRID_LENGTH) = 0;
//             }
//         }
//     }
        
//     // move gsh down
//     grid_search += (shift_count * GRID_LENGTH);

//     // reassign block's values
//     int square_val = 0;
//     for (int i = 0; i < shape_stride; i++){
//         for (int j = 0; j < shape_stride; j++){
//             if ((grid_search + j + i * GRID_LENGTH) >= 0 &&
//                 (grid_search + j + i * GRID_LENGTH) < GRID_SIZE &&
//                 grid.at(grid_search + j + i * GRID_LENGTH) == 0)
//             {        
//                 if (*(rotation_offset + j + i * shape_arr_length) > 0){
//                     square_val = *(rotation_offset + j + i * shape_arr_length);
//                     grid.at(grid_search + j + i * GRID_LENGTH) = square_val;
//                 }
//             }
//         }
//     }
// }

// void tetris_grid::shiftLeft(int shift_count){
//     int shape_stride = curr_block->shapeStride();
//     int shape_arr_length = shape_stride * POSSIBLE_ROTATIONS;
//
//     // grid_search = grid_block_location - (shape_stride - 1) * GRID_LENGTH;
//
//     // erase block's values
//     for (int i = 0; i < shape_stride; i++){
//         for (int j = 0; j < shape_stride; j++){
//             if ((grid_search + j + i * GRID_LENGTH) >= 0 &&
//                 (grid_search + j + i * GRID_LENGTH) < GRID_SIZE &&
//                 (grid.at(grid_search + j + i * GRID_LENGTH) > 0))
//             {
//                 grid.at(grid_search + j + i * GRID_LENGTH) = 0;
//             }
//         }
//     }
//      
//     // move gsh left
//     grid_search -= shift_count;
//
//     // reassign block's values
//     int square_val = 0;
//     for (int i = 0; i < shape_stride; i++){
//         for (int j = 0; j < shape_stride; j++){
//             if ((grid_search + j + i * GRID_LENGTH) >= 0 &&
//                 (grid_search + j + i * GRID_LENGTH) < GRID_SIZE &&
//                 grid.at(grid_search + j + i * GRID_LENGTH) == 0)
//             {        
//                 if (*(rotation_offset + j + i * shape_arr_length) > 0){
//                     square_val = *(rotation_offset + j + i * shape_arr_length);
//                     grid.at(grid_search + j + i * GRID_LENGTH) = square_val;
//                 }
//             }
//         }
//     }
// }

// void tetris_grid::shiftRight(int shift_count){
//     int shape_stride = curr_block->shapeStride();
//     int shape_arr_length = shape_stride * POSSIBLE_ROTATIONS;
//
//     // grid_search = grid_block_location - (shape_stride - 1) * GRID_LENGTH;
//
//     // erase block's values
//     for (int i = 0; i < shape_stride; i++){
//         for (int j = 0; j < shape_stride; j++){
//             if ((grid_search + j + i * GRID_LENGTH) >= 0 &&
//                 (grid_search + j + i * GRID_LENGTH) < GRID_SIZE &&
//                 (grid.at(grid_search + j + i * GRID_LENGTH) > 0))
//             {
//                 grid.at(grid_search + j + i * GRID_LENGTH) = 0;
//             }
//         }
//     }
//     
//     // move gsh right
//     grid_search += shift_count;
//
//     // reassign block's values
//     int square_val = 0;
//     for (int i = 0; i < shape_stride; i++){
//         for (int j = 0; j < shape_stride; j++){
//             if ((grid_search + j + i * GRID_LENGTH) >= 0 &&
//                 (grid_search + j + i * GRID_LENGTH) < GRID_SIZE &&
//                 grid.at(grid_search + j + i * GRID_LENGTH) == 0)
//             {        
//                 if (*(rotation_offset + j + i * shape_arr_length) > 0){
//                     square_val = *(rotation_offset + j + i * shape_arr_length);
//                     grid.at(grid_search + j + i * GRID_LENGTH) = square_val;
//                 }
//             }
//         }
//     }
// }

// BLOCK HANDLING =============================================================
// SOME OF THIS SHOULD BELONG TO THE BLOCK CLASS

void tetris_grid::generateNextBlock(){
    curr_block = next_block;
    rotation_offset = curr_block->shapeArray();
    next_block = &tetris_blocks[rand() % 7];
}

// Calculates the highest row of the current block in the shape array
void tetris_grid::calcTopandBottomSquare(const int* test_rotation_offset){
    
    int shape_stride = curr_block->shapeStride();

    // Since the shape array is not 2D, to step through it like it is 2D, there
    // needs to be a multiplier that will get you to the next "row". This case
    // is more complicated because i only want to read a quarter of the array
    // like its 2D, and because I'm working with pointer and not the index
    // operator.
    //   To read from only a section of the array like it's 2D, there needs to
    // be an offset that, once you've made it to the new "row", can be used
    // to put you back on the correct "column".

    int shape_arr_length = shape_stride * POSSIBLE_ROTATIONS;
    // Hit is true when a 1 is found in the part of the array that is being searched.
    int hit = false;
    for (int row = 0; row < shape_stride; row++){
        for (int col = 0; col < shape_stride; col++){
            if (*(test_rotation_offset + col + row * shape_arr_length) > 0){
                highest_square_offset = row;
                hit = true;
                break;
            }  
        }
        if (hit) break;
    }

    hit = false;
    for (int row = shape_stride - 1; row >= 0; row--){
        for (int col = 0; col < shape_stride; col++){
            if (*(test_rotation_offset + col + row * shape_arr_length) > 0){
                lowest_square_offset = row;
                hit = true;
                break;
            }  
        }
        if (hit) break;
    }
}

void tetris_grid::calcLeftandRightSquare(const int* test_rotation_offset){

    int shape_stride = curr_block->shapeStride();
    int shape_arr_length = shape_stride * POSSIBLE_ROTATIONS;

    int hit = false;
    for (int col = 0; col < shape_stride; col++){
        for (int row = 0; row < shape_stride; row++){
            if (*(test_rotation_offset + col + row * shape_arr_length) > 0){
                leftmost_square_offset = col;
                hit = true;
                break;
            }  
        }
        if (hit) break;
    }

    hit = false;
    for (int col = shape_stride - 1; col >= 0; col--){
        for (int row = 0; row < shape_stride; row++){
            if (*(test_rotation_offset + col + row * shape_arr_length) > 0){
                rightmost_square_offset = col;
                hit = true;
                break;
            }  
        }
        if (hit) break;
    }
}

void tetris_grid::setCurrBlockOnGrid(){

    calcLeftandRightSquare(rotation_offset);
    calcTopandBottomSquare(rotation_offset);

    int shape_stride = curr_block->shapeStride();
    int grid_start_x = (GRID_LENGTH - shape_stride) / 2;

    int shape_arr_length = shape_stride * POSSIBLE_ROTATIONS;

    // This is the number of rows in the current block that have squares in them.
    // int shape_square_row_ct = ((rotation_offset + lowest_square_offset * shape_arr_length) - 
    //                           (rotation_offset + highest_square_offset * shape_arr_length)) / 
    //                           shape_arr_length + 1;

    //const int* highest_square_row = rotation_offset + highest_square_offset * shape_arr_length;

    // int uncut_rows = shape_stride - (highest_square_row - rotation_offset) / shape_arr_length; I DONT THINK THIS IS NEEDED ANYMORE
    grid_search = grid_start_x - highest_square_offset * GRID_LENGTH;

    int square_val = 0;
    for (int i = 0; i < shape_stride; i++){ // HEY! Why not just have it start the rotation offset and grid earch and only print the numbers if they're in bounds?
        for (int j = 0; j < shape_stride; j++){
            if (*(rotation_offset + j + i * shape_arr_length) > 0 &&
                grid_search + j + i * GRID_LENGTH >= 0 &&
                grid_search + j + i * GRID_LENGTH < GRID_SIZE && 
                grid.at(grid_search + j + i * GRID_LENGTH) == 0)
            {
                square_val = *(rotation_offset + j + i * shape_arr_length);
                grid.at(grid_search + j + i * GRID_LENGTH) = square_val;
            }
        }
    }
}

void tetris_grid::settleCurrBlock(){
    int shape_stride = curr_block->shapeStride();
    int shape_arr_length = shape_stride * POSSIBLE_ROTATIONS;

    for (int i = 0; i < shape_stride; i++){
        for (int j = 0; j < shape_stride; j++){
            if (*(rotation_offset + j + i * shape_arr_length) > 0){
                grid.at(grid_search + j + i * GRID_LENGTH) = -1;
            }
        }
    }
} 
