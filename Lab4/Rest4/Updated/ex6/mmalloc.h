/*************************************
 ** Lab 4 Exercise 6 
 **************************************
 * You may change this file during your own testing, 
 * but note that they will be replaced with the original 
 * files when we test your assignments.
 **************************************/

#ifndef __MMALLOC_H_	//standard protection against multiple includes
#define __MMALLOC_H_

#include <unistd.h>	//for brk, sbrk

#define FREE 0
#define OCCUPIED 1

typedef struct bm {
	int	size;
	struct bm *nextPart;
	int status;
} partMetaInfo;

typedef struct {
	partMetaInfo* base;
	int totalSize;
	int partMetaSize;
} heapMetaInfo;


int setupHeap(int);
void printHeapMetaInfo();
void printHeapStatistics();
void heapCleanUp();

void* mymalloc(int);
void myfree(void*);

#endif
