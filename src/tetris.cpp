// John Wesley Thompson
// Created: 8/10/2024
// Completed:
// Last Edited: 
// tetris.cpp

// This file includes all of the implentation for the tetromino class and
// tetris_grid class.


#include "tetris.h"


// TO DO: import the random library for better tetromino generation
// TO DO: improve the refreshTetromino function's speed
// TO DO: create a row clear function and a row check function
// TO DO: create a game over function called topReached()
// TO DO: make the place tetromino on grid function return false or -1 if 
//        a block could not be placed.
// TO DO: rename varables to more intuitive names and call the stack the stack
//        and tetrominoes tetrominoes
// TO DO: create a tetris settings class or strucct that has al of the tetris
//        setting data as wella s getters and setters
// TO DO: move all of the functions that would be more intuitive in the tetromino
//        class from the tetris grid class and into the tetromino class
// TO DO: prefix the variables in the tetris_grid class with m
// TO DO: make appropriate functions pass by reference and const
// TO DO: add a boolean variable to tetris.h that tells if the terminal can
//        print in color so that print functions can use it to print 
//        appropriately
// TO DO: make macros or an enum for the different grid values.
// TO DO: get rid of error checking in the rotate and shift functions that
//        are already included in the movement validation functions
// TO DO: rename grid_pos to tet_pos

// ============================================================================
// tetromino CLASS 
// ============================================================================

tetromino::tetromino(tetromino_type tet_t){

    switch(tet_t){
        case L_TETROMINO:   
            m_shape_arr_ptr = L_TETROMINO_ARR;
            m_shape_stride = 3;

            m_color_id = ORANGE;
            break;

        case J_TETROMINO: 
            m_shape_arr_ptr = J_TETROMINO_ARR;
            m_shape_stride = 3;
            
            m_color_id = BLUE;
            break;

        case T_TETROMINO: 
            m_shape_arr_ptr = T_TETROMINO_ARR;
            m_shape_stride = 3;
            
            m_color_id = PURPLE;
            break;

        case I_TETROMINO: 
            m_shape_arr_ptr = I_TETROMINO_ARR;
            m_shape_stride = 4;

            m_color_id = CYAN;
            break;

        case S_TETROMINO: 
            m_shape_arr_ptr = S_TETROMINO_ARR;
            m_shape_stride = 3;

            m_color_id = GREEN;
            break;

        case Z_TETROMINO: 
            m_shape_arr_ptr = Z_TETROMINO_ARR;
            m_shape_stride = 3;

            m_color_id = RED;
            break;

        case O_TETROMINO: 
            m_shape_arr_ptr = O_TETROMINO_ARR;
            m_shape_stride = 3;
            
            m_color_id = YELLOW;
            break;
    }
}

// This function returns the address of the m_shape_arr_ptr
const int* tetromino::shapeArray(){
    return m_shape_arr_ptr;
}

int tetromino::shapeStride(){
    return m_shape_stride;
}

int tetromino::color(){
    return m_color_id;
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

    int curr_tetromino_index = rand() % 7; // if it is assigned here it's always 2 for some reason.
    curr_tetromino_index = rand() % 7;
    int next_tetromino_index = rand() % 7;

    mvprintw(30, 0, "curr_tetromino_index %i: ", curr_tetromino_index);
    mvprintw(31, 0, "next_tetromino_index %i: ", next_tetromino_index);

    curr_tetromino = &tetris_tetrominoes[curr_tetromino_index];
    next_tetromino = &tetris_tetrominoes[next_tetromino_index];
    m_rotation_offset = curr_tetromino->shapeArray();

    grid.resize(GRID_HEIGHT * GRID_LENGTH);

    grid_pos = 0;

    for (int i = 0; i < GRID_SIZE; i++){
        if (i % GRID_LENGTH == LEFT_WALL_X ||
            i % GRID_LENGTH == RIGHT_WALL_X)
        {
            grid.at(i) = -2;
        }
    }

}

