// John Wesley Thompson
// Created: 8/10/2024
// Last Edited: 12/30/2024
// tetrose_tetromino.cpp


#include "tetrose_tetromino.h"


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
            
            color = L_BLUE;
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
            
            color = L_YELLOW;
            break;
    }

    current_rotation = 0;
    type = tet_t;

    calcEdges();
    
}

// In the following two functions, the pointer to the array is intentionally
// being set to the same array in memory. That array is unmodifiable and can
// not be deleted. I don't think they're needed, but I made them just in
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

void tetromino::rotate(direction rotation_dir){

    if (type == O_TETROMINO) // O_TETROMINO doesn't rotate
        return;

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
    calcEdges();
    
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

// resetRotation sets the curr_rotation variable to the initial rotation for 
// the tetromino and calculates the edges at that rotation.
void tetromino::resetRotation(){

    current_rotation = 0;
    calcEdges();
}

// PRIVATE ====================================================================

// calcEdges assigns the correct value to the topmost bottommost, leftmost, and 
// rightmost square values. This is usually used after a rotation.
void tetromino::calcEdges(){

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

    hit = false;
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

//
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

//
void tetromino::setShapeArray(int** tet_array, int rows, int cols){

    shape_arr = new int*[rows];
    for (int i = 0; i < rows; i++){
        shape_arr[i] = new int[cols];

        for (int j = 0; j < cols; j++){
            shape_arr[i][j] = tet_array[i][j];
        }        
    }
}
