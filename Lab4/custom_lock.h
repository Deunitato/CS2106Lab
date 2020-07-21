/*************************************
 * Lab 4 Exercise 1
 * Name:  Charlotte Lim
 * Student No:A0185403J
 * Lab Group: B05
 **************************************/

#include <semaphore.h>

typedef struct {

    //my implementation
    sem_t* semaArr; //semaphore array
    int index;

    //global lock
    sem_t mutex;


    //lock for sort_value function
    sem_t write_lock;

    //max threads that can be waiting on this lock
    int max_threads;

} custom_lock;

