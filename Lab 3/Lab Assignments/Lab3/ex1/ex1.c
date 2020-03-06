
/*************************************
 * Lab 3 Exercise 1
 * Name: Charlotte Lim
 * Student No:
 * Lab Group:
 *************************************/
#include <pthread.h>
#include "rw_lock.h"

void initialise(rw_lock* lock)
{
  pthread_mutex_init(&(lock->mutex), NULL);
  pthread_mutex_init(&(lock->writerCondition), NULL);
  lock->reader_count = 0;
  lock->writer_count = 0;
}

void writer_acquire(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->writerCondition));
  lock->writer_count++;
  //pthread_mutex_unlock(&(lock->mutex));
}

void writer_release(rw_lock* lock)
{
  //pthread_mutex_lock(&(lock->mutex));
  lock->writer_count--;
  pthread_mutex_unlock(&(lock->writerCondition));

}

void reader_acquire(rw_lock* lock) //reader tries to
{

  pthread_mutex_lock(&(lock->mutex));//prevent other readers from reading my count
  if(lock->reader_count==0){
    pthread_mutex_lock(&(lock->writerCondition)); //lock the writer because reading
  }
  lock->reader_count++;
  pthread_mutex_unlock(&(lock->mutex));
}

void reader_release(rw_lock* lock) //reader unlocks
{
  pthread_mutex_lock(&(lock->mutex)); //prevent reader from editing my count
  lock->reader_count--;
  if(lock->reader_count==0){
    pthread_mutex_unlock(&(lock->writerCondition)); //no more reading, writers can write now
  }
  pthread_mutex_unlock(&(lock->mutex));
}

void cleanup(rw_lock* lock)
{
  pthread_mutex_destroy(&(lock->mutex));
  pthread_mutex_destroy(&(lock->writerCondition));
}
