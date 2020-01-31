#include <stdio.h>
#include <stdlib.h> // includes malloc(), free(), realloc()

#include "ex3.h" // do not modify this file

// put your function prototypes for additional helper functions below:


// parse input stream and perform corresponding matrix operation on input mat
// assume input stream is fully correct
void my_run() 
{
    // TODO
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
