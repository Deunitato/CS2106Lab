/*************************************
 * Lab 3 Exercise 3
 * Name: 
 * Student No:
 * Lab Group:
 *************************************/
 
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

#include "traffic_synchronizer.h"

//Using extern, you can use the global variables num_of_cars and num_of_segments from ex3_runner.c in your code.
extern int num_of_cars;
extern int num_of_segments;

void initialise()
{
}

void cleanup()
{
}

void* car(void* car)
{
    //This function modeles a thread 
    //A car: 
    //   -should call enter_roundabout (...)
    //   -followed by some calls to move_to_next_segment (...)
    //   -finally call exit_roundabout (...)
    
    pthread_exit(NULL);
}
