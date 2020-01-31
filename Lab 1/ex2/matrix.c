#include <stdio.h>
#include <stdlib.h> // includes malloc(), free(), realloc()
#include <string.h> // includes memcpy()

#include "matrix.h" // do not modify this file

// put your function prototypes for additional helper functions below:


// implementation
matrix *create_matrx(int num_rows, int num_cols)
{   
    // TODO
    return NULL;
}

void add_row(matrix *mat, int *row)
{
    // TODO
}

void add_col(matrix *mat, int *col)
{
    // TODO
}

void increment(matrix *mat, int num)
{
    // TODO
}

void scalar_multiply(matrix *mat, int num)
{
    // TODO
}

void scalar_divide(matrix *mat, int num)
{
    // TODO
}

void scalar_power(matrix *mat, int num)
{
    // TODO
}

void delete_matrix(matrix *mat)
{
    // TODO
}

void element_wise_op(matrix *mat, int num, void *op_tr(int *, int))
{
    // TODO
}

/*
    DO NOT MODIFY BELOW
*/
// print out matrix in row-major order
// elements in the same row are space-separated
// each row starts in a new line
void print_matrix(matrix* mat) 
{
    int row_idx, col_idx;
    for(row_idx = 0; row_idx < mat->num_rows; ++row_idx) {
        for(col_idx = 0; col_idx < mat->num_cols; ++col_idx) {
            if (col_idx == mat->num_cols-1) {
                printf("%d\n", mat->data[row_idx][col_idx]);
            } else {
                printf("%d ", mat->data[row_idx][col_idx]);
            }
        }
    }
}
