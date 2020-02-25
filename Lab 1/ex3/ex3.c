#include <stdio.h>
#include <stdlib.h> // includes malloc(), free(), realloc()

#include "ex3.h" // do not modify this file

// put your function prototypes for additional helper functions below:


// parse input stream and perform corresponding matrix operation on input mat
// assume input stream is fully correct
/*
Refernces:
#define PRINT_MATRIX 0

#define CREATE_MATRIX 1
#define ADD_ROW 2
#define ADD_COL 3
#define INCREMENT 4
#define SCALAR_MULTIPLY 5
#define SCALAR_DIVIDE 6
#define SCALAR_POWER 7
#define DELETE_MATRIX 8


matrix *create_matrix(int num_rows, int num_cols);
void add_row(matrix *mat, int *row);
void add_col(matrix *mat, int *col);
void increment(matrix *mat, int num);
void scalar_multiply(matrix *mat, int num);
void scalar_divide(matrix *mat, int num);
void scalar_power(matrix *mat, int num);
void delete_matrix(matrix *mat);
void element_wise_op(matrix *mat, int num, void (*op_ptr)(int *, int));
void print_matrix(matrix *mat); // implemented for you


*/

matrix *wrapper_addRow(matrix *mat){
    
    int *parameters = (int *) malloc(sizeof(int)*(mat->num_cols));
    read_numbers(parameters, mat->num_cols);
    add_row(mat, parameters);
    free(parameters);
    return mat;
}


matrix *wrapper_addCol(matrix *mat){
    int *parameters = (int *) malloc(sizeof(int)*(mat->num_rows));
    read_numbers(parameters, mat->num_rows);
    add_col(mat, parameters);
    free(parameters);
        return mat;
}

matrix *wrapper_MakeMat(matrix *mat){
    int *parameters = (int *) malloc(sizeof(int)*2);
    read_numbers(parameters, 2);
    mat = create_matrix(parameters[0], parameters[1]);
    free(parameters);
        return mat;
}
matrix *wrapper_print(matrix *mat){
        print_matrix(mat);
        return mat;
}

matrix *wrapper_Increment(matrix *mat){
    int *parameters = (int *) malloc(sizeof(int));
    read_numbers(parameters, 1);
    increment(mat, parameters[0]);
    free(parameters);

    return mat;
}

matrix *wrapper_Mut(matrix *mat){
    int *parameters = (int *) malloc(sizeof(int));
    read_numbers(parameters, 1);
    scalar_multiply(mat, parameters[0]);
    free(parameters);
     return mat;
}

matrix *wrapper_Div(matrix *mat){
        int *parameters = (int *) malloc(sizeof(int));
        read_numbers(parameters, 1);
        scalar_divide(mat, parameters[0]);
        free(parameters);
        return mat;

}

matrix *wrapper_pw(matrix *mat){
        int *parameters = (int *) malloc(sizeof(int));
        read_numbers(parameters, 1);
        scalar_power(mat, parameters[0]);
        free(parameters);
        return mat;
}

matrix *wrapper_del(matrix *mat){
       delete_matrix(mat);
       matrix *emptyMat = create_matrix(1, 1);
       return emptyMat;
}



/*
ref
Refernces:
#define PRINT_MATRIX 0

#define CREATE_MATRIX 1
#define ADD_ROW 2
#define ADD_COL 3
#define INCREMENT 4
#define SCALAR_MULTIPLY 5
#define SCALAR_DIVIDE 6
#define SCALAR_POWER 7
#define DELETE_MATRIX 8


*/

void my_run() 
{
    // TODO
    matrix *(*sendHelp0)(matrix*) = wrapper_print;
    matrix *(*sendHelp1)(matrix*) = wrapper_MakeMat;
    matrix *(*sendHelp2)(matrix*) = wrapper_addRow;
    matrix *(*sendHelp3)(matrix*) = wrapper_addCol;
    matrix *(*sendHelp4)(matrix*) = wrapper_Increment;
    matrix *(*sendHelp5)(matrix*) = wrapper_Mut;
    matrix *(*sendHelp6)(matrix*) = wrapper_Div;
    matrix *(*sendHelp7)(matrix*) = wrapper_pw;
    matrix *(*sendHelp8)(matrix*) = wrapper_del;

    matrix *(*ope[9])(matrix *mat) = {sendHelp0,sendHelp1,sendHelp2,sendHelp3,
                                      sendHelp4,sendHelp5, sendHelp6,sendHelp7,
                                      sendHelp8};

   while(1) {
      matrix *mat;
	  int instruction;
      if (scanf("%d", &instruction) <= 0) break;
      if(instruction < 9 && instruction >=0){
           mat = (*ope[instruction])(mat);
      }
      else{
        fprintf(stderr, "Invalid instruction\nTerminating...\n");
        exit(1);
      }
   }
}

// read n numbers into arr
void read_numbers(int* arr, int n) 
{
	int idx;
    for (idx = 0; idx < n; ++idx) {
        if (scanf("%d", arr + idx) <= 0) {
            fprintf(stderr, "Failed to read numbers\n");
            break;
        }
    }
}
