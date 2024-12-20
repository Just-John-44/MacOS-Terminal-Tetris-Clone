// John Wesley Thompson
// Created: 8/10/2024
// Completed:
// Last Edited: 10/17/2024
// tetris.cpp

// This file includes all of the implentation for the tetromino class and
// tetris_grid class.


#include "tetris.h"


// TO DO: tetris init should create the window that it's displayed in as well.
// TO DO: import the random library for better tetromino generation
// TO DO: improve the refreshTetromino function's speed
// TO DO: create a row clear function and a row check function
// TO DO: create a game over function called topReached()
// TO DO: make the place tetromino on grid function return false or -1 if 
//        a block could not be placed.
// TO DO: create a tetris settings class or struct that has al of the tetris
//        setting data as wella s getters and setters
// TO DO: move all of the functions that would be more intuitive in the tetromino
//        class from the tetris grid class and into the tetromino class
// TO DO: prefix the variables in the tetris_grid class with m
// TO DO: make appropriate functions pass by reference and const
// TO DO: get rid of error checking in the rotate and shift functions that
//        are already included in the movement validation functions
// TO DO: rename grid_pos to tet_pos
// TO DO: make array start, shapestride, and shape arr length member variables.
// TO DO: make the terminal print in only black and white if it doesn't support colors.
// TO DO: maybe code would be simplified if i made curr block a tetromino variable and
//        set it equal to the next block each time. maybe it would run faster because I
//        wouldn't have to dereference the pointer each time i want info.
// TO DO: I should probably make the tetromino arrays square types instead of ints
// TO DO: calc left and right should take arguments in as parameters instead of direcetly
//        modifying the tetris struct's data. Also, it makes more sense for these to be
//        on the tetromino class.
// TO DO: make a set curr block data function that gathers all of the information about
//        the current block so it can be used instead fo crowding other function's code     

// ============================================================================
// TETRIS_GRID CLASS 
// ============================================================================

int out_of_bounds_buffer = 2;

int GRID_HEIGHT = 20;
int GRID_LENGTH = 10;
int GRID_SIZE = GRID_HEIGHT * GRID_LENGTH;

tetris_grid::tetris_grid(){

    curr_tetromino = tetris_tetrominoes[rand() % 7];
    next_tetromino = tetris_tetrominoes[rand() % 7];

    setCurrTetrominoData();

    // set non garbage values for the position of the tetromino on the grid
    tet_y_pos = tet_x_pos = 0;

    // This resizes the grid to the default values: a height of 20 and
    // a length of 10.
    grid.resize(GRID_HEIGHT);
    for (int i = 0; i < GRID_HEIGHT; i++){
        grid.at(i).resize(GRID_LENGTH);
    }
}