#ifdef DEBUG
void tetris_grid::printInfo(WINDOW* win){

    int c_stride = curr_tetromino->shapeStride();
    const int* c_arr_start = curr_tetromino->shapeArray();
    // const int* n_arr_start = next_tetromino->shapeArray();

    int y, x;
    y = x = 30;
    wmove(win, y, x);

    for (int i = 0; i < c_stride; i++){
        for (int j = 0; j < c_stride; j++){
            wprintw(win, "%i ", *(m_rotation_offset + j + (i * c_stride * POSSIBLE_ROTATIONS)));
        }
        wmove(win, ++y, x);
    }

    wmove(win, y += 2, x);
    wclrtoeol(win);
    wprintw(win, "c_arr_s: %i", c_arr_start);
    wmove(win, ++y, x);
    wclrtoeol(win);
    wprintw(win, "r_o:     %i", m_rotation_offset);
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
    wprintw(win, "gsch: %i", grid_pos);
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

// TO DO: make tetrominoes print in color and only print the grid indices that are 
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
            } else if (grid.at(j + i * GRID_LENGTH) == -2){
                wprintw(grid_win, "[X]");
            } else if (grid.at(j + i * GRID_LENGTH) == 3){
                wprintw(grid_win, "[3]");
            } else {                                        // function because -1s won't
                wprintw(grid_win, "   ");                   // need to be reprinted.
            }
            // wprintw(grid_win, " %i ", grid.at(j + i * GRID_LENGTH));
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
//        y_values instead of having a bunch of caluclations between each
//        print.

// This function only reprints the current tetromino
void tetris_grid::refreshTetromino(WINDOW* grid_win){
    int shape_stride = curr_tetromino->shapeStride();
    int shape_arr_length = shape_stride * POSSIBLE_ROTATIONS;

    // This is the index of where a refresh will start on the grid. It starts
    // above the tetromino and scans the area arounf it for trail values.
    int refresh_idx;
    
    // I don't want to go into depth about how this math works because it's
    // annoying and confusing, but it calculates the individual x and y
    // positions of refresh idx as if it was in a 2D array. These values are
    // later used for positioning the window cursor for reprinting.

    int refresh_idx_y, refresh_idx_x;
    int grid_pos_x, grid_pos_y;
    int refresh_area_height; // + 2 to the top and the bottom
    int refresh_area_width; 
    int x_dif; // This is the difference between the x values of grid_pos and 
               // refresh index.

    if (grid_pos >= 0)
        grid_pos_x = grid_pos % GRID_LENGTH;
    else 
        grid_pos_x = GRID_LENGTH + grid_pos % GRID_LENGTH;

    grid_pos_y = (grid_pos - grid_pos_x) / GRID_LENGTH;

    if (grid_pos_x < LEFT_WALL_X)
        refresh_idx_x = 0;
    else
        refresh_idx_x = grid_pos_x - 2;

    refresh_idx_y = grid_pos_y - 2;
    x_dif = grid_pos_x - refresh_idx_x;
    

    refresh_idx = refresh_idx_x + refresh_idx_y * GRID_LENGTH;

    refresh_area_height = shape_stride + 4;
    refresh_area_width = shape_stride + x_dif + 2;

    for (int i = 0; i < refresh_area_height; i++){
        for (int j = 0; j < refresh_area_width; j++){
            if (refresh_idx + j + i * GRID_LENGTH < 0 ||
                refresh_idx + j + i * GRID_LENGTH >= GRID_SIZE)
            {
                continue;
            } 
            
            // 3 is the width of each printed square, and 1 is added to offset
            // from the border of the ncurses window
            wmove(grid_win, refresh_idx_y + i + 1, (refresh_idx_x + j) * 3 + 1);
            wattron(grid_win, COLOR_PAIR(curr_tetromino->color()));

            if (grid.at(refresh_idx + j + i * GRID_LENGTH) == 1 &&
                j >= x_dif && j < x_dif + shape_stride && i >= 2 && i < 2 + shape_stride &&
                *(m_rotation_offset + (j - x_dif) + (i - 2) * shape_arr_length) == 1)
            {
                wprintw(grid_win, "[ ]");

            } else if (grid.at(refresh_idx + j + i * GRID_LENGTH) == 2 &&
                       j >= x_dif && j < x_dif + shape_stride && i >= 2 && i < 2 + shape_stride &&
                       *(m_rotation_offset + (j - x_dif) + (i - 2) * shape_arr_length) == 2)
            {
                wprintw(grid_win, "[_]");

            } else if (grid.at(refresh_idx + j + i * GRID_LENGTH) == 3){
                wprintw(grid_win, "   ");
            } 

            wattroff(grid_win, COLOR_PAIR(curr_tetromino->color()));
            wrefresh(grid_win);
        }
    }
}

