/*
	You can modify this file for your testing purpose 
	but note that we will use our own copies of this file for testing during grading 
	so changes in this file will be overwritten
*/

#include "matrix.h" // do not modify this file

#define PRINT_MATRIX 0

#define CREATE_MATRIX 1
#define ADD_ROW 2
#define ADD_COL 3
#define INCREMENT 4
#define SCALAR_MULTIPLY 5
#define SCALAR_DIVIDE 6
#define SCALAR_POWER 7
#define DELETE_MATRIX 8

void my_run();
void read_numbers(int *buffer, int num_items_to_read); // implemented for you
