/*************************************
 * Lab 4 Exercise 1
 * Name: 
 * Student No:
 * Lab Group:
 **************************************/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include "my_semaphore.h"


void debugPrintSema(custom_lock* lock){
    printf("\nHere are the sem values \n");
     for(int i =0; i< lock->max_threads ; i++){
       int k =0;
       sem_getvalue(&(lock->semaArr[i]), &k);
       printf("index %d: %d\n", i,k);
   }

}

//intialise everything in lock here 
void initialise(custom_lock* lock) {

    //modify here
    sem_init(&lock->mutex, 0, 0);
    int total = lock -> max_threads;
    lock ->semaArr = malloc (sizeof (sem_t) * total);
    lock ->index = 0;
    //sem_t *arrOfSem = malloc(sizeof(sem_t) * total);

    for(int i = 0 ; i < total ; i++){
         sem_init(&((lock->semaArr))[i], 0, 0);
   }

   //lock->semaArr = arrOfSem;
}

//wait takes in a custom struct
//we want the thread to be added to our semaphore here
void wait(custom_lock* lock) {


    int currentindex = lock -> index; //we know that they will not write at the same time
    //printf("Being wait: %d\n",currentindex);
    int next = currentindex + 1;
    lock-> index = next;


    sem_wait(&(lock->semaArr)[currentindex]); //<n has to wait here, only n get to pass
    
}
//notify_all takes in a custom struct
//we want to wake all the sleeping threads here!
void notify_all(custom_lock* lock) {

    //modify here
    int i;
    for (i = lock->max_threads -1 ; i >=0 ; i--) {
    sem_post(&(lock->semaArr)[i]); //n wake up, where n is my current thread starting from back
    //wait for this value to be written before moving on
    sem_wait(&lock->write_lock);
    }
}

//free anything allocated here
void cleanup(custom_lock* lock) {

    //modify here
    int total = lock->max_threads;
    for(int i = 0 ; i < total ; i++){
        sem_destroy(&(lock->semaArr[i]));
   }
    sem_destroy(&lock->write_lock);
}



