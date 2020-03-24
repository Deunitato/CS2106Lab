/*************************************
 * Lab 4 Exercise 1
 * Name: 
 * Student No:
 * Lab Group:
 **************************************/
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "my_semaphore.h"

//intialise everything in lock here
void initialise(custom_lock* lock)
{
    //modify here
    sem_init(&lock->write_lock, 0, 0);
    int total = lock->max_threads;
    lock->semaArr = malloc(sizeof(sem_t) * total);
    lock->index = 0;
    for (int i = 0; i < total; i++) {
        sem_init(&(lock->semaArr[i]), 0, 0);
    }
}

//wait takes in a custom struct
//we want the thread to be added to our semaphore here
void wait(custom_lock* lock)
{
    int currentindex = lock->index;  //we know that they will not write at the same time
    lock->index = currentindex + 1;
    sem_wait(&(lock->semaArr)[currentindex]);  //<n has to wait here, only n get to pass
}
//notify_all takes in a custom struct
//we want to wake all the sleeping threads here!
void notify_all(custom_lock* lock)
{
    int i;
    for (i = lock->max_threads - 1; i >= 0; i--) {
        sem_post(&(lock->semaArr)[i]);  //n wake up, where n is my current thread starting from back
        sem_wait(&lock->write_lock);
    }
}

//free anything allocated here
void cleanup(custom_lock* lock)
{
    //modify here
    int total = lock->max_threads;
    for (int i = 0; i < total; i++) {
        sem_destroy(&(lock->semaArr[i]));
    }
    sem_destroy(&lock->write_lock);
    free(lock->semaArr);
}
