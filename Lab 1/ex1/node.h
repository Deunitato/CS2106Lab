/*
	You can modify this file for your testing purpose 
	but note that we will use our own copies of this file for testing during grading 
	so changes in this file will be overwritten
*/

#define MAX_WORD_LENGTH 10

typedef struct NODE {
	struct NODE *prev;
	struct NODE *next;
    char *word;
} node;

typedef struct {
	node *head;
} list;

// function prototypes
void insert_node_before(list *lst, int index, char *word);
void insert_node_after(list *lst, int index, char *word);
char *list_to_sentence(list *lst);
void delete_node(list *lst, int index);
void delete_list(list *lst);
