#include <stdio.h>
#include <stdlib.h> // includes malloc(), free(), realloc()
#include <string.h> // includes memcpy()

#include "matrix.h" // do not modify this file

// put your function prototypes for additional helper functions below:

int power(int num, int exp);
void increment_single(int *x, int y);
void power_single(int *x, int y);

// implementation
matrix *create_matrix(int num_rows, int num_cols)
{   
    int **arr = (int**) malloc(num_rows * sizeof(int*));
    for (int i = 0; i < num_rows; ++i) {
        arr[i] = (int*) malloc(num_cols * sizeof(int));
        for (int j = 0; j < num_cols; ++j) {
            arr[i][j] = 0;
        }
    }
    matrix *m = (matrix*) malloc(sizeof(matrix));
    m->data = arr;
    m->num_rows = num_rows;
    m->num_cols = num_cols;
    return m;
}

void add_row(matrix *mat, int *row)
{
    int num_rows = mat->num_rows;
    int num_cols = mat->num_cols;
    int **arr = (int**) malloc((num_rows + 1) * sizeof(int*));
    for (int i = 0; i < num_rows; ++i) {
        arr[i] = (int*) malloc(num_cols * sizeof(int));
        for (int j = 0; j < num_cols; ++j) {
            arr[i][j] = mat->data[i][j];
        }
    }
    arr[num_rows] = (int*) malloc(num_cols * sizeof(int));
    for (int j = 0; j < num_cols; ++j) {
        arr[num_rows][j] = row[j];
    }
    for (int i = 0; i < num_rows; ++i) {
        free(mat->data[i]);
    }
    free(mat->data);
    mat->data = arr;
    mat->num_rows = num_rows + 1;
}

void add_col(matrix *mat, int *col)
{
    int num_rows = mat->num_rows;
    int num_cols = mat->num_cols;
    int **arr = (int**) malloc(num_rows * sizeof(int*));
    for (int i = 0; i < num_rows; ++i) {
        arr[i] = (int*) malloc((num_cols + 1) * sizeof(int));
        for (int j = 0; j < num_cols; ++j) {
            arr[i][j] = mat->data[i][j];
        }
        arr[i][num_cols] = col[i];
    }
    for (int i = 0; i < num_rows; ++i) {
        free(mat->data[i]);
    }
    free(mat->data);
    mat->data = arr;
    mat->num_cols = num_cols + 1;
}

void increment(matrix *mat, int num)
{
    int num_rows = mat->num_rows;
    int num_cols = mat->num_cols;
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            mat->data[i][j] = mat->data[i][j] + num;
        }
    }
}

void scalar_multiply(matrix *mat, int num)
{
    int num_rows = mat->num_rows;
    int num_cols = mat->num_cols;
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            mat->data[i][j] = mat->data[i][j] * num;
        }
    }
}

void scalar_divide(matrix *mat, int num)
{
    int num_rows = mat->num_rows;
    int num_cols = mat->num_cols;
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            mat->data[i][j] = mat->data[i][j] / num;
        }
    }
}

int power(int num, int exp) {
    int sum = num;
    for (int i = 1; i < exp; ++i) {
        sum *= num;
    }
    return sum;
}

void scalar_power(matrix *mat, int num)
{
    int num_rows = mat->num_rows;
    int num_cols = mat->num_cols;
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            mat->data[i][j] = power(mat->data[i][j], num);
        }
    }
}

void delete_matrix(matrix *mat)
{
    for (int i = 0; i < mat->num_rows; ++i) {
        free(mat->data[i]);
    }
    free(mat->data);
    free(mat);
}

void element_wise_op(matrix *mat, int num, void (*op_ptr)(int *, int))
{
    for (int i = 0; i < mat->num_rows; ++i) {
        for (int j = 0; j < mat->num_cols; ++j) {
            (*op_ptr)(&(mat->data[i][j]), num);
        }
    }
}

void increment_single(int *x, int y)
{
    (*x) += y;
}

void power_single(int *x, int y)
{
    (*x) = power(*x, y);
}

/*
    DO NOT MODIFY BELOW
*/
// print out matrix in row-major order
// elements in the same row are space-separated
// each row starts in a new line
void print_matrix(matrix *mat) 
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
