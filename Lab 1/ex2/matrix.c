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
    //newMat = (matrix*)malloc(sizeof(int) * num_cols *num_cols);
    newMat = (matrix*)malloc(sizeof(matrix));
    //set values
    newMat->num_cols = num_cols;
    newMat -> num_rows = num_rows;

    // Allocate the row
     int **myData=(int**)malloc(num_rows*sizeof(int*));

    //allocate cool
    for(int i = 0; i<num_rows ; i++){
      myData[i] = (int*)malloc(sizeof(int) * num_cols);
    }

    //reassing
    for(int i=0; i< num_rows ; i++){
        for(int j =0; j <num_cols ; j++){
           // myData[i][j] = (int*)malloc(sizeof(int));
            myData[i][j] = 0;
        }
    }
    //set it to matrix
    newMat->data = myData;
    // TODO
    return newMat;
}

void add_row(matrix *mat, int *row)
{
    // TODO
	// TODO
    //add more rows to current matrix
     int** newData = (int**)malloc((mat->num_rows +1)*sizeof(int*));
     int** arr = mat->data;
     int num_rows = mat->num_rows;
     int num_cols = mat->num_cols;

    for(int i =0 ; i <=num_rows ; i++){
        newData[i] = (int*)malloc((mat->num_cols)*sizeof(int));
        for(int j = 0; j < num_cols ; j++){
            if(i==num_rows){
                newData[i][j] = row[j];
            }
            else{
                newData[i][j] = arr[i][j];
            }
        }
    }
    //add the row
    // newData[num_rows-1] = (int*)realloc(*row, sizeof(row));

     //throw away the old
        for(int i = 0 ; i < num_rows ; ++i){
           free(mat->data[i]);
        }
        // Deallocate the columns
       free(mat->data);
       mat->data = newData;
       mat->num_rows = num_rows + 1; //increase
}

void add_col(matrix *mat, int *col)
{
    // TODO
	int** arr = mat->data;
     int num_rows = mat->num_rows;
     int num_cols = mat->num_cols;
     int** newData = (int**)malloc((mat->num_rows)*sizeof(int*));;

     for(int i =0 ; i < num_rows ; i ++){
         newData[i] = (int*)malloc((mat->num_cols + 1)*sizeof(int));
         for(int j =0 ; j <num_cols ; j ++){
             newData[i][j] = arr[i][j];
         }
         newData[i][num_cols] = col[i];
     }

      for(int i = 0 ; i < num_rows-1 ; ++i){
           free(mat->data[i]);
        }
        // Deallocate the columns
       free(mat->data);
       mat->data = newData;
       mat->num_cols = num_cols + 1; //increase
}

void increment(matrix *mat, int num)
{
    // TODO
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
            myData[i][j] = power(num, myData[i][j]);
        }
    }
}

int power(int pow, int num){
    int ans = num;
    if(pow == 0){
        return 1;
    }

    for(int i  =1 ; i < pow ; i++ ){
        ans = ans * num;
    }
    return ans;
    
}	

void delete_matrix(matrix *mat)
{
    // TODO
	
    int rows = mat->num_rows;
    for(int i = 0 ; i < rows ; i++){
       free(mat->data[i]);
    }

// Deallocate the columns
       free(mat->data);
       free(mat);
    
}

void element_wise_op(matrix *mat, int num, void (*op_ptr)(int *, int))
{
    // TODO
	    int** myData = mat->data;
    for(int i = 0; i < (mat->num_rows) ; i++){
        for(int j =0 ; j < (mat-> num_cols) ; j++){
             (op_ptr)(&(myData[i][j]), num);
        }
    }
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
