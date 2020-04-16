/*************************************
* Lab 4 Exercise 5
* Name: Charlotte Lim
* Student No: A0185403J
* Lab Group: 11 (JinYi class)
*************************************/

#include <stdio.h>
#include "mmalloc.h"
#include "semaphore.h"
#include "ex5_extra.h" 
#include "string.h"
#include "stdlib.h"


//This is a "global" structure storing heap information
//Only visible to functions in this file
static heapMetaInfo hmi;
myLock* mySema;



//meta data for our nodes
void initializeMetaPartAt(partMetaInfo* bmiPtr, int size)
{
	bmiPtr->size = size;
	bmiPtr->nextPart = NULL;
	bmiPtr->status = FREE;
}

//to init our mutexes
void initialise()
{
	mySema =  malloc(sizeof(myLock));
    sem_init(&mySema->sharedMutex, 0 ,1);
	sem_init(&mySema->allocateMutex, 0 ,1);
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
	initialise(); //init the mutexes
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

void printHeapStatistics()
{
    printf("Heap Usage Statistics\n");
	printf("===============\n");
    
    //TODO: calculate and print the releavant information
    partMetaInfo* pmi = hmi.base;
	int holeCount =0;
	int holeTotalsize = 0;
	int occupiedSize =0;
	int occupiedCount =0;
	

	do{
		if(pmi->status == FREE){ //if its empty
		holeCount++;
		holeTotalsize = holeTotalsize + pmi->size;
	
		}
		else{ //not empty
		occupiedCount++;
		occupiedSize = occupiedSize + pmi->size;
		}
		pmi = pmi->nextPart;
	}
	while(pmi!=NULL);

    printf("Total Space: %d bytes\n", hmi.totalSize);
    printf("Total Occupied Partitions: %d\n", occupiedCount);
    printf("\tTotal Occupied Size: %d bytes\n", occupiedSize);
    printf("Total Number of Holes: %d\n", holeCount);
    printf("\tTotal Hole Size: %d bytes\n", holeTotalsize);
    printf("Total Meta Information Size: %d bytes\n", hmi.totalSize - (occupiedSize + holeTotalsize)); //should be same as metasize * (total partitions)

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

//merge two empty spaces together 
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

/*
* MyMalloc is O(N) for each run
* MyMalloc has two parts -> search and compact
*
* Search: O(N)
* Searches for the worst fit
* Only one thread shld be running this part
*
* Compact: O(N)
* Reshuffles the heap to collate the bottom 
* Searches again
* Compact itself can be run once 
*  While compact is running, no threads shld try to malloc/free

*/
void* mymalloc(int size)
{
	
    size = (size - 1) / 4 * 4 + 4;
    //TODO: Implement worst fit algorithm here instead of first-fit
	sem_wait(&mySema->allocateMutex);
	sem_wait(&mySema->sharedMutex);
  	partMetaInfo *save = mallocHelper(size); 

	if (save == NULL) { //heap full
	   // compact();
		//save = mallocHelper(size);
		//if(save==NULL){
		   sem_post(&mySema->sharedMutex);
		   sem_post(&mySema->allocateMutex);
		   return NULL;
		//}
	}

	//Can we split the part? 
	//The new "hole" should >= 4 bytes after placing in a new part
	//meta info structure 
	if (save->size >= size + hmi.partMetaSize + 4 ){
		splitPart(save, size);
	}

	save->status = OCCUPIED;
	sem_post(&mySema->sharedMutex);
	sem_post(&mySema->allocateMutex);
	return (void*)save + hmi.partMetaSize;
}

/*
* Charlotte's brainstorm
*
* myfree is split into two -> Merging and setting free.
* No threads can run mymalloc while at least one thread is freeing
* should be done once all threads have called myfree
*
* Set free: O(1)
* We should allow all threads to set their partition free any time.
* More than one thread can run this concurrently
*
* Merging 
* Only one thread at a time
* We should merge all at once becase merging runs for O(n)
*/
void myfree(void* address)
{
	partMetaInfo *toBeFreed;

	//For a robust implementation, we should verify whether
	//address is valid (i.e. whether it points to a part we allocated
	//earlier)
	sem_wait(&mySema->sharedMutex);
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
	sem_post(&mySema->sharedMutex);
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

		   //this moves the pointer to the start of occupied
		   // Copies from that and move to where the free was
		   //memmove(freepointer, freepointer + freeSize,copySize);

		   /* Copies the data starting from nextpart
		   *  Moves it to where my current is currently at
		   */
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

//do any clean up here
void heapCleanUp() 
{
	sem_destroy(&(mySema->sharedMutex));
	sem_destroy(&(mySema->allocateMutex));
}

