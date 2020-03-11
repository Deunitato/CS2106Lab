
/*************************************
 * Lab 3 Exercise 2
 * Name: Charlotte Lim
 * Student No:
 * Lab Group:
 *************************************/
#include <pthread.h>
#include "rw_lock.h"

void initialise(rw_lock* lock)
{
  pthread_mutex_init(&(lock->mutex), NULL);
  pthread_mutex_init(&(lock->sharedMutex), NULL);
  pthread_mutex_init(&(lock->letmeINNNN), NULL);
  lock->reader_count = 0;
  lock->writer_count = 0;
}

void writer_acquire(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->letmeINNNN)); //if there is writer waiting
  pthread_mutex_lock(&(lock->sharedMutex));
  lock->writer_count++;
  //pthread_mutex_unlock(&(lock->mutex));
}

void writer_release(rw_lock* lock)
{
  //pthread_mutex_lock(&(lock->mutex));
  lock->writer_count--;
  pthread_mutex_unlock(&(lock->sharedMutex));
  pthread_mutex_unlock(&(lock->letmeINNNN)); //unlock once my writer finish
}

void reader_acquire(rw_lock* lock) //reader tries to
{
  /*if(lock->writer_count > 0){
    pthread_mutex_lock(&(lock->letmeINNNN)); //let writer write
    pthread_mutex_lock(&(lock->sharedMutex));
  }*/
  pthread_mutex_lock(&(lock->letmeINNNN)); //let writer write

  pthread_mutex_lock(&(lock->mutex));//prevent other readers from reading my count
  if(lock->reader_count==0){
    pthread_mutex_lock(&(lock->sharedMutex)); //lock the writer because reading
  }
  lock->reader_count++;
  pthread_mutex_unlock(&(lock->mutex));
  pthread_mutex_unlock(&(lock->letmeINNNN)); 
}

void reader_release(rw_lock* lock) //reader unlocks
{
  pthread_mutex_lock(&(lock->mutex)); //prevent reader from editing my count
  lock->reader_count--;
  if(lock->reader_count==0){
    pthread_mutex_unlock(&(lock->sharedMutex)); //no more reading, writers can write now
  }
  pthread_mutex_unlock(&(lock->mutex));
}

void cleanup(rw_lock* lock)
{
  pthread_mutex_destroy(&(lock->mutex));
  pthread_mutex_destroy(&(lock->sharedMutex));
  pthread_mutex_destroy(&(lock->letmeINNNN));
}