void tetris_grid::printInfo(WINDOW* win){

    int y, x;
    
    y = 30;
    wmove(win, y, 0);
    wclrtoeol(win);
    wprintw(win, "tet_y: %i", tet_y_pos);
    wmove(win, y += 2, 0);
    wclrtoeol(win);
    wprintw(win, "tet_x: %i", tet_x_pos);
    wmove(win, y += 2, 0);

    y = 30;
    x = 30;
    wmove(win, y, x);
    wclrtoeol(win);
    wprintw(win, "c_arr_s: %x", m_curr_arr_start);
    wmove(win, ++y, x);
    wclrtoeol(win);
    wprintw(win, "r_o:     %x", m_curr_rotation_offset);
    wmove(win, y += 2, x);
    wclrtoeol(win);
    wprintw(win, "c_highest: %i", topmost_square_offset);
    wmove(win, ++y, x);
    wclrtoeol(win);
    wprintw(win, "c_lowest:  %i", bottommost_square_offset);
    wmove(win, ++y, x);
    wclrtoeol(win);
    wprintw(win, "c_leftmost:  %i", leftmost_square_offset);
    wmove(win, ++y, x);
    wclrtoeol(win);
    wprintw(win, "c_rightmost:  %i", rightmost_square_offset);
    wmove(win, ++y, x);
    wclrtoeol(win);
    wprintw(win, "c_lowest - 12: %i", bottommost_square_offset - 12);
    wmove(win, ++y, x);
    wclrtoeol(win);
    wprintw(win, "c_lowest - 24: %i", bottommost_square_offset - 24);
    wmove(win, y += 2, x);
    wclrtoeol(win);
    wprintw(win, "gbl: %i", 0); // not used anymore

    y = 30;
    x = 50;
    wmove(win, y ++, x);
    wclrtoeol(win);
    const int* arr_ptr = O_TETROMINO_ARR;
    wprintw(win, "O_arr_s: %x", arr_ptr);
    wmove(win, y ++, x);
    wclrtoeol(win);
    arr_ptr = J_TETROMINO_ARR;
    wprintw(win, "J_arr_s: %x", arr_ptr);
    wmove(win, y ++, x);
    wclrtoeol(win);
    arr_ptr = I_TETROMINO_ARR;
    wprintw(win, "I_arr_s: %x", arr_ptr);
    wmove(win, y ++, x);
    wclrtoeol(win);
    arr_ptr = S_TETROMINO_ARR;
    wprintw(win, "S_arr_s: %x", arr_ptr);
    wmove(win, y ++, x);
    wclrtoeol(win);
    arr_ptr = Z_TETROMINO_ARR;
    wprintw(win, "Z_arr_s: %x", arr_ptr);
    wmove(win, y ++, x);
    wclrtoeol(win);
    arr_ptr = L_TETROMINO_ARR;
    wprintw(win, "L_arr_s: %x", arr_ptr);
    wmove(win, y ++, x);
    wclrtoeol(win);
    arr_ptr = T_TETROMINO_ARR;
    wprintw(win, "T_arr_s: %x", arr_ptr);

    wmove(win, y ++, x);
    wclrtoeol(win);
    wprintw(win, "test t tet: %i, %i, %i", T_TETROMINO_ARR[0], T_TETROMINO_ARR[1], T_TETROMINO_ARR[2]);
    wmove(win, y ++, x);
    wclrtoeol(win);
    wprintw(win, "test arr ptr: %i, %i, %i", *(arr_ptr), *(arr_ptr + 1), *(arr_ptr + 2));
    

    wrefresh(win);
}

// TERMINAL OUTPUT ============================================================

// TO DO: only print the grid indices that are within bounds.
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
            //wprintw(grid_win, " (%2i, %i)", grid.at(i).at(j).s_type, grid.at(i).at(j).s_color);
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
//        y_values instead of having a bunch of caluclations between each
//        print.

