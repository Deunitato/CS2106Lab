/*************************************
 * Lab 3 Exercise 1
 * Name: 
 * Student No:
 * Lab Group:
 *************************************/


#include <pthread.h>

typedef struct {
  pthread_mutex_t mutex;
  pthread_mutex_t writerCondition;
  int reader_count;
  int writer_count;
} rw_lock;

