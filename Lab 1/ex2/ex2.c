/*
	You can modify this file for your testing purpose 
	but note that we will use our own copies of this file for testing during grading 
	so changes in this file will be overwritten
*/

#include <stdio.h>
#include <stdlib.h> // includes malloc(), free(), realloc()

#include "ex2.h" // do not modify this file

// parse input stream and perform corresponding matrix operation on input mat
// assume input stream is fully correct
void increment_single(int *x, int y)
{
(*x) += y;
}
void run() 
{
	while(1) {
        matrix *mat;
		int instruction;
        int *parameters;

        if (scanf("%d", &instruction) <= 0) break;

        switch(instruction) {
            case PRINT_MATRIX:
                print_matrix(mat);
                break;
            case CREATE_MATRIX:
                parameters = (int *) malloc(sizeof(int) * 2);
                read_numbers(parameters, 2);
                mat = create_matrix(parameters[0], parameters[1]);
                free(parameters);
                break;
            case ADD_ROW:
                parameters = (int *) malloc(sizeof(int) * mat->num_cols);
                read_numbers(parameters, mat->num_cols);
                add_row(mat, parameters);
                free(parameters);
                break;
            case ADD_COL:
                parameters = (int *) malloc(sizeof(int) * mat->num_rows);
                read_numbers(parameters, mat->num_rows);
                add_col(mat, parameters);
                free(parameters);
                break;
            case INCREMENT:
                parameters = (int *) malloc(sizeof(int));
                read_numbers(parameters, 1);
               // increment(mat, parameters[0]);
               void (*sendHelp)(int*, int) = increment_single;
               element_wise_op(mat,parameters[0],sendHelp);
                free(parameters);
                break;
            case SCALAR_MULTIPLY:
                parameters = (int *) malloc(sizeof(int));
                read_numbers(parameters, 1);
                scalar_multiply(mat, parameters[0]);
                free(parameters);
                break;
            case SCALAR_DIVIDE:
                parameters = (int *) malloc(sizeof(int));
                read_numbers(parameters, 1);
                scalar_divide(mat, parameters[0]);
                free(parameters);
                break;
            case SCALAR_POWER:
                parameters = (int *) malloc(sizeof(int));
                read_numbers(parameters, 1);
                scalar_power(mat, parameters[0]);
                free(parameters);
                break;
            case DELETE_MATRIX:
                delete_matrix(mat);
                break;
            default:
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
