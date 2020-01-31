// general purpose standard C lib
#include <stdio.h>
#include <stdlib.h> // includes malloc(),free()
#include <string.h> // includes strlen(), memcpy()
#include <ctype.h> // includes toupper(), tolower()

// user-defined header files
#include "node.h" // do not modify this file

// put your function prototypes for additional helper functions below:

// implementation
void insert_node_before(list *lst, int index, char *word)
{	
	// TODO
	//create node first
	struct NODE *nodePtr;
	nodePtr = (node*)malloc(sizeof(node));
	(*nodePtr).word = word; //assign value

	if (lst->head == NULL) { //empty list
	 (*nodePtr).prev = nodePtr;
	 (*nodePtr).next = nodePtr;
	  lst->head = nodePtr;
	}
	else{ //assess the list and insert
	 //offset index:
	 
	 node *currPtr = (*lst).head;
	 node *next = currPtr;
	 node *prev=(*next).prev;

	 if(index>=0){ //go forward
	   for(int i = 0; i < index; i ++){ //get the index of before
		 currPtr = (*currPtr).next; //get next
	   }
	 }
	 else{ //go backwards
	   for(int i = 0; i > index; i --){ //get the index of before
     	 currPtr = (*currPtr).prev; //get next
	    }
	 }
	   next = currPtr;
	   prev = (*currPtr).prev;

	 
	  currPtr = nodePtr;  //set hole as me
	  (*prev).next = nodePtr; //set previous node's prev as me
	  (*next).prev = nodePtr; //set pushed next prev as me

	  //Set myvalues
	  (*nodePtr).prev = prev;
	  (*nodePtr).next = next;
	}
}

void insert_node_after(list *lst, int index, char *word)
{
	// TODO
	insert_node_before(lst, index+1, word);
}

char *list_to_sentence(list *lst)
{
	// TODO
	int DEST_SIZE = 400;
	node *nodeHead = (*lst).head;
	node *nodePtr = (*lst).head;
	int isFirst = 1;
	char *sentence;
    sentence = (char*) malloc(11 * sizeof(char));

	//copy
	do{
		if(isFirst == 1){
			strcpy(sentence,(*nodePtr).word);
			isFirst = 0;
		}
		else{
			strcat(sentence, (*nodePtr).word);
		}
		strcat(sentence, " ");
		nodePtr = (*nodePtr).next;
	}while((nodePtr)!=nodeHead);


	//edit the string
	//to caps
	sentence[0] = toupper(sentence[0]);
	//to lower
	for(int i = 1; i < strlen(sentence); i++){
		sentence[i] = tolower(sentence[i]);
	}

    //fullstop
	sentence[strlen(sentence)-1] = '.';
	strcat(sentence, "\0");
    return sentence; // Change this line accordingly
}

void delete_node(list *lst, int index)
{
	// TODO
	node *nodePtr = (*lst).head;
	//check if next is me
	if((*nodePtr).next == (*lst).head){
		(*lst).head = NULL;
		free((*nodePtr).word);
		free(nodePtr);
	}
	else if(index>=0){ //go forwards
	   for(int i = 0 ; i < index; i++){
		   nodePtr = (*nodePtr).next;
	   }

	}
	else{//go backwards
	   for(int i = 0 ; i > index; i--){
		   nodePtr = (*nodePtr).prev;
	   }
	}

	//delete 
	node *prev = (*nodePtr).prev;
	node *next = (*nodePtr).next;

	(*prev).next = next; //set my prev's next to my next
	(*next).prev = prev; //set my next's prev to my prev
	free((*nodePtr).word); //delete myself
	free(nodePtr);
}

void delete_list(list *lst)
{
	// TODO
	//delete everything
	node *nodePtr = (*lst).head;
	node *nodeHead = nodePtr;
	//check if next is me
	while((nodePtr)!=nodeHead){
		
		free((*nodePtr).word);
		free(nodePtr);
	}
	 (*lst).head = NULL;

}
