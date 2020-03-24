/*************************************
 * Lab 4 Exercise 1
 *************************************
 You may change this file during your own testing, 
 but note that they will be replaced with the original 
 files when we test your assignments.
 *************************************/

//for usleep warning
#define _BSD_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "my_semaphore.h"

//input - number to sort
int N;

//result array + pointer to help insert into array
int* res;
int res_pointer;

//to prevent race conditions in runner
sem_t mutex;

//custom struct
custom_lock* lock;

//to keep time
const int TIME_INTERVAL = 5000;

//for error message
const char* MEMORY_ERROR = "Failed to allocate memory\n";
const char* THREAD_ERROR = "Error creating thread\n";

void *sort_value(void* x);
void err(const char *msg);

int main(int argc, char** argv) 
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <N>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    N = atoi(argv[1]);
    if (N < 0) {
        fprintf(stderr, "Invalid N value, N should be > %d\n", 0);
        exit(EXIT_FAILURE);
    }

    //initialize variables for runner
    res = malloc(sizeof(int) * N);
    if (res == NULL) {
        err(MEMORY_ERROR);
    }

    res_pointer = N - 1;
    sem_init(&mutex, 0, 1);

    //create and shuffle an array of size N
    int* raw_data = malloc(sizeof(int) * N);
    if (raw_data == NULL) {
        err(MEMORY_ERROR);
    }

    int i;
    for (i = 0; i < N; i++) {
        raw_data[i] = i + 1; 
    }

    for (i = 1; i < N; i++) {
        int j = rand() % i;
        int temp = raw_data[j];
        raw_data[j] = raw_data[i];
        raw_data[i] = temp;
    }

    printf("INITIAL ARRAY: ");
    for (i = 0; i < N; i++) {
        if (i) printf(" ");
        printf("%d", raw_data[i]);
    }
    printf("\n");

    //setup custom struct
    lock = malloc(sizeof(custom_lock));
    lock->max_threads = N;
    initialise(lock);

    //create and start our threads
    pthread_t workers[N];
    for (i = 0; i < N; i++) {
        int *x = malloc(sizeof(int));
        if (x == NULL) {
            err(MEMORY_ERROR);
        }

        *x = i + 1;
        int ret = pthread_create(&workers[i], NULL, sort_value, x);
        if (ret) {
            err(THREAD_ERROR);
        }
    }

    //sleep for a larger interval (ensures all threads have run)
    usleep(TIME_INTERVAL * N * 2);

    //wake up everyone and collect
    notify_all(lock);
    for (i = 0; i < N; i++) {
        pthread_join(workers[i], NULL);
    }

    //print final result
    printf("FINAL ARRAY: ");
    for (i = 0; i < N; i++) {
        if (i) printf(" ");
        printf("%d", res[i]);
    }
    printf("\n");

    //check correctness 
    int ok = 1;
    for (i = 1; i < N; i++) {
        ok &= (res[i] > res[i-1]);
    }
    if (ok) printf("Program is correct!\n");
    else printf("Program failed :(\n");

    cleanup(lock);
    free(res);
    free(raw_data);

    return 0;
}

//we run this with N threads
void *sort_value(void* x) {
    int val = *((int*)x);
    free(x);

    //sleep for time proportional to val
    usleep(TIME_INTERVAL * val);

    wait(lock); 

    //add the value to result backwards
    sem_wait(&mutex);
    res[res_pointer--] = val;
    sem_post(&mutex);

    //release write lock
    sem_post(&lock->write_lock);

    pthread_exit(NULL);
}

void err(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}

