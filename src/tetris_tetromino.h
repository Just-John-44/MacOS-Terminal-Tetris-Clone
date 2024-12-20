#ifndef TETRIS_TETROMINO_H
#define TETRIS_TETROMINO_H

// John Wesley Thompson
// Created: 8/10/2024
// Completed:
// Last Edited: 10/17/2024
// tetris_tetromino.h

// DATA =======================================================================

// The following tetromino rotations follow the NES tetris rules.
extern const int L_TETROMINO_ARR[36];
extern const int J_TETROMINO_ARR[36];
extern const int T_TETROMINO_ARR[36];
extern const int I_TETROMINO_ARR[64]; 
extern const int S_TETROMINO_ARR[36];
extern const int Z_TETROMINO_ARR[36];
extern const int O_TETROMINO_ARR[36];

enum tetromino_type {
    L_TETROMINO = 1,
    J_TETROMINO,
    T_TETROMINO,
    I_TETROMINO,
    S_TETROMINO,
    Z_TETROMINO,
    O_TETROMINO
};

#ifndef ENUM_COLOR_ID
#define ENUM_COLOR_ID
enum color_id{
    STANDARD = 1,
    ORANGE,
    BLUE,
    PURPLE,
    CYAN,
    GREEN,
    RED,
    YELLOW
};
#endif

const int POSSIBLE_ROTATIONS = 4; // This doesn't apply to the O_TETROMINO

// TETROMINO CLASS ================================================================

class tetromino {

public:

    // Constructors
    tetromino();
    tetromino(tetromino_type);
    tetromino(const tetromino &);
    
    // Operators
    tetromino& operator = (const tetromino&);

    // Accessors
    const int* shapeArray() const;
    int shapeStride() const;
    color_id color() const;

    // Functions
    void calcTopandBottomSquare(const int*, int&, int&);
    void calcLeftandRightSquare(const int*, int&, int&);

    const int* m_shape_arr_ptr; // the tetromino's shape array

private:

    color_id m_color_id;
    int m_shape_stride; // the distance from one rotation to another in the tetromino shape array
    
};



#endif