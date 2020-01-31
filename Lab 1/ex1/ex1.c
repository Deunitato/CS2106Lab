/*
	You can modify this file for your testing purpose 
	but note that we will use our own copies of this file for testing during grading 
	so changes in this file will be overwritten
*/

// general purpose standard C lib
#include <stdio.h>
#include <stdlib.h> // stdlib includes malloc() and free()

// user-defined header files
#include "node.h"

// macros
#define PRINT_LIST 0

#define INSERT_BEFORE 1
#define INSERT_AFTER 2
#define LIST_TO_SENTENCE 3
#define DELETE_NODE 4
#define DELETE_LIST 5

// function prototypes
void print_list(list * lst); // implemented for you
void run(list * lst); // implemented for you


int main()
{
    list * lst = (list *) malloc(sizeof(list));
    lst->head = NULL;
	run(lst);
    if (lst->head != NULL) delete_list(lst);
    free(lst);
	return 0;
}

// parse stdin and call corresponding functions to operate on the list *
void run(list *lst) 
{
	while(1) {
		int instruction;

		if (scanf("%d", &instruction) <= 0) {
			break;
		}

        if (instruction < 0 || instruction > 6) {
            fprintf(stderr, "Invalid instruction\nTerminating...\n");
            break;
        }

        if (instruction == PRINT_LIST) {
            print_list(lst);
            continue;
        }

        if (instruction == DELETE_LIST) {
            delete_list(lst);
            continue;
        }

        if (instruction == LIST_TO_SENTENCE) {
            char *sentence = list_to_sentence(lst);
            printf("%s\n", sentence);
            free(sentence);
            continue;
        }

        int index;
        if (scanf("%d", &index) <= 0) {
            fprintf(stderr, "Failed to read index\nTerminating...\n");
            break;
        }

        if (instruction == DELETE_NODE) {
            delete_node(lst, index);
            continue;
        }

        char *word = (char *)malloc(sizeof(char) * (MAX_WORD_LENGTH+1));
        if (scanf("%s", word) <= 0) {
            fprintf(stderr, "Failed to read word\nTerminating...\n");
            break;
        }

        if (instruction == INSERT_BEFORE) {
            insert_node_before(lst, index, word);
        } else if (instruction == INSERT_AFTER) {
            insert_node_after(lst, index, word);
        } else {
			fprintf(stderr, "Invalid instruction\nTerminating...\n");
			break;
		}

	}
}

// print out the whole list * and words clockwise once and anti-clockwise once
// clockwise is defined as keep travelling to next node
// anti-clockwise is defined as keep travelling to prev node
// print out the whole list * and words clockwise once and anti-clockwise once
// clockwise is defined as keep travelling to next node
// anti-clockwise is defined as keep travelling to prev node
void print_list(list * lst)
{
    if (lst->head == NULL) {
        fprintf(stderr, "Cannot print an empty list\n");
        return;
    }

	int count = 0;

	printf("Printing clockwise:\n");
	node *curr_node = lst->head;
	do {
		printf("[Idx %d:%s]", count, curr_node->word);
		printf("\n   |\n   v\n");
		curr_node = curr_node->next;
        count++;
	} while(curr_node != lst->head);
    printf("[Idx %d:%s]\n", count, curr_node->word);
	
	printf("\nPrinting counter-clockwise:\n");
	do {
		printf("[Idx %d:%s]", count, curr_node->word);
		printf("\n   |\n   v\n");
		curr_node = curr_node->prev;
        count--;
	} while(curr_node != lst->head);
    printf("[Idx %d:%s]\n", count, curr_node->word);
}

