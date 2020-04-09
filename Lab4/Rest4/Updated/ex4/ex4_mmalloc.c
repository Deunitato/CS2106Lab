/*************************************
* Lab 4 Exercise 4
* Name:
* Student No:
* Lab Group:
*************************************/

#include <stdio.h>
#include "mmalloc.h"
#include "string.h"

//This is a "global" structure storing heap information
//Only visible to functions in this file
static heapMetaInfo hmi;

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
	partMetaInfo *save = NULL;
	partMetaInfo *current = NULL;
	int first = 1;

		for (current = hmi.base; current != NULL; current = current->nextPart) {
			if (current->status == OCCUPIED) continue; //current is occupied
			if (current->size < size) continue; //current too small
			if(first == 1){
				save = hmi.base;
				first =0;
				continue;
			}

			if(current->size > save->size){ //save if the new one is larger than my current
				save = current;
				continue; //move to the next
			}
			break;    
		}
		return save;
}

void* mymalloc(int size)
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
    
    //TODO: Implement worst fit algorithm here instead of first-fit
  	partMetaInfo *save = mallocHelper(size);


	if (save == NULL) { //heap full
	    compact();
		save = mallocHelper(size);
		if(save==NULL) return NULL;
	}

	//Can we split the part? 
	//The new "hole" should >= 4 bytes after placing in a new part
	//meta info structure 
	if (save->size >= size + hmi.partMetaSize + 4 ){
		splitPart(save, size);
	}

	save->status = OCCUPIED;
	return (void*)save + hmi.partMetaSize;
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
	/*partMetaInfo *left = (void*)toBeFreed - hmi.partMetaSize;
	partMetaInfo *right = toBeFreed ->nextPart;


	if(right->status ==FREE){ //need to merge the left
	  mergePart(toBeFreed,right); //merge the right with my current
	  printf("merge\n");
	}

	if(left->status == FREE){ //need to merge the left
	  mergePart(left,toBeFreed); //merge the left with my current
	  printf("merge\n");
	}*/
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
	while(current->nextPart!=NULL){
		if((current->nextPart)->status == OCCUPIED){ //if next is occupied
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
		   
		   
		   freepointer = freepointer + copySize; //move the pointer to the next
		   current ->nextPart = freepointer; //reset the pointer to next

		   //Merge the extra freed
		   initializeMetaPartAt(freepointer, freeSize - hmi.partMetaSize);
		}
		else { //the next is free
		//merge the free together
			mergePart(current,current->nextPart);
		}
		current = current->nextPart;
	}


	//collect all the empty slots
	/*while(current!= NULL){
		if(current ->status == FREE){ //if it is free, ill collect it
			newSize = newSize + current->size;
		}
		else{
			memmove(newBase,current,sizeof(partMetaInfo)+ current->size); //move the memory to new 
			if(current->nextPart!=NULL){ //if my next is not the ened
				partMetaInfo *buffer = (void*) newBase + hmi.partMetaSize + newBase->size; //move pointerforwards
				newBase->nextPart = buffer; //reset the next part's pointer
				newBase = buffer; //move there
			}
			
		}
		current = current ->nextPart;
	}	*/

	// initializeMetaPartAt(newHole,newSize); //create new hole
	// newBase ->nextPart = newHole; //set the space
}


//Do NOT Change
void verifyCompact()
{
  partMetaInfo *current;
  int *array, first, last;

  printf("Compact Verifier:\n");
  printf("=================\n");
  printf("Content Check:\n");
  for (current = hmi.base ;current != NULL && current->status != FREE; 
      current = current->nextPart) {

    //Pointer arithmetic is based on the type of pointer
    // so, need to use (void*) to calculate the offsetproperly
    array = (int*)((void*)current + hmi.partMetaSize);

    //print first and last element
    first = array[0];
    last = array[ current->size / sizeof(int) - 1];
    printf("\t[%d == %d]", first, last);
    if (first != last){
      printf(" = FAILED!\n");
    } else {
      printf("\n");  
    }
  }

  //Should not have any more partition after the last free partition
  printf("Partition Check: ");
  if (current != NULL && current->nextPart != NULL){
    printf("FAILED!\n");
  } else {
    printf("PASSED\n");
  }
  printf("\n");
}

