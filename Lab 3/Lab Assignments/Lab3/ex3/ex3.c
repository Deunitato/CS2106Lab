/*************************************
 * Lab 3 Exercise 3
 * Name: Charlotte Lim
 * Student No: A0185403J
 * Lab Group: B05
 *************************************/
 
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

#include "traffic_synchronizer.h"

//Using extern, you can use the global variables num_of_cars and num_of_segments from ex3_runner.c in your code.
extern int num_of_cars;
extern int num_of_segments;


//init sema
sem_t *segmentMutex; 
sem_t carMutex;





void initialise()
{
    sem_init(&carMutex, 0 ,num_of_segments-1);
    segmentMutex = malloc (sizeof (sem_t) * num_of_segments);
    for(int i = 0 ; i < num_of_segments ; i++){
        sem_init(&(segmentMutex[i]), 0, 1);  //only one car can move each time <== Possible seg fault
   }
}

void cleanup()
{
    for(int i = 0 ; i < num_of_segments ; i++){
        sem_destroy(&segmentMutex[i]);
   }
   sem_destroy(&carMutex);
   free(segmentMutex);
}

void* car(void* car)
{
    //This function modeles a thread 
    //A car: 
    //   -should call enter_roundabout (...)
    //   -followed by some calls to move_to_next_segment (...)
    //   -finally call exit_roundabout (...)
     
     car_struct* myCar = (car_struct *) car;
     sem_wait(&carMutex); //let one car in
     int enter = myCar-> entry_seg;
     sem_wait(&segmentMutex[enter]); //check there is no other car in my entry point
     enter_roundabout(myCar); 


     int currentSeg = myCar-> current_seg;
     int exitSeg = myCar ->exit_seg;
     

     //try to move here
    while(currentSeg!= exitSeg){
        int next = NEXT(currentSeg, num_of_segments);
        sem_wait(&segmentMutex[next]);

        move_to_next_segment(myCar); //move to the next segment

        int prev = PREV(myCar->current_seg,num_of_segments);
        sem_post(&segmentMutex[prev]); //signal the prev segment to be empty
        currentSeg = myCar-> current_seg; //get new valye
    } 
     exit_roundabout(myCar);
     sem_post(&segmentMutex[exitSeg]); //leave
     sem_post(&carMutex); //one car left

    pthread_exit(NULL);
}
