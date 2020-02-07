/*
	You can modify this file for your testing purpose 
	but note that we will use our own copies of this file for testing during grading 
	so changes in this file will be overwritten
*/

typedef struct {
	int **data;
	int num_rows;
	int num_cols;
} matrix;

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
