// John Wesley Thompson
// Created: 8/10/2024
// Completed:
// Last Edited: 10/24/2024
// tetris_tetromino.h


#include "tetris_tetromino.h"


// DATA =======================================================================

const int L_TETROMINO_ARR[3][12] = 
{
    { 0, 0, 1,  0, 1, 0,  0, 0, 0,  2, 1, 0 },  // [ ]
    { 2, 2, 2,  0, 1, 0,  1, 2, 2,  0, 1, 0 },  // [ ]		[ ][_][_]
    { 0, 0, 0,  0, 2, 2,  2, 0, 0,  0, 2, 0 }   // [_][_]	[_]
};
const int J_TETROMINO_ARR[3][12] = 
{
    { 1, 0, 0,  0, 1, 2,  0, 0, 0,  0, 1, 0 },  // [ ][ ]
    { 2, 2, 2,  0, 1, 0,  2, 2, 1,  0, 1, 0 },  // [ ]		[_][_][ ]
    { 0, 0, 0,  0, 2, 0,  0, 0, 2,  2, 2, 0 }   // [_]   	      [_]
};
const int T_TETROMINO_ARR[3][12] = 
{
    { 0, 1, 0,  0, 1, 0,  0, 0, 0,  0, 1, 0 },  // [ ]		
    { 2, 2, 2,  0, 1, 2,  2, 1, 2,  2, 1, 0 },  // [ ][_]	   [ ]
    { 0, 0, 0,  0, 2, 0,  0, 2, 0,  0, 2, 0 }   // [_]		[_][_][_]
};
const int I_TETROMINO_ARR[4][16] = 
{ 
    { 0, 0, 0, 0,  0, 0, 1, 0,  0, 0, 0, 0,  0, 1, 0, 0 },  // [ ]	
    { 2, 2, 2, 2,  0, 0, 1, 0,  0, 0, 0, 0,  0, 1, 0, 0 },  // [ ]	
    { 0, 0, 0, 0,  0, 0, 1, 0,  2, 2, 2, 2,  0, 1, 0, 0 },  // [ ]	[_][_][_][_]
    { 0, 0, 0, 0,  0, 0, 2, 0,  0, 0, 0, 0,  0, 2, 0, 0 }   // [_]	
}; 
const int S_TETROMINO_ARR[3][12] = 
{
    { 0, 1, 2,  0, 1, 0,  0, 0, 0,  1, 0, 0 },  // [ ]		
    { 2, 2, 0,  0, 2, 1,  0, 1, 2,  2, 1, 0 },  // [_][ ]	   [ ][_]
    { 0, 0, 0,  0, 0, 2,  2, 2, 0,  0, 2, 0 }   //    [_]	[_][_]	
};
const int Z_TETROMINO_ARR[3][12] = 
{
    { 2, 1, 0,  0, 0, 1,  0, 0, 0,  0, 1, 0 },  //    [ ]	
    { 0, 2, 2,  0, 1, 2,  2, 1, 0,  1, 2, 0 },  // [ ][_]	[_][ ]
    { 0, 0, 0,  0, 2, 0,  0, 2, 2,  2, 0, 0 }   // [_]	 	   [_][_]
};
const int O_TETROMINO_ARR[3][12] = {
    { 0, 1, 1,  0, 1, 1,  0, 1, 1,  0, 1, 1 },  // [ ][ ]	[ ][ ] 
    { 0, 2, 2,  0, 2, 2,  0, 2, 2,  0, 2, 2 },  // [_][_]	[_][_] 
    { 0, 0, 0,  0, 0, 0,  0, 0, 0,  0, 0, 0 }   //                
};

// ============================================================================
// TETROMINO CLASS 
// ============================================================================

// CONSTRUCTORS DESTRUCTORS AND OPERATORS

tetromino::tetromino(tetromino_type tet_t){

    switch(tet_t){
        case L_TETROMINO:   
            sstride = 3;
            salength = 12;
            setShapeArray(L_TETROMINO_ARR);


            color = ORANGE;
            break;

        case J_TETROMINO: 
            sstride = 3;
            salength = 12;
            setShapeArray(J_TETROMINO_ARR);
            
            color = BLUE;
            break;

        case T_TETROMINO: 
            sstride = 3; 
            salength = 12;
            setShapeArray(T_TETROMINO_ARR);
            
            color = PURPLE;
            break;

        case I_TETROMINO: 
            sstride = 4;
            salength = 16;
            setShapeArray(I_TETROMINO_ARR);

            color = CYAN;
            break;

        case S_TETROMINO: 
            sstride = 3;
            salength = 12;
            setShapeArray(S_TETROMINO_ARR);

            color = GREEN;
            break;

        case Z_TETROMINO: 
            sstride = 3;
            salength = 12;
            setShapeArray(Z_TETROMINO_ARR);

            color = RED;
            break;

        case O_TETROMINO: 
            sstride = 3;
            salength = 12;
            setShapeArray(O_TETROMINO_ARR);
            
            color = YELLOW;
            break;
    }

    current_rotation = 0;
    type = tet_t;

    calcTopandBottomSquare();
    calcLeftandRightSquare();
}