// This function only reprints the current tetromino
void tetris_grid::refreshTetromino(WINDOW* grid_win){

    int refresh_idx_y = tet_y_pos - 2;
    int refresh_idx_x = tet_x_pos - 2;

    int refresh_area_height = m_curr_shape_stride + 4; // +2 on all sides of shape array
    int refresh_area_width = m_curr_shape_stride + 4;  //

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

bool tetris_grid::canRotate(direction dir){
    
    if (m_curr_arr_start == O_TETROMINO_ARR){ // O tetromino doesn't rotate
        return false;
    }

    square grid_square_val;     // used to make code more readable in the 
    int prev_square_val;        // following loops
    int curr_square_val;        //

    const int* rotation_1;  // This the rotation after the start of the array,
    const int* rotation_2;  //    so m_curr_arr_start is like rotation_0.
    const int* rotation_3;
    rotation_1 = m_curr_arr_start + (m_curr_shape_stride);
    rotation_2 = m_curr_arr_start + (m_curr_shape_stride * 2);
    rotation_3 = m_curr_arr_start + (m_curr_shape_stride * 3);

    const int* prev_rotation_offset = m_curr_rotation_offset;


    if (dir == UP || dir == DOWN || dir == LEFT || dir == RIGHT)
        return false;

    if (dir == CWISE){
        if (m_curr_rotation_offset == rotation_3){
            m_curr_rotation_offset = m_curr_arr_start;
        } else {
            m_curr_rotation_offset += m_curr_shape_stride;
        }
    } else { // if dir == CCWISE
        if (m_curr_rotation_offset == m_curr_arr_start){
            m_curr_rotation_offset = rotation_3;
        } else {
            m_curr_rotation_offset -= m_curr_shape_stride;
        }
    }


    // find if there is a collision with the next rotation offset
    bool collision = false;
    for (int i = 0; i < m_curr_shape_stride; i++){
        for (int j = 0; j < m_curr_shape_stride; j++){

            prev_square_val = *(prev_rotation_offset + j + i * m_curr_shape_arr_len);
            curr_square_val = *(m_curr_rotation_offset + j + i * m_curr_shape_arr_len);

            if (!inBounds(tet_y_pos + i, tet_x_pos + j)){
                if (curr_square_val != EMPTY_SQR)
                    collision = true; // The tetromino is colliding with a wall.
                continue;
            }

            grid_square_val = grid.at(tet_y_pos + i).at(tet_x_pos + j);

            if (curr_square_val != EMPTY_SQR && prev_square_val == EMPTY_SQR &&   // if the tetromino is 
                grid_square_val.s_type != EMPTY_SQR)                              // colliding with
            {                                                                     // another tetromino
                collision = true;
            }

            if (collision) break;
        }
        if (collision) break;
    }

    if (!collision){
        m_curr_rotation_offset = prev_rotation_offset;
        return true;
    }

    bool can_rotate = true;
    if (m_curr_arr_start == L_TETROMINO_ARR ||
        m_curr_arr_start == J_TETROMINO_ARR ||
        m_curr_arr_start == T_TETROMINO_ARR){
        
        if (dir == CWISE){
            if (canShiftLeft(1)){
                blindShiftTetromino(prev_rotation_offset,1, LEFT);
            } else if (canShiftRight(1)){
                blindShiftTetromino(prev_rotation_offset,1, RIGHT);
            } else if (canShiftUp(1)){
                blindShiftTetromino(prev_rotation_offset,1, UP);
            } else {
                can_rotate = false;
            } 

        } else {// dir == CCWISE

            if (canShiftLeft(1)){
                blindShiftTetromino(prev_rotation_offset,1, LEFT);
            } else if (canShiftRight(1)){
                blindShiftTetromino(prev_rotation_offset,1, RIGHT);
            } else if (canShiftUp(1)){
                blindShiftTetromino(prev_rotation_offset,1, UP);
            } else {
                can_rotate = false;
            } 
            // if (canShiftRight(1)){
            //     blindShiftTetromino(prev_rotation_offset,1, RIGHT);
            // } else if (canShiftLeft(1)){
            //     blindShiftTetromino(prev_rotation_offset,1, LEFT);
            // } else if (canShiftUp(1)){
            //     blindShiftTetromino(prev_rotation_offset,1, UP);
            // } else {
            //     can_rotate = false;
            // } 
        }

    } else if (m_curr_arr_start == I_TETROMINO_ARR){

        if ((m_curr_rotation_offset == m_curr_arr_start ||
            m_curr_rotation_offset == rotation_2) && dir == CWISE)
        {
            if (canShiftLeft(1)){
                blindShiftTetromino(prev_rotation_offset,1, LEFT);
            } else if (canShiftRight(1)){
                blindShiftTetromino(prev_rotation_offset,1, RIGHT);
            } else if (canShiftUp(1)){
                blindShiftTetromino(prev_rotation_offset,1, UP);
            } else {
                can_rotate = false;
            } 
            
        } else if ((m_curr_rotation_offset == m_curr_arr_start ||
                   m_curr_rotation_offset == rotation_2) && dir == CCWISE)
        {
            if (canShiftRight(1)){
                blindShiftTetromino(prev_rotation_offset,1, RIGHT);
            } else if (canShiftLeft(1)){
                blindShiftTetromino(prev_rotation_offset,1, LEFT);
            } else if (canShiftUp(1)){
                blindShiftTetromino(prev_rotation_offset,1, UP);
            } else {
                can_rotate = false;
            } 

        } else if ((m_curr_rotation_offset == rotation_1 ||
                   m_curr_rotation_offset == rotation_3) && dir == CWISE)
        {
            if (canShiftRight(1)){
                blindShiftTetromino(prev_rotation_offset,1, RIGHT);
            } else if (canShiftLeft(1)){
                blindShiftTetromino(prev_rotation_offset,1, LEFT);
            } else if (canShiftUp(1)){
                blindShiftTetromino(prev_rotation_offset,1, UP);
            } else {
                can_rotate = false;
            } 

        } else if ((m_curr_rotation_offset == rotation_1 ||
                   m_curr_rotation_offset == rotation_3) && dir == CCWISE)
        {
            if (canShiftLeft(1)){
                blindShiftTetromino(prev_rotation_offset,1, LEFT);
            } else if (canShiftRight(1)){
                blindShiftTetromino(prev_rotation_offset,1, RIGHT);
            } else if (canShiftUp(1)){
                blindShiftTetromino(prev_rotation_offset,1, UP);
            } else {
                can_rotate = false;
            } 
        }

    } else {    // m_curr_arr_start == S_TETROMINO_ARR ||
                // m_curr_arr_start == Z_TETROMINO_ARR
        if (m_curr_rotation_offset == m_curr_arr_start || 
            m_curr_rotation_offset == rotation_2)
        {
            if (canShiftRight(1)){
                blindShiftTetromino(prev_rotation_offset,1, RIGHT);
            } else if (canShiftLeft(1)){
                blindShiftTetromino(prev_rotation_offset,1, LEFT);
            } else if (canShiftUp(1)){
                blindShiftTetromino(prev_rotation_offset,1, UP);
            } else {
                can_rotate = false;
            }

        } else {// next rotation offset == rotation 1 or 3
            if (canShiftLeft(1)){
                blindShiftTetromino(prev_rotation_offset,1, LEFT);
            } else if (canShiftRight(1)){
                blindShiftTetromino(prev_rotation_offset,1, RIGHT);
            } else if (canShiftUp(1)){
                blindShiftTetromino(prev_rotation_offset,1, UP);
            } else {
                can_rotate = false;
            }
        }
    }

    m_curr_rotation_offset = prev_rotation_offset;
    
    return can_rotate;
}

// The following rotation functions move the rotation offset forwards
// or backwards in the shape array and rotate the values on the grid.

void tetris_grid::rotateTetromino(direction rotation_dir){

    if (m_curr_arr_start == O_TETROMINO_ARR) // O_TETROMINO doesn't rotate
        return;

    // if user entered an ambiguous direction to rotate, specify which direction to rotate.
    if (rotation_dir == UP || rotation_dir == LEFT) 
        rotation_dir = CCWISE;
    else if (rotation_dir == DOWN || rotation_dir == RIGHT) 
        rotation_dir = CWISE;

    // Assign next rotation offset
    const int* next_rotation_offset = m_curr_rotation_offset;
    const int* rotation_3 = m_curr_arr_start + m_curr_shape_stride * 3;
    
    if (rotation_dir == CWISE){
        if (next_rotation_offset == rotation_3){
            next_rotation_offset = m_curr_arr_start;
        } else {
            next_rotation_offset += m_curr_shape_stride;
        }
    } else { // if dir == CCWISE
        if (next_rotation_offset == m_curr_arr_start){
            next_rotation_offset = rotation_3;
        } else {
            next_rotation_offset -= m_curr_shape_stride;
        }
    }

    if (!canRotate(rotation_dir)) return;


    // set new tetromino square values.

    int cro_square_val;  // m_curr_rotation_offset square val
    int nro_square_val;  // next_rotation_offset square val
    square grid_val;

    for (int i = 0; i < m_curr_shape_stride; i++){
        for (int j = 0; j < m_curr_shape_stride; j++){
    
            if (!inBounds(tet_y_pos + i, tet_x_pos + j))
                continue;

            grid_val = grid.at(tet_y_pos + i).at(tet_x_pos + j);
            cro_square_val = *(m_curr_rotation_offset + j + i * m_curr_shape_arr_len);
            nro_square_val = *(next_rotation_offset + j + i * m_curr_shape_arr_len);

            // If the square isn't empty, make it empty
            if (cro_square_val != EMPTY_SQR)
                grid_val.setEmpty();
            
            // Replace the square's value with the next rotation's square value
            // without overwriting a different square.
            if (nro_square_val != EMPTY_SQR){
                grid_val.s_type = (square_type)nro_square_val;
                grid_val.s_color = curr_tetromino.color();
            }

            // Set the actual grid value to the newly modified one.
            grid.at(tet_y_pos + i).at(tet_x_pos + j) = grid_val;
        }
    }

    // Set the new rotation offset.
    m_curr_rotation_offset = next_rotation_offset;
    curr_tetromino.calcTopandBottomSquare(m_curr_rotation_offset,
                                          topmost_square_offset,
                                          bottommost_square_offset);
    curr_tetromino.calcLeftandRightSquare(m_curr_rotation_offset,
                                          leftmost_square_offset,
                                          rightmost_square_offset);
}

bool tetris_grid::canShiftUp(int shift_count){

    for (int col = 0; col < m_curr_shape_stride; col++){
        for (int row = 0; row < m_curr_shape_stride; row++){
                // if there is a square and
            if (*(m_curr_rotation_offset + col + row * m_curr_shape_arr_len) == 0){
                continue;
            } 
                // the shifted square is out of bounds
            if (!inBounds(tet_y_pos + row - shift_count, tet_x_pos + col) || 
                // there is a square above the square
                grid.at(tet_y_pos + row - shift_count).at(tet_x_pos + col).s_type != EMPTY_SQR)
            {
                return false;
            } else {
                // skip to the next line to avoid checking if the tetromino is colliding with itself.
                break;
            }
        }
    }
    return true;
}

bool tetris_grid::canShiftDown(int shift_count){

    for (int col = 0; col < m_curr_shape_stride; col++){
        for (int row = m_curr_shape_stride - 1; row >= 0; row--){
        
            if (*(m_curr_rotation_offset + col + row * m_curr_shape_arr_len) == 0){
                continue;
            } 

                // the shifted square is out of bounds
            if (!inBounds(tet_y_pos + row + shift_count, tet_x_pos + col) ||
                // there is a square beneath the square 
                grid.at(tet_y_pos + row + shift_count).at(tet_x_pos + col).s_type != EMPTY_SQR)
            {
                return false;
            } else {
                // skip to the next line to avoid checking if the tetromino is colliding with itself.
                break;
            }
        }
    }
    return true;
}

bool tetris_grid::canShiftLeft(int shift_count){

    bool left_edge_checked;
    for (int row = 0; row < m_curr_shape_stride; row++){
        left_edge_checked = false;

        for (int col = 0; col < m_curr_shape_stride; col++){
            
            if (*(m_curr_rotation_offset + col + row * m_curr_shape_arr_len) == 0){
                continue;
            }

            if (!inBounds(tet_y_pos + row, tet_x_pos + col - shift_count)){
                return false;
            }

            // If the leftmost square on this row has not been checked for a future collision,
            // check it.
            if (!left_edge_checked &&
                grid.at(tet_y_pos + row).at(tet_x_pos + col - shift_count).s_type != EMPTY_SQR)
            {
                return false;

            } else {
                // the leftmost edge should be checked if the previous if statement executed.
                left_edge_checked = true;
            }  
        }
    }
    return true;
}

bool tetris_grid::canShiftRight(int shift_count){

    bool right_edge_checked;
    for (int row = 0; row < m_curr_shape_stride; row++){
        right_edge_checked = false;

        for (int col = m_curr_shape_stride - 1; col >= 0; col--){

            if (*(m_curr_rotation_offset + col + row * m_curr_shape_arr_len) == 0){
                continue;
            }

            // If the rightmost square on this row has been checked for a future collision, 
            // skip to the next row.
            if (right_edge_checked){
                break;
            }

            if (!inBounds(tet_y_pos + row, tet_x_pos + col + shift_count) || 
                grid.at(tet_y_pos + row).at(tet_x_pos + col + shift_count).s_type != EMPTY_SQR)
            {    
                return false;
            }

            // the rightmost edge should be checked if the previous if statement executed.
            right_edge_checked = true;
        }
    }

    return true;
}

void tetris_grid::shiftTetromino(int shift_count, direction dir){

    // move grid_pos in the appropriate direction
    int next_tet_y_pos = tet_y_pos;
    int next_tet_x_pos = tet_x_pos;
    if (dir == UP && canShiftUp(shift_count)){
        next_tet_y_pos -= shift_count;
    } else if (dir == DOWN && canShiftDown(shift_count)){
        next_tet_y_pos += shift_count;
    } else if (dir == LEFT && canShiftLeft(shift_count)){
        next_tet_x_pos -= shift_count;
    } else if (dir == RIGHT && canShiftRight(shift_count)){
        next_tet_x_pos += shift_count;
    } else {
        // The block is unable to shift if none of the previous conditions are true.
        return;
    }

    // replace all squares in tetromino with empty values.
    for (int i = 0; i < m_curr_shape_stride; i++){
        for (int j = 0; j < m_curr_shape_stride; j++){
            if (inBounds(tet_y_pos + i, tet_x_pos + j) &&
                *(m_curr_rotation_offset + j + i * m_curr_shape_arr_len) > 0)
            {
                grid.at(tet_y_pos + i).at(tet_x_pos + j).setEmpty();
            }
        }
    }

    tet_y_pos = next_tet_y_pos;
    tet_x_pos = next_tet_x_pos;

    // reassign tetromino's values
    int square_val = 0;
    for (int i = 0; i < m_curr_shape_stride; i++){
        for (int j = 0; j < m_curr_shape_stride; j++){
            if (inBounds(tet_y_pos + i, tet_x_pos + j) &&
                grid.at(tet_y_pos + i).at(tet_x_pos + j).s_type == 0) // this shouldn't be needed after canShift() is added to the function.
            {        
                if (*(m_curr_rotation_offset + j + i * m_curr_shape_arr_len) > 0)
                {
                    square_val = *(m_curr_rotation_offset + j + i * m_curr_shape_arr_len);
                    grid.at(tet_y_pos + i).at(tet_x_pos + j).s_type = (square_type)square_val;
                    grid.at(tet_y_pos + i).at(tet_x_pos + j).s_color = curr_tetromino.color();
                }
            }
        }
    }
}

// This function has a very specific use case. It shifts the tetromino position
// in the specified direction. It erases its values from the grid, but does not 
// reassign them, so a function to reassign them should be used afterwards.
void tetris_grid::blindShiftTetromino(const int* rotation_offset, int shift_count, direction dir){

    // replace all squares in tetromino with empty values.
    for (int i = 0; i < m_curr_shape_stride; i++){
        for (int j = 0; j < m_curr_shape_stride; j++){
            if (inBounds(tet_y_pos + i, tet_x_pos + j) &&
                *(rotation_offset + j + i * m_curr_shape_arr_len) > 0)
            {
                grid.at(tet_y_pos + i).at(tet_x_pos + j).setEmpty();
            }
        }
    }

    // move grid_pos in the appropriate direction
    if (dir == UP){
        tet_y_pos -= shift_count;
    } else if (dir == DOWN){
        tet_y_pos += shift_count;
    } else if (dir == LEFT){
        tet_x_pos -= shift_count;
    } else if (dir == RIGHT){
        tet_x_pos += shift_count;
    } else {
        return; // an invalid direction variable was passed.
    }
}

// TETROMINO HANDLING =============================================================
// SOME OF THIS SHOULD BELONG TO THE TETROMINO CLASS

void tetris_grid::generateNextTetromino(){
    curr_tetromino = next_tetromino;
    next_tetromino = tetris_tetrominoes[rand() % 7];

    // set new curr_tetrimono data
    setCurrTetrominoData();
}

void tetris_grid::setCurrTetrominoOnGrid(){

    tet_x_pos = (GRID_LENGTH - m_curr_shape_stride) / 2;
    tet_y_pos = -topmost_square_offset; // this sets it to the highest value in the 
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

    // I replaced may 2d array accesses with pointers in an attempt to shrink
    // code size, but I'm not too sure if it made it any more readable. Also,
    // to make the code more readable, I separated the out of bounds condition
    // from the others, so i could arrange the variable assignments nicely.
    int arr_val;
    square* grid_val;
    for (int i = 0; i < m_curr_shape_stride; i++){
        for (int j = 0; j < m_curr_shape_stride; j++){

            if (!inBounds(tet_y_pos + i, tet_x_pos + j)){
                continue;
            }

            arr_val = *(m_curr_rotation_offset + j + i * m_curr_shape_arr_len);
            grid_val = &grid.at(tet_y_pos + i).at(tet_x_pos + j);

            if (arr_val > 0 && 
                grid_val->s_type == EMPTY_SQR)
            {
                grid_val->s_type = (square_type)arr_val;
                grid_val->s_color = curr_tetromino.color();
            }
        }
    }
}

bool tetris_grid::inBounds(int y, int x){
    if (y >= 0 && y < GRID_HEIGHT &&
        x >= 0 && x < GRID_LENGTH)
    {
        return true;
    }
    
    return false;
}

// This function copies all of the current block's data to variables, so that
// can be used within the tetris_grid class without calling their getters each
// time.
void tetris_grid::setCurrTetrominoData(){
    m_curr_arr_start = curr_tetromino.shapeArray();
    m_curr_rotation_offset = m_curr_arr_start;
    m_curr_shape_stride = curr_tetromino.shapeStride();
    m_curr_shape_arr_len = m_curr_shape_stride * POSSIBLE_ROTATIONS;
    curr_tetromino.calcTopandBottomSquare(m_curr_rotation_offset, 
                                          topmost_square_offset, 
                                          bottommost_square_offset);
    curr_tetromino.calcLeftandRightSquare(m_curr_rotation_offset, 
                                          leftmost_square_offset, 
                                          rightmost_square_offset);
}