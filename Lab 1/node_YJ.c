// general purpose standard C lib
#include <stdio.h>
#include <stdlib.h> // includes malloc(), free()
#include <string.h> // includes strlen(), memcpy()
#include <ctype.h> // includes toupper(), tolower()
#include <stdbool.h>

// user-defined header files
#include "node.h" // do not modify this file

// put your function prototypes for additional helper functions below:

bool is_empty(list *lst) {
    return !(lst->head);
}

// implementation
void insert_node_before(list *lst, int index, char *word)
{	
    // printf("%s", word);
    node *new_node;
    new_node = (node*) malloc(sizeof(node));
    if (is_empty(lst)) { // head of lst points to inserted node
        new_node->prev = new_node;
        new_node->next = new_node;
        new_node->word = word;
        lst->head = new_node;
    } else {
        node *curr = lst->head; // need to change curr's prev
        node *prev = curr->prev; // need to change prev's next
        if (index == 0) { // head of lst does not change
            new_node->prev = prev;
            new_node->next = curr;
            new_node->word = word;
        } else if (index > 0) { // go clockwise (forward, ++)
            int i = 0;
            while (i < index) {
                i++;
                curr = curr->next;
                prev = prev->next;
            }
            new_node->prev = prev;
            new_node->next = curr;
            new_node->word = word;
        } else { // go anticlockwise (backwards, --)
            int i = 0;
            while (i > index) {
                i--;
                curr = curr->prev;
                prev = prev->prev;
            }
            new_node->prev = prev;
            new_node->next = curr;
            new_node->word = word;
        }
        curr->prev = new_node;
        prev->next = new_node;
    }
    //printf("%s", word);
}

void insert_node_after(list *lst, int index, char *word)
{
    //printf("%s", word);
    node *new_node;
    new_node = (node*) malloc(sizeof(node));
    if (is_empty(lst)) { // head of lst points to inserted node
        new_node->prev = new_node;
        new_node->next = new_node;
        new_node->word = word;
        lst->head = new_node;
    } else {
        node *curr = lst->head; // need to change curr's next
        node *next = curr->next; // need to change next's prev
        if (index == 0) { // head of lst does not change
            new_node->prev = curr;
            new_node->next = next;
            new_node->word = word;
        } else if (index > 0) { // go clockwise (forward, ++)
            int i = 0;
            while (i < index) {
                i++;
                curr = curr->next;
                next = next->next;
            }
            new_node->prev = curr;
            new_node->next = next;
            new_node->word = word;
        } else { // go anticlockwise (backwards, --)
            int i = 0;
            while (i > index) {
                i--;
                curr = curr->prev;
                next = next->prev;
            }
            new_node->prev = curr;
            new_node->next = next;
            new_node->word = word;
        }
        curr->next = new_node;
        next->prev = new_node;
    }
    //printf("%s", word);
}

char *list_to_sentence(list *lst)
{
    // returns a sentence containing all words in lst 
    node *curr;
    node *next;
    curr = lst->head; 
    next = curr->next;
    char *sentence;
    sentence = (char*) malloc(11 * sizeof(char));
    strcpy(sentence, curr->word);
    while (next != curr) {
        strcat(sentence, " ");
        strcat(sentence, next->word);
        next = next->next;
    }
    sentence[0] = toupper(sentence[0]);
    for (int i = 1; i < strlen(sentence); ++i) {
        sentence[i] = tolower(sentence[i]);
    }
    strcat(sentence, ".");
    //printf("%s", sentence);
    return sentence;
}

void delete_node(list *lst, int index)
{
    node *curr = lst->head;
    if (curr == curr->next) {
        lst->head = NULL;
        free(curr->word);
        free(curr);
        return;
    }
    if (index == 0) {
        node *next = curr->next;
        node *prev = curr->prev;
        lst->head = next;
        next->prev = prev;
        prev->next = next;
        free(curr->word);
        free(curr);
    } else if (index > 0) {
        int i = 0;
        node *next = curr->next;
        node *prev = curr->prev;
        while (i < index) {
            ++i;
            next = next->next;
            curr = curr->next;
            prev = prev->next;
        }
        next->prev = prev;
        prev->next = next;
        free(curr->word);
        free(curr);
    } else {
        int i = 0;
        node *next = curr->next;
        node *prev = curr->prev;
        while (i > index) {
            --i;
            next = next->prev;
            curr = curr->prev;
            prev = prev->prev;
        }
        next->prev = prev;
        prev->next = next;
        free(curr->word);
        free(curr);
    }
}

void delete_list(list *lst)
{
    while (lst->head) {
        delete_node(lst, 0);
    }
}

/*
int main() {
}

typedef struct test {
    int *ptr;
} test;

int main() {
    int num = 28;
    test obj = { &num };
    printf("hello\n");
    printf("%d", *(obj.ptr));
}
*/
