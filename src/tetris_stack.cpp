// Author: John Wesley Thompson
// Creation Date: 9/14/2024
// Completed:
// Last Edited: 9/14/2024
// tetris_stack.cpp


#include "tetris_stack.h"


// SQUARE =====================================================================

square::square(){
    s_color = STANDARD;
    s_type = EMPTY_SQR;
}

// square& square::operator = (const square& rhs){
//     s_color = rhs.s_color;
//     s_type = rhs.s_type;

//     return *this;
// }

void square::setEmpty(){
    s_color = STANDARD;
    s_type = EMPTY_SQR;
}


// STACK_ROW ==================================================================

// stack_row::stack_row(int row_len){
//     sr_row_data.resize(row_len);
//     sr_row_len = row_len;
// }

// bool stack_row::full(){
//     for (int i = 0; i < sr_row_len; i++){
//         if (sr_row_data.at(i).s_type == EMPTY_SQR){
//             return false;
//         }
//     }
//     return true;
// }

// void stack_row::wipe(){
//     for (int i = 0; i < sr_row_len; i++){
//         sr_row_data.at(i).setEmpty();
//     }
// }

// square& stack_row::squareAt(int idx){
//     return sr_row_data.at(idx);
// }

// void stack_row::operator == (stack_row& rhs_row){
//     for (int i = 0; i < sr_row_len; i++){
//         sr_row_data.at(i) = rhs_row.sr_row_data.at(i);
//     }
// }