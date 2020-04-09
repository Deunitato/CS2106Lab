/*************************************
 ** Lab 4 Exercise 5
 **************************************
 * You may change this file during your own testing, 
 * but note that they will be replaced with the original 
 * files when we test your assignments.
 **************************************/

#include <stdio.h>
#include <string.h>
#include "mmalloc.h"
#include "pthread.h"
#include "stdlib.h"

//Buffer Size and Operation Count
const int ALLOC_SZ = 50;
const int NUM_OPERATIONS = 1000;

//Size of heap and number of threads
int heapSize = (1 << 30);
int numThreads = 16;

//stores addresses that have been allocated
int** allocated;

//for error message
const char* MEMORY_ERROR = "Failed to allocate memory\n";
const char* THREAD_ERROR = "Error creating thread\n";

void *run_operation(void* x);
void err(const char *msg); 
void allocationMsg(int threadId, int sz);
void freeMsg(int threadId, int* addr); 

int main()
{
    //create buffer
    allocated = malloc(sizeof(int*) * ALLOC_SZ * numThreads); 
    if (allocated == NULL) {
        err(MEMORY_ERROR);    
    }
   
	if(!setupHeap(heapSize)){
		return 0;
	}

    //create and start our threads
    pthread_t workers[numThreads];
    for (int i = 0; i < numThreads; i++) {
        int *x = malloc(sizeof(int));
        if (x == NULL) {
            err(MEMORY_ERROR);
        }

        *x = i;
        int ret = pthread_create(&workers[i], NULL, run_operation, x);
        if (ret) {
            err(THREAD_ERROR);
        }
    }

    //wait for threads to return
    for (int i = 0; i < numThreads; i++) {
        pthread_join(workers[i], NULL);
    }

    //final heap
    printHeapMetaInfo();
    printHeapStatistics();
    
    //cleanup
    free(allocated);
    heapCleanUp();

	return 0;
}

/*
 * Each thread will 
 * (1) Allocate a bunch of memory - ALLOC_SZ times
 * (2) Either free or reallocate memory - NUM_OPERATION times
 */
void *run_operation(void* x) 
{
    int threadId = *((int*)x);  
    free(x);

    //calculate max amount to allocate
    int rand_size = (1.0*heapSize / numThreads) * (1.0 / ALLOC_SZ) * (1.0 / sizeof(int));

    int start = 0 + threadId * ALLOC_SZ;

    //allocating initial buffer
    for (int i = start; i < start + ALLOC_SZ; i++) {
        int sz = (rand() % rand_size + 1) * sizeof(int);
        allocationMsg(threadId, sz);       
        allocated[i]  = (int*) mymalloc(sz);

#ifdef DEBUG
        printHeapMetaInfo();
#endif
    }

    //fill or clear our buffer
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        int j = rand() % ALLOC_SZ + start;
 
        //if space is empty 
        if (allocated[j] == NULL) {
            int sz = (rand() % rand_size + 1) * sizeof(int);
            allocationMsg(threadId, sz);
            allocated[j] = (int*) mymalloc(sz);
        }
        //there is already allocation, so free!
        else if (allocated[j] != NULL) {
            freeMsg(threadId, allocated[j]); 
            myfree(allocated[j]);
            allocated[j] = NULL;
        }
 
#ifdef DEBUG
        printHeapMetaInfo();
#endif
    }

    return NULL;
}

/* Printing / Error Handling */

void err(const char *msg) 
{
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}

void allocationMsg(int threadId, int sz) 
{
    printf("Thread [%d] : Allocating %d bytes\n", threadId, sz);
}

void freeMsg(int threadId, int* addr) 
{
    printf("Thread [%d] : Freeing Memory at address, %p\n", threadId, addr);
}

