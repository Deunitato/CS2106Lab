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

void mergePart(partMetaInfo *part1, partMetaInfo *part2){

	int p1Size = part1->size;
	int p2Size = part2->size;
	part1->size = p1Size + p2Size; //change size
	part1->nextPart = part2->nextPart; //change pointer
	//free(part2);
}

//mallocHelper looks for the largest "Worst fit" mememory and return it.
partMetaInfo* mallocHelper(int size){
	partMetaInfo *save = hmi.base;
	partMetaInfo *current = NULL;

		for (current = hmi.base; current != NULL; current = current->nextPart) {
			if (current->status == OCCUPIED) continue; //current is occupied
			if (current->size < size) continue; //current too small
			if(current->size > save->size){ //save if the new one is larger than my current
				save = current;
				continue; //move to the next
			}
		}
		return save;
}

void mymalloc(void** ref, int size)
{
    // We need to make sure the size is word
    // aligned, i.e. if the word size is 4 bytes, the size need to be
    // rounded to nearest multiples of 4. Otherwise, user can get "bus
    // error" when accessing non-aligned memory locations

    // divide by 4 then multiply by 4 gives rounded multiples of 4. 
    // addition  of 4 round up to the next multiple 
    // subtraction take care of the case where size is already multiples
    // of 4
    size = (size - 1) / 4 * 4 + 4;
    partMetaInfo *save = mallocHelper(size);
    //TODO: Copy your solution from previous exercise here 
	if (save == NULL) { //heap full
	    compact();
		save = mallocHelper(size);
		if(save==NULL){
			*ref = NULL;
			return;
		} 
	}
    

	//Can we split the part? 
	//The new "hole" should >= 4 bytes after placing in a new part
	//meta info structure 
	if (save->size >= size + hmi.partMetaSize + 4 ){
		splitPart(save, size);
	}

	save->status = OCCUPIED;
	*ref = (void*)save + hmi.partMetaSize;
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
	partMetaInfo *current = hmi.base;
	partMetaInfo *next = current->nextPart;

	while(next!=NULL){
		if(current->status ==FREE && next->status == FREE){
			mergePart(current,next);
			next = current -> nextPart;
		}
		else{
			current = current->nextPart;
			next = current->nextPart;
		}
	}
}


void compact()
{
    //TODO: Perform compaction
    //Note: The relative ordering of the occupied partitions should be
    //      maintained.

    //Remember that the _content_ of each partition need to be copied
    // too. Look into memmove() library call 
	partMetaInfo *current = hmi.base;
	void* freepointer = hmi.base;
	int copySize = 0;
	int freeSize =0;


	//navigate to the first free slot
	while(current->status == OCCUPIED){
		current = current->nextPart;
		freepointer = (void*)current;
	}


	//start merging
	while(current!= NULL && current->nextPart!=NULL){
		if((current->nextPart)->status == OCCUPIED && current->status == FREE){ //if current is free and next is occupied
		   //move that occupied forward
		   copySize = hmi.partMetaSize + (current->nextPart) ->size; //size of my occupied
		   freeSize = hmi.partMetaSize + current->size; //size of my free

		   memmove(current, current->nextPart,copySize);
		   
		   
		   freepointer = freepointer + copySize; //move the pointer to the free part
		   void* currNext = current->nextPart; 
		
		   current ->nextPart = freepointer; //reset the pointer to next
		   //init the free
		   initializeMetaPartAt(freepointer, freeSize - hmi.partMetaSize);

		   partMetaInfo *newPmi = freepointer; 
		   
		  // partMetaInfo *freePartionn = current->nextPart; 
		   newPmi->nextPart = currNext; //free pointer next to be current's next
		   current = freepointer;
		   continue;
		}
		else if((current->nextPart)->status == FREE && current->status == FREE) { //the next is free
		//merge the free together
			mergePart(current,current->nextPart);
			continue;
			
		}
		current = current->nextPart;
	}
}

void cleanUpHeap() {
    //do any clean up here
	/*partMetaInfo *current = hmi.base;
	
	while(current->nextPart !=NULL){
		partMetaInfo *freeed = current;
		current = current->nextPart;
		free(freeed);
	}*/
}

