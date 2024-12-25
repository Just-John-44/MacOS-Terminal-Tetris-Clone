#ifndef TETRIS_STACK_H
#define TETRIS_STACK_H


// Author: John Wesley Thompson
// Creation Date: 9/14/2024
// Completed:
// Last Edited: 10/6/2024
// tetris_stack.h


#include <vector>
#include "tetris_types.h"


// SQUARE =====================================================================

struct square {
    color_id s_color;  
    square_type s_type;

    square();
    void setEmpty();
    // square& operator = (const square&);
};

// STACK_ROW ==================================================================

class stack_row {
public:

    stack_row(int = 0);
    bool full();
    void wipe();
    // square& squareAt(int);
    
    void operator == (stack_row&);
    std::vector<square> sr_row_data;

private:

    int sr_row_len;
};

#endif