// In the following two functions, the pointer to the array is intentionally
// being set to the same array in memory. That array is unmodifiable and can
// not be deleted. I don't think they're needed, but they I made them just in
// case. 

tetromino::tetromino (const tetromino& tet){
    this->color = tet.color;
    this->sstride = tet.sstride;
    this->salength = tet.salength;
    setShapeArray(tet.shape_arr, tet.sstride, tet.salength);
    this->current_rotation = tet.current_rotation;

    this->topmost_sqr = tet.topmost_sqr;
    this->bottommost_sqr = tet.bottommost_sqr;
    this->leftmost_sqr = tet.leftmost_sqr;
    this->rightmost_sqr = tet.rightmost_sqr;
}

tetromino::~tetromino(){

    for (int i = 0; i < sstride; i++){
        delete[] shape_arr[i];
    }

    delete[] shape_arr;
}

tetromino& tetromino::operator = (const tetromino& rhs){
    this->color = rhs.color;
    this->sstride = rhs.sstride;
    this->salength = rhs.salength;
    setShapeArray(rhs.shape_arr, rhs.sstride, rhs.salength);
    this->current_rotation = rhs.current_rotation;

    this->topmost_sqr = rhs.topmost_sqr;
    this->bottommost_sqr = rhs.bottommost_sqr;
    this->leftmost_sqr = rhs.leftmost_sqr;
    this->rightmost_sqr = rhs.rightmost_sqr;

    return *this;
}

// GETTERS

// const int* tetromino::shapeArray() const {
//     return shape_arr;
// }

// int tetromino::shapeStride() const {
//     return sstride;
// }

// color_id tetromino::color() const {
//     return color;
// }

void tetromino::rotate(direction rotation_dir){

    if (type == O_TETROMINO) // O_TETROMINO doesn't rotate
        return;

    // if user entered an ambiguous direction to rotate, specify which direction to rotate.
    if (rotation_dir == UP || rotation_dir == LEFT) 
        rotation_dir = CCWISE;
    else if (rotation_dir == DOWN || rotation_dir == RIGHT) 
        rotation_dir = CWISE;

    // Assign next rotation offset
    if (rotation_dir == CWISE){
        if (current_rotation == sstride * 3){
            current_rotation = 0;
        } else {
            current_rotation += sstride;
        }
    } else { // if dir == CCWISE
        if (current_rotation == 0){
            current_rotation = sstride * 3;
        } else {
            current_rotation -= sstride;
        }
    }

    // Calculate the new edge squares
    calcTopandBottomSquare();
    calcLeftandRightSquare();
}

// This function is used to index the shape array, but only at the current 
// rotation. So it handles indexes as if the array is only as big as the 
// current shape.
// Input: a y and x value
// Output: the value of the current shape at the given coordinates (0, 1, or 2)
//         -1 if the indices are invalid
int tetromino::shapeAt(int y, int x){

    if (y >= sstride || y < 0 || x >= sstride || x < 0){
        return -1;
    }
        
    return shape_arr[y][x + current_rotation];
    
}
// bool tetromino::canRotate(direction dir){
    
//     if (m_curr_arr_start == O_TETROMINO_ARR){ // O tetromino doesn't rotate
//         return false;
//     }

//     square grid_square_val;     // used to make code more readable in the 
//     int prev_square_val;        // following loops
//     int curr_square_val;        //

//     const int* rotation_1;  // This the rotation after the start of the array,
//     const int* rotation_2;  //    so m_curr_arr_start is like rotation_0.
//     const int* rotation_3;
//     rotation_1 = m_curr_arr_start + (m_curr_shape_stride);
//     rotation_2 = m_curr_arr_start + (m_curr_shape_stride * 2);
//     rotation_3 = m_curr_arr_start + (m_curr_shape_stride * 3);

//     const int* prev_rotation_offset = m_curr_rotation_offset;


//     if (dir == UP || dir == DOWN || dir == LEFT || dir == RIGHT)
//         return false;

