#include <stdio.h>
#include <stdlib.h> // includes malloc(), free(), realloc()
#include <string.h> // includes memcpy()

#include "matrix.h" // do not modify this file

// put your function prototypes for additional helper functions below:
int power(int, int);


// implementation
matrix *create_matrix(int num_rows, int num_cols)
{   
    matrix *newMat;
    int** myData;
    //newMat = (matrix*)malloc(sizeof(int) * num_cols *num_cols);
    newMat = (matrix*)malloc(sizeof(matrix));
    //set values
    newMat->num_cols = num_cols;
    newMat -> num_rows = num_rows;

    // Allocate the columns
    myData = (int**)malloc(num_cols * sizeof(int*));

    //init to 0
    for(int i=0; i< num_rows ; i++){
        myData[i] = (int*)malloc(sizeof(int));
        myData[i] = 0;
    }

    //set it to matrix
    newMat->data = myData;
    // TODO
    return newMat;
}

void add_row(matrix *mat, int *row)
{
    // TODO
    //add more rows to current matrix
    int** myData = mat->data;
    mat->num_rows = (mat->num_rows)++; //increase
    int numberRows = mat->num_rows;
    //add space and realloc it
    myData[numberRows] = (int*)realloc(row, (mat->num_cols)*sizeof(int));
    /*for (int i = 0; i < (mat->num_cols); i++){
       myData[numberRows][i] = (int*)realloc((int)row[i], numberRows*sizeof(int));
    }*/
}

void add_col(matrix *mat, int *col)
{
    // TODO
     int** myData = mat->data;
     mat->num_cols = (mat->num_cols)++; //increase
     int numberCol = mat->num_cols;
     myData = (int**)realloc(col, (mat->num_rows)*sizeof(int*));

    /*for (int i = 0; i < (mat->num_rows); i++){
       myData[i][numberCol] = (int*)realloc((int)col[i], numberCol*sizeof(int));
    }*/


    
}

void increment(matrix *mat, int num)
{
    // TODO
    int** myData = mat->data;
    for(int i = 0; i < (mat->num_rows) ; i++){
        for(int j =0 ; j < (mat-> num_cols) ; j++){
            myData[i][j] = myData[i][j] + num;
        }
    }
}

void scalar_multiply(matrix *mat, int num)
{
    // TODO
    int** myData = mat->data;
    for(int i = 0; i < (mat->num_rows) ; i++){
        for(int j =0 ; j < (mat-> num_cols) ; j++){
            myData[i][j] = myData[i][j] * num;
        }
    }
}

void scalar_divide(matrix *mat, int num)
{
    // TODO
    int** myData = mat->data;
    for(int i = 0; i < (mat->num_rows) ; i++){
        for(int j =0 ; j < (mat-> num_cols) ; j++){
            myData[i][j] = myData[i][j] / num;
        }
    }
}

void scalar_power(matrix *mat, int num)
{
    // TODO
     int** myData = mat->data;
    for(int i = 0; i < (mat->num_rows) ; i++){
        for(int j =0 ; j < (mat-> num_cols) ; j++){
            myData[i][j] = power(myData[i][j],num);
        }
    }
}

int power(int pow, int num){
    int ans = num;
    if(pow == 0){
        return 1;
    }

    for(int i  =1 ; i < pow ; i++ ){
        ans = ans * ans;
    }
    return ans;
    
}

void delete_matrix(matrix *mat)
{
    // TODO
    int** myData = mat->data;
   /* for(int i = 0; i < (mat->num_rows) ; i++){
        for(int j =0 ; j < (mat-> num_cols) ; j++){
            free(myData[i][j]);
    }*/

    int m = mat->num_cols;
    int n = mat->num_rows;
    for(int i = 0 ; i < (n+m) ; ++i){
       free(myData[i]);
    }

// Deallocate the columns
       free(myData);
       free(mat);
    
}
/*
void element_wise_op(matrix *mat, int num, void *op_tr(int *, int))
{
    // TODO
}*/

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
