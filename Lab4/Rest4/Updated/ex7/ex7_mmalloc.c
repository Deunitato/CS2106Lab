/*************************************
* Lab 4 Exercise 7
* Name:
* Student No:
* Lab Group:
*************************************/

#include <stdlib.h>
#include <stdio.h>
#include "mmalloc.h"
#include "string.h"

//This is a "global" structure storing heap information
//Only visible to functions in this file
static heapMetaInfo hmi;

void compact();

//meta data for our nodes
void initializeMetaPartAt(partMetaInfo* bmiPtr, int size)
{
	bmiPtr->size = size;
	bmiPtr->nextPart = NULL;
	bmiPtr->status = FREE;
}

//extend program memory and setup heap
int setupHeap(int initialSize)
{
	void* base;
	base = sbrk(0);
	if(sbrk(initialSize) == (void*) - 1 ) {
		printf("Cannot set break! Behavior undefined!\n");
		return 0;
	}

	hmi.totalSize = initialSize;
	hmi.base = (partMetaInfo*) base;
	hmi.partMetaSize = sizeof(partMetaInfo);
	
	initializeMetaPartAt(hmi.base, initialSize - hmi.partMetaSize);
	return 1;
}

//print information about each node
void printMetaPartList(partMetaInfo* bmiPtr)
{
	partMetaInfo* current = bmiPtr;
	for ( current = bmiPtr; current != NULL; 
		  current = current->nextPart) {
        printf("[+%5d | %5d bytes | %d]\n", 
				(int)((void*)current - (void*)hmi.base), 
                current->size, current->status);
	}
}

//prints some statistics about our heap
void printHeapStatistics()
{
    //TODO: Copy from ex2 if you want 
}

//print the whole heap
void printHeapMetaInfo()
{
	printf("Heap Meta Info:\n");
	printf("===============\n");
	printf("Total Size = %d bytes\n", hmi.totalSize);
	printf("Start Address = %p\n", hmi.base);
	printf("Partition Meta Size = %d bytes\n", hmi.partMetaSize);
	printf("Partition list:\n");

	printMetaPartList((partMetaInfo*) hmi.base);
	printf("\n");
}

//break out smaller chunk
void splitPart(partMetaInfo *bigPart, int size)
{
	partMetaInfo *holeAt;
	int holeSize;

	//Remember: When calculating address offset, use a pointer of
	//(void*) type (or char*), so that the calculation is in bytes.
	holeAt = (void*)bigPart + hmi.partMetaSize + size;
	holeSize = bigPart->size - hmi.partMetaSize - size;

	//Make a new partition for the hole
	initializeMetaPartAt(holeAt, holeSize);
	holeAt->nextPart = bigPart->nextPart;
	bigPart->nextPart = holeAt;
    bigPart->size = size;
}

void mymalloc(void** ref, int size)
{
    partMetaInfo *current = NULL;

    // We need to make sure the size is word
    // aligned, i.e. if the word size is 4 bytes, the size need to be
    // rounded to nearest multiples of 4. Otherwise, user can get "bus
    // error" when accessing non-aligned memory locations

    // divide by 4 then multiply by 4 gives rounded multiples of 4. 
    // addition  of 4 round up to the next multiple 
    // subtraction take care of the case where size is already multiples
    // of 4
    size = (size - 1) / 4 * 4 + 4;
    
    //TODO: Copy your solution from previous exercise here 
    
    if (current == NULL) { //heap full
        return;
    }

	//Can we split the part? 
	//The new "hole" should >= 4 bytes after placing in a new part
	//meta info structure 
	if (current->size >= size + hmi.partMetaSize + 4 ){
		splitPart(current, size);
	}

	current->status = OCCUPIED;
	*ref = (void*)current + hmi.partMetaSize;
}

void myfree(void* address)
{
	partMetaInfo *toBeFreed;

	//For a robust implementation, we should verify whether
	//address is valid (i.e. whether it points to a part we allocated
	//earlier)

	//For our lab, we assume the user always give a valid pointer ;-)
 	toBeFreed = address - hmi.partMetaSize;
	toBeFreed->status = FREE;	//Question: Really this simple?

    //TODO: Implement merging here
}

void compact()
{
    //TODO: Perform compaction
    //Note: You have to call this function on your own now!
}

void cleanUpHeap() {
    //do any clean up here
}