//     if (dir == CWISE){
//         if (m_curr_rotation_offset == rotation_3){
//             m_curr_rotation_offset = m_curr_arr_start;
//         } else {
//             m_curr_rotation_offset += m_curr_shape_stride;
//         }
//     } else { // if dir == CCWISE
//         if (m_curr_rotation_offset == m_curr_arr_start){
//             m_curr_rotation_offset = rotation_3;
//         } else {
//             m_curr_rotation_offset -= m_curr_shape_stride;
//         }
//     }


//     // find if there is a collision with the next rotation offset
//     bool collision = false;
//     for (int i = 0; i < m_curr_shape_stride; i++){
//         for (int j = 0; j < m_curr_shape_stride; j++){

//             prev_square_val = *(prev_rotation_offset + j + i * m_curr_shape_arr_len);
//             curr_square_val = *(m_curr_rotation_offset + j + i * m_curr_shape_arr_len);

//             if (!inBounds(tet_y_pos + i, tet_x_pos + j)){
//                 if (curr_square_val != EMPTY_SQR)
//                     collision = true; // The tetromino is colliding with a wall.
//                 continue;
//             }

//             grid_square_val = grid.at(tet_y_pos + i).at(tet_x_pos + j);

//             if (curr_square_val != EMPTY_SQR && prev_square_val == EMPTY_SQR &&   // if the tetromino is 
//                 grid_square_val.s_type != EMPTY_SQR)                              // colliding with
//             {                                                                     // another tetromino
//                 collision = true;
//             }

//             if (collision) break;
//         }
//         if (collision) break;
//     }

//     if (!collision){
//         m_curr_rotation_offset = prev_rotation_offset;
//         return true;
//     }

//     bool can_rotate = true;
//     if (m_curr_arr_start == L_TETROMINO_ARR ||
//         m_curr_arr_start == J_TETROMINO_ARR ||
//         m_curr_arr_start == T_TETROMINO_ARR){
        
//         if (dir == CWISE){
//             if (canShiftLeft(1)){
//                 blindShiftTetromino(prev_rotation_offset,1, LEFT);
//             } else if (canShiftRight(1)){
//                 blindShiftTetromino(prev_rotation_offset,1, RIGHT);
//             } else if (canShiftUp(1)){
//                 blindShiftTetromino(prev_rotation_offset,1, UP);
//             } else {
//                 can_rotate = false;
//             } 

//         } else {// dir == CCWISE

//             if (canShiftLeft(1)){
//                 blindShiftTetromino(prev_rotation_offset,1, LEFT);
//             } else if (canShiftRight(1)){
//                 blindShiftTetromino(prev_rotation_offset,1, RIGHT);
//             } else if (canShiftUp(1)){
//                 blindShiftTetromino(prev_rotation_offset,1, UP);
//             } else {
//                 can_rotate = false;
//             } 
//             // if (canShiftRight(1)){
//             //     blindShiftTetromino(prev_rotation_offset,1, RIGHT);
//             // } else if (canShiftLeft(1)){
//             //     blindShiftTetromino(prev_rotation_offset,1, LEFT);
//             // } else if (canShiftUp(1)){
//             //     blindShiftTetromino(prev_rotation_offset,1, UP);
//             // } else {
//             //     can_rotate = false;
//             // } 
//         }
//
//     } else if (m_curr_arr_start == I_TETROMINO_ARR){

//         if ((m_curr_rotation_offset == m_curr_arr_start ||
//             m_curr_rotation_offset == rotation_2) && dir == CWISE)
//         {
//             if (canShiftLeft(1)){
//                 blindShiftTetromino(prev_rotation_offset,1, LEFT);
//             } else if (canShiftRight(1)){
//                 blindShiftTetromino(prev_rotation_offset,1, RIGHT);
//             } else if (canShiftUp(1)){
//                 blindShiftTetromino(prev_rotation_offset,1, UP);
//             } else {
//                 can_rotate = false;
//             } 
            
//         } else if ((m_curr_rotation_offset == m_curr_arr_start ||
//                    m_curr_rotation_offset == rotation_2) && dir == CCWISE)
//         {
//             if (canShiftRight(1)){
//                 blindShiftTetromino(prev_rotation_offset,1, RIGHT);
//             } else if (canShiftLeft(1)){
//                 blindShiftTetromino(prev_rotation_offset,1, LEFT);
//             } else if (canShiftUp(1)){
//                 blindShiftTetromino(prev_rotation_offset,1, UP);
//             } else {
//                 can_rotate = false;
//             } 

//         } else if ((m_curr_rotation_offset == rotation_1 ||
//                    m_curr_rotation_offset == rotation_3) && dir == CWISE)
//         {
//             if (canShiftRight(1)){
//                 blindShiftTetromino(prev_rotation_offset,1, RIGHT);
//             } else if (canShiftLeft(1)){
//                 blindShiftTetromino(prev_rotation_offset,1, LEFT);
//             } else if (canShiftUp(1)){
//                 blindShiftTetromino(prev_rotation_offset,1, UP);
//             } else {
//                 can_rotate = false;
//             } 

