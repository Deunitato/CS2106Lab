/*************************************
* Lab 4 Exercise 5
* Name: Charlotte Lim
* Student No: A0185403J
* Lab Group: 11 (JinYi class)
*************************************/

typedef struct
{
    sem_t sharedMutex;  
    sem_t allocateMutex;
} myLock;

void initialise();
void compact();