// TETRIS STACK MANIPULATION ==================================================

int stackRemoveRow(){

    // iterate through all rows from the bottom to the top and count how many are complete and gayher their y values
    // shift every square above the 



}

// MOVEMENT AND MOVEMENT VALIDATION ===========================================

// TO DO: simplify this monstrosity
bool tetris_grid::canRotate(const int* test_rotation_offset){

    int shape_stride = curr_tetromino->shapeStride();

    int shape_arr_length = shape_stride * POSSIBLE_ROTATIONS;

    calcLeftandRightSquare(test_rotation_offset);   // These are used to find out THESE NEED TO BE RESET IF THE FUNCTION RETURNS FALSE
    calcTopandBottomSquare(test_rotation_offset);   // if any edges of the shape
                                                    // are colliding or out of 
                                                    // bounds.

    c_collision = false;    // This info is used later to find which  
    c_collision_x = 0;      // way the tetromino should shift if there is 
    c_collision_y = 0;      // only a center collision.

    l_collisions = 0;       // This info is used to determine where the 
    r_collisions = 0;       // tetromino is colliding and to determine which
    t_collisions = 0;       // way the tetromino will shift.
    b_collisions = 0;       // 

    
    for (int i = 0; i < shape_stride; i++){
        for (int j = 0; j < shape_stride; j++){

            if (*(test_rotation_offset + j + i * shape_arr_length) <= 0)
                continue;       // There's no square here.

            // This checks for a top out of bounds.
            if (grid_pos + j + i * GRID_LENGTH < -1){
                continue;       // Do nothing and let the tetromino rotate.
                                // It's annoying to wait for the tetromino to
                                // fall far enough before you can rotate it.

            // This checks for a bottom out of bounds.
            } else if (grid_pos + j + i * GRID_LENGTH > GRID_SIZE){

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
            } else if (j == leftmost_square_offset + 1 && (grid_pos + j + i * GRID_LENGTH) % GRID_LENGTH == LEFT_WALL_X)
            {
                c_collision = true;
                c_collision_y = i;
                c_collision_x = j;

            // center collision on the right wall
            } else if (j == rightmost_square_offset - 1 && (grid_pos + j + i * GRID_LENGTH) % GRID_LENGTH == RIGHT_WALL_X)
            {
                c_collision = true;
                c_collision_y = i;
                c_collision_x = j;
                       
            // This checks to see if the tetromino is colliding with the wall on the right side
            } else if (j == rightmost_square_offset && ((grid_pos + j + i * GRID_LENGTH) % GRID_LENGTH >= RIGHT_WALL_X ||
                       grid_pos + j + i * GRID_LENGTH == RIGHT_WALL_X)) // This line is redundant but I'm keeping it for readability.
            {
                r_collisions++;

                if (i != 0 && i != shape_stride - 1 && 
                    j != 0 && j != shape_stride - 1)
                {
                    c_collision = true;    
                }

            // This checks to see if the tetromino is colliding with the wall on the left side
            } else if (j == leftmost_square_offset && ((grid_pos + j + i * GRID_LENGTH) % GRID_LENGTH <= LEFT_WALL_X ||
                       grid_pos + j + i * GRID_LENGTH == LEFT_WALL_X))
            {
                l_collisions++;

                if (i != 0 && i != shape_stride - 1 && 
                    j != 0 && j != shape_stride - 1)
                {
                    c_collision = true;    
                }
            
            } else if (grid.at(grid_pos + j + i * GRID_LENGTH) == -1){
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
            if (canShiftRight(test_rotation_offset, 2))
                shiftTetromino(2, RIGHT);
            else 
                can_rotate = false;

        } else if (l_collisions && r_collisions){
            can_rotate = false;

        } else if (l_collisions){
            if (canShiftRight(test_rotation_offset, 2))
                shiftTetromino(2, RIGHT);
            else 
                can_rotate = false;

        } else if (r_collisions){

            if (canShiftLeft(test_rotation_offset, 2))
                shiftTetromino(2, LEFT);
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
            can_rotate = false; // A tetromino should not be shifted down for a rotation.

        } else if (b_collisions && l_collisions <= b_collisions){
            // if l_collisions and b_collisions are equal, bottom collisions are prioritized
            // shift up
            if (canShiftUp(test_rotation_offset, 1)){
                shiftTetromino(1, UP);
                
            } else if (canShiftRight(test_rotation_offset, 1)){
                shiftTetromino(1, RIGHT);
                
            } else {
                can_rotate = false;
            }

        } else if (c_collision){
            if (canShiftRight(test_rotation_offset, 2)){
                shiftTetromino(2, RIGHT);
                
            } else {
                can_rotate = false;
            }

        } else {    // Just left collisions
            if (canShiftRight(test_rotation_offset, 1)){
                shiftTetromino(1, RIGHT);

            } else {
                can_rotate = false;
            }
        }
    } else if (r_collisions){
        // If there are more top collisions than right collisions and there are top collisions,
        // return because the player should not be able to shift anywhere.
        if (t_collisions && r_collisions < t_collisions){
            can_rotate = false; // A tetromino should not be shifted down for a rotation.
        
        // If there are more or equal bottom collisions compared to right collisions,
        // prioritise the bottom collisions and shift up.
        } else if (b_collisions && r_collisions <= b_collisions){
            // If r_collisions and b_collisions are equal, bottom collisions are prioritized.
            // shift up
            if (canShiftUp(test_rotation_offset, 1)){
                shiftTetromino(1, UP);
                
            } else if (canShiftLeft(test_rotation_offset, 1)){
                shiftTetromino(1, LEFT);
                
            } else {
                can_rotate = false;
            }
        } else if (c_collision){
            if (canShiftLeft(test_rotation_offset, 2)){
                shiftTetromino(2, LEFT);
                
            } else {
                can_rotate = false;
            }

        } else { // just right collisions
            if (canShiftLeft(test_rotation_offset, 1)){
                shiftTetromino(1, LEFT);
                
            } else {
                can_rotate = false;
            }
        }
        
    } else if (t_collisions){
        if (!b_collisions)
            can_rotate = false;

        else if (l_collisions){
            if (c_collision && canShiftRight(test_rotation_offset, 2)){
                shiftTetromino(2, RIGHT);
            } else if (canShiftRight(test_rotation_offset, 1)){
                shiftTetromino(1, RIGHT);
            } else {
                can_rotate = false;
            }
        } else if (r_collisions){
            if (c_collision && canShiftLeft(test_rotation_offset, 2)){
                shiftTetromino(2, LEFT);
            } else if (canShiftLeft(test_rotation_offset, 1)){
                shiftTetromino(1, LEFT);
            } else {
                can_rotate = false;
            }
        }

    } else if (b_collisions){ // if there is only a bottom collision

        // shift up because there are only bottom collisions
        if (!c_collision && canShiftUp(test_rotation_offset, 1)){
            shiftTetromino(1, UP);
            
        } else if (c_collision && canShiftUp(test_rotation_offset, 2)){
            shiftTetromino(2, UP);
            
        } else {
            can_rotate = false;
        }

    } 

    if (!can_rotate){ // set the following block data according to the original rotation.
        calcLeftandRightSquare(m_rotation_offset);
        calcTopandBottomSquare(m_rotation_offset);
        return false;
    } else {
        return true;
    }
}

// The following rotation functions move the rotation offset forwards
// or backwards in the shape array and rotate the values on the grid.

// TO DO: convert all of the rotate functions into 1 function

void tetris_grid::rotateTetromino(direction rotation_dir){

    const int* array_start = curr_tetromino->shapeArray();
    int shape_stride = curr_tetromino->shapeStride();
    
    int shape_arr_length = shape_stride * POSSIBLE_ROTATIONS;

    if (array_start == O_TETROMINO_ARR) // O_TETROMINO doesn't rotate
        return;

    // if user entered an ambiguous direction to rotate, specify which direction to rotate.
    if (rotation_dir == UP || rotation_dir == LEFT) 
        rotation_dir = CCWISE;
    else if (rotation_dir == DOWN || rotation_dir == RIGHT) 
        rotation_dir = CWISE;

    // Assign next rotation offset
    const int* next_rotation_offset = m_rotation_offset;
    const int* fourth_rotation = array_start + (shape_arr_length - shape_stride);
    
    if (rotation_dir == CWISE){
        if (next_rotation_offset == fourth_rotation){
            next_rotation_offset = array_start;
        } else {
            next_rotation_offset += shape_stride;
        }
    } else { // if dir == CCWISE
        if (next_rotation_offset == array_start){
            next_rotation_offset = fourth_rotation;
        } else {
            next_rotation_offset -= shape_stride;
        }
    }

    if (!canRotate(next_rotation_offset)) return;
    

    // TO DO: the following for loops can easily be combined.

    // replace tetromino's square values with trail values.
    for (int i = 0; i < shape_stride; i++){
        for (int j = 0; j < shape_stride; j++){
            if (grid_pos + j + i * GRID_LENGTH >= 0 &&
                grid.at(grid_pos + j + i * GRID_LENGTH) > 0)
            {    
                grid.at(grid_pos + j + i * GRID_LENGTH) = 3;
            }
        }
    }

    m_rotation_offset = next_rotation_offset;

    // set the new tetromino square values.
    for (int i = 0; i < shape_stride; i++){
        for (int j = 0; j < shape_stride; j++){
            if (grid_pos + j + i * GRID_LENGTH >= 0)
            {
                if (*(m_rotation_offset + j + i * shape_arr_length) == 1)
                    grid.at(grid_pos + j + i * GRID_LENGTH) = 1;
                else if (*(m_rotation_offset + j + i * shape_arr_length) == 2)
                    grid.at(grid_pos + j + i * GRID_LENGTH) = 2;
            }
        }
    }
}

// TO DO: convert all of the canShift functions into 1 function 
bool tetris_grid::canShiftUp(const int* test_rotation_offset, int shift_count){
    int shape_stride = curr_tetromino->shapeStride();
    int shape_arr_length = shape_stride * POSSIBLE_ROTATIONS;

    for (int col = 0; col < shape_stride; col++){
        for (int row = 0; row < shape_stride; row++){
                // if there is a square and
            if (*(test_rotation_offset + col + row * shape_arr_length) > 0 &&
                // the square is on the top row of the grid and in bounds or
                ((grid_pos + col + (row - shift_count) * GRID_LENGTH < 0 && grid_pos + col + row * GRID_LENGTH >= 0) ||
                // there is a square above the square
                (grid_pos + col + (row - shift_count) * GRID_LENGTH >= 0 && grid.at(grid_pos + col + (row - shift_count) * GRID_LENGTH) == -1)))
            {
                return false;
            }
        }
    }
    return true;
}

bool tetris_grid::canShiftDown(const int* test_rotation_offset, int shift_count){
    int shape_stride = curr_tetromino->shapeStride();
    int shape_arr_length = shape_stride * POSSIBLE_ROTATIONS;

    for (int col = 0; col < shape_stride; col++){
        for (int row = shape_stride - 1; row >= 0; row--){
                // if there is a square and
            if (*(test_rotation_offset + col + row * shape_arr_length) > 0 &&
                // the square is on the bottom row and in bounds or
                ((grid_pos + col + (row + shift_count) * GRID_LENGTH >= GRID_SIZE && grid_pos + col + row * GRID_LENGTH < GRID_SIZE) ||
                // there is a square beneath the square
                (grid_pos + col + (row + shift_count) * GRID_LENGTH < GRID_SIZE && 
                grid_pos + col + (row + shift_count) * GRID_LENGTH > 0 &&
                grid.at(grid_pos + col + (row + shift_count) * GRID_LENGTH) == -1)))
            {
                return false;
            }
        }
    }
    return true;
}

bool tetris_grid::canShiftLeft(const int* test_rotation_offset, int shift_count){
    int shape_stride = curr_tetromino->shapeStride();
    int shape_arr_length = shape_stride * POSSIBLE_ROTATIONS;

    calcLeftandRightSquare(test_rotation_offset);

    for (int i = 0; i < shape_stride; i++){
        for (int j = 0; j < shape_stride; j++){
                // if there is a square and
            if (*(test_rotation_offset + j + i * shape_arr_length) > 0 &&
                // if the square is on the left edge of the grid, and in bounds
                (((grid_pos + (leftmost_square_offset - shift_count) + i * GRID_LENGTH) % GRID_LENGTH <= LEFT_WALL_X && grid_pos + j + i * GRID_LENGTH >= 0) ||
                // the square is in the 0th position or 
                // (still on left edge, but if its zero the previous condition
                // tests false because (0 - 1) % GRID_LENGTH == 1
                // and 1 != GRID_LENGTH - 1)
                grid_pos + j + i * GRID_LENGTH == 0 ||
                // there is a square to the left of the square
                (grid_pos + (j - shift_count) + i * GRID_LENGTH >= 0 && grid.at(grid_pos + (j - shift_count) + i * GRID_LENGTH) == -1)))
            {
                return false;
            }
        }
    }
    return true;
}

bool tetris_grid::canShiftRight(const int* test_rotation_offset, int shift_count){
    int shape_stride = curr_tetromino->shapeStride();
    int shape_arr_length = shape_stride * POSSIBLE_ROTATIONS;

    calcLeftandRightSquare(test_rotation_offset);

    for (int i = 0; i < shape_stride; i++){
        for (int j = shape_stride - 1; j >= 0; j--){
            if (*(test_rotation_offset + j + i * shape_arr_length) == 0)
                continue;
                // If the square is on the right edge of the grid and in bounds or
            if (((grid_pos + (rightmost_square_offset + shift_count) + i * GRID_LENGTH) % GRID_LENGTH >= RIGHT_WALL_X && grid_pos + j + i * GRID_LENGTH >= 0) ||
                // there is a square to the right of the square
                (grid_pos + (j + shift_count) + i * GRID_LENGTH >= 0 && grid.at(grid_pos + (j + shift_count) + i * GRID_LENGTH) == -1))
            {
                return false;
            }
        }
    }
    return true;
}

void tetris_grid::shiftTetromino(int shift_count, direction dir){
    int shape_stride = curr_tetromino->shapeStride();
    int shape_arr_length = shape_stride * POSSIBLE_ROTATIONS;

    // replace all squares in tetromino with trail values.
    for (int i = 0; i < shape_stride; i++){
        for (int j = 0; j < shape_stride; j++){
            if ((grid_pos + j + i * GRID_LENGTH) >= 0 &&
                (grid_pos + j + i * GRID_LENGTH) < GRID_SIZE &&
                (grid.at(grid_pos + j + i * GRID_LENGTH) > 0))
            {
                grid.at(grid_pos + j + i * GRID_LENGTH) = 3;
            }
        }
    }
        
    // move grid_pos in the appropriate direction
    if (dir == UP){
        grid_pos -= (shift_count * GRID_LENGTH);
    } else if (dir == DOWN){
        grid_pos += (shift_count * GRID_LENGTH);
    } else if (dir == LEFT){
        grid_pos -= shift_count;
    } else { //dir == RIGHT
        grid_pos += shift_count;
    }

    // reassign tetromino's values
    int square_val = 0;
    for (int i = 0; i < shape_stride; i++){
        for (int j = 0; j < shape_stride; j++){
            if ((grid_pos + j + i * GRID_LENGTH) >= 0 &&
                (grid_pos + j + i * GRID_LENGTH) < GRID_SIZE &&
                (grid.at(grid_pos + j + i * GRID_LENGTH) == 0 ||
                grid.at(grid_pos + j + i * GRID_LENGTH) == 3))
            {        
                if (*(m_rotation_offset + j + i * shape_arr_length) > 0){
                    square_val = *(m_rotation_offset + j + i * shape_arr_length);
                    grid.at(grid_pos + j + i * GRID_LENGTH) = square_val;
                }
            }
        }
    }
}

// void tetris_grid::shiftDown(int shift_count){

//     int shape_stride = curr_tetromino->shapeStride();
//     int shape_arr_length = shape_stride * POSSIBLE_ROTATIONS;

//     // grid_pos = grid_tetromino_location - (shape_stride - 1) * GRID_LENGTH;

//     // erase all tetromino's values
//     for (int i = 0; i < shape_stride; i++){
//         for (int j = 0; j < shape_stride; j++){
//             if ((grid_pos + j + i * GRID_LENGTH) >= 0 &&
//                 (grid_pos + j + i * GRID_LENGTH) < GRID_SIZE &&
//                 (grid.at(grid_pos + j + i * GRID_LENGTH) > 0))
//             {
//                 grid.at(grid_pos + j + i * GRID_LENGTH) = 0;
//             }
//         }
//     }
        
//     // move gsh down
//     grid_pos += (shift_count * GRID_LENGTH);

//     // reassign tetromino's values
//     int square_val = 0;
//     for (int i = 0; i < shape_stride; i++){
//         for (int j = 0; j < shape_stride; j++){
//             if ((grid_pos + j + i * GRID_LENGTH) >= 0 &&
//                 (grid_pos + j + i * GRID_LENGTH) < GRID_SIZE &&
//                 grid.at(grid_pos + j + i * GRID_LENGTH) == 0)
//             {        
//                 if (*(rotation_offset + j + i * shape_arr_length) > 0){
//                     square_val = *(rotation_offset + j + i * shape_arr_length);
//                     grid.at(grid_pos + j + i * GRID_LENGTH) = square_val;
//                 }
//             }
//         }
//     }
// }

// void tetris_grid::shiftLeft(int shift_count){
//     int shape_stride = curr_tetromino->shapeStride();
//     int shape_arr_length = shape_stride * POSSIBLE_ROTATIONS;
//
//     // grid_pos = grid_tetromino_location - (shape_stride - 1) * GRID_LENGTH;
//
//     // erase tetromino's values
//     for (int i = 0; i < shape_stride; i++){
//         for (int j = 0; j < shape_stride; j++){
//             if ((grid_pos + j + i * GRID_LENGTH) >= 0 &&
//                 (grid_pos + j + i * GRID_LENGTH) < GRID_SIZE &&
//                 (grid.at(grid_pos + j + i * GRID_LENGTH) > 0))
//             {
//                 grid.at(grid_pos + j + i * GRID_LENGTH) = 0;
//             }
//         }
//     }
//      
//     // move gsh left
//     grid_pos -= shift_count;
//
//     // reassign tetromino's values
//     int square_val = 0;
//     for (int i = 0; i < shape_stride; i++){
//         for (int j = 0; j < shape_stride; j++){
//             if ((grid_pos + j + i * GRID_LENGTH) >= 0 &&
//                 (grid_pos + j + i * GRID_LENGTH) < GRID_SIZE &&
//                 grid.at(grid_pos + j + i * GRID_LENGTH) == 0)
//             {        
//                 if (*(rotation_offset + j + i * shape_arr_length) > 0){
//                     square_val = *(rotation_offset + j + i * shape_arr_length);
//                     grid.at(grid_pos + j + i * GRID_LENGTH) = square_val;
//                 }
//             }
//         }
//     }
// }

// void tetris_grid::shiftRight(int shift_count){
//     int shape_stride = curr_tetromino->shapeStride();
//     int shape_arr_length = shape_stride * POSSIBLE_ROTATIONS;
//
//     // grid_pos = grid_tetromino_location - (shape_stride - 1) * GRID_LENGTH;
//
//     // erase tetromino's values
//     for (int i = 0; i < shape_stride; i++){
//         for (int j = 0; j < shape_stride; j++){
//             if ((grid_pos + j + i * GRID_LENGTH) >= 0 &&
//                 (grid_pos + j + i * GRID_LENGTH) < GRID_SIZE &&
//                 (grid.at(grid_pos + j + i * GRID_LENGTH) > 0))
//             {
//                 grid.at(grid_pos + j + i * GRID_LENGTH) = 0;
//             }
//         }
//     }
//     
//     // move gsh right
//     grid_pos += shift_count;
//
//     // reassign tetromino's values
//     int square_val = 0;
//     for (int i = 0; i < shape_stride; i++){
//         for (int j = 0; j < shape_stride; j++){
//             if ((grid_pos + j + i * GRID_LENGTH) >= 0 &&
//                 (grid_pos + j + i * GRID_LENGTH) < GRID_SIZE &&
//                 grid.at(grid_pos + j + i * GRID_LENGTH) == 0)
//             {        
//                 if (*(rotation_offset + j + i * shape_arr_length) > 0){
//                     square_val = *(rotation_offset + j + i * shape_arr_length);
//                     grid.at(grid_pos + j + i * GRID_LENGTH) = square_val;
//                 }
//             }
//         }
//     }
// }

// TETROMINO HANDLING =============================================================
// SOME OF THIS SHOULD BELONG TO THE TETROMINO CLASS

void tetris_grid::generateNextTetromino(){
    curr_tetromino = next_tetromino;
    m_rotation_offset = curr_tetromino->shapeArray();
    next_tetromino = &tetris_tetrominoes[rand() % 7];
}

// Calculates the highest row of the current tetromino in the shape array
void tetris_grid::calcTopandBottomSquare(const int* test_rotation_offset){
    
    int shape_stride = curr_tetromino->shapeStride();

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

    int shape_stride = curr_tetromino->shapeStride();
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

void tetris_grid::setCurrTetrominoOnGrid(){

    calcLeftandRightSquare(m_rotation_offset);
    calcTopandBottomSquare(m_rotation_offset);

    int shape_stride = curr_tetromino->shapeStride();
    int grid_start_x = (GRID_LENGTH - shape_stride) / 2;

    int shape_arr_length = shape_stride * POSSIBLE_ROTATIONS;

    grid_pos = grid_start_x - highest_square_offset * GRID_LENGTH;

    int square_val = 0;
    for (int i = 0; i < shape_stride; i++){
        for (int j = 0; j < shape_stride; j++){
            if (*(m_rotation_offset + j + i * shape_arr_length) > 0 &&
                grid_pos + j + i * GRID_LENGTH >= 0 &&
                grid_pos + j + i * GRID_LENGTH < GRID_SIZE && 
                grid.at(grid_pos + j + i * GRID_LENGTH) == 0)
            {
                square_val = *(m_rotation_offset + j + i * shape_arr_length);
                grid.at(grid_pos + j + i * GRID_LENGTH) = square_val;
            }
        }
    }
}

void tetris_grid::settleCurrTetromino(){
    int shape_stride = curr_tetromino->shapeStride();
    int shape_arr_length = shape_stride * POSSIBLE_ROTATIONS;

    for (int i = 0; i < shape_stride; i++){
        for (int j = 0; j < shape_stride; j++){
            if (*(m_rotation_offset + j + i * shape_arr_length) > 0){
                grid.at(grid_pos + j + i * GRID_LENGTH) = -1;
            }
        }
    }
} 