//         } else if ((m_curr_rotation_offset == rotation_1 ||
//                    m_curr_rotation_offset == rotation_3) && dir == CCWISE)
//         {
//             if (canShiftLeft(1)){
//                 blindShiftTetromino(prev_rotation_offset,1, LEFT);
//             } else if (canShiftRight(1)){
//                 blindShiftTetromino(prev_rotation_offset,1, RIGHT);
//             } else if (canShiftUp(1)){
//                 blindShiftTetromino(prev_rotation_offset,1, UP);
//             } else {
//                 can_rotate = false;
//             } 
//         }

//     } else {    // m_curr_arr_start == S_TETROMINO_ARR ||
//                 // m_curr_arr_start == Z_TETROMINO_ARR
//         if (m_curr_rotation_offset == m_curr_arr_start || 
//             m_curr_rotation_offset == rotation_2)
//         {
//             if (canShiftRight(1)){
//                 blindShiftTetromino(prev_rotation_offset,1, RIGHT);
//             } else if (canShiftLeft(1)){
//                 blindShiftTetromino(prev_rotation_offset,1, LEFT);
//             } else if (canShiftUp(1)){
//                 blindShiftTetromino(prev_rotation_offset,1, UP);
//             } else {
//                 can_rotate = false;
//             }

//         } else {// next rotation offset == rotation 1 or 3
//             if (canShiftLeft(1)){
//                 blindShiftTetromino(prev_rotation_offset,1, LEFT);
//             } else if (canShiftRight(1)){
//                 blindShiftTetromino(prev_rotation_offset,1, RIGHT);
//             } else if (canShiftUp(1)){
//                 blindShiftTetromino(prev_rotation_offset,1, UP);
//             } else {
//                 can_rotate = false;
//             }
//         }
//     }

//     m_curr_rotation_offset = prev_rotation_offset;
    
//     return can_rotate;
// }


// PRIVATE ====================================================================

// The next two functions are called after the shape array of the tetromino is
// modified. Its purpose is to make sure that the tetromino's topmost,
// bottommost, leftmost, and rightmost variables are up to date.
void tetromino::calcTopandBottomSquare(){
    // Since the shape array is not 2D, to step through it like it is 2D, there
    // needs to be a multiplier that will get you to the next "row". This case
    // is more complicated because i only want to read a quarter of the array
    // like its 2D, and because I'm working with pointer and not the index
    // operator.
    //   To read from only a section of the array like it's 2D, there needs to
    // be an offset that, once you've made it to the new "row", can be used
    // to put you back on the correct "column".

    // Hit is true when a 1 is found in the part of the array that is being searched.
    bool hit = false;
    for (int row = 0; row < sstride; row++){
        for (int col = 0; col < sstride; col++){
            if (shape_arr[row][current_rotation + col] > 0){
                topmost_sqr = row;
                hit = true;
                break;
            }  
        }
        if (hit) break;
    }

    hit = false;
    for (int row = sstride - 1; row >= 0; row--){
        for (int col = 0; col < sstride; col++){
            if (shape_arr[row][current_rotation + col] > 0){
                bottommost_sqr = row;
                hit = true;
                break;
            }  
        }
        if (hit) break;
    }
}

void tetromino::calcLeftandRightSquare(){

    bool hit = false;
    for (int col = 0; col < sstride; col++){
        for (int row = 0; row < sstride; row++){
            if (shape_arr[row][current_rotation + col] > 0){
                leftmost_sqr = col;
                hit = true;
                break;
            }  
        }
        if (hit) break;
    }

    hit = false;
    for (int col = sstride - 1; col >= 0; col--){
        for (int row = 0; row < sstride; row++){
            if (shape_arr[row][current_rotation + col] > 0){
                rightmost_sqr = col;
                hit = true;
                break;
            }  
        }
        if (hit) break;
    }
}

template <int rows, int cols>
void tetromino::setShapeArray(const int (&tet_array)[rows][cols]){

    shape_arr = new int*[rows];
    for (int i = 0; i < rows; i++){
        shape_arr[i] = new int[cols];

        for (int j = 0; j < cols; j++){
            shape_arr[i][j] = tet_array[i][j];
        }        
    }
}

void tetromino::setShapeArray(int** tet_array, int rows, int cols){

    shape_arr = new int*[rows];
    for (int i = 0; i < rows; i++){
        shape_arr[i] = new int[cols];

        for (int j = 0; j < cols; j++){
            shape_arr[i][j] = tet_array[i][j];
        }        
    }
}
