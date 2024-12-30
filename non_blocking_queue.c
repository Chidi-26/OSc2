// Student : Chidi Ikwunze ID : 20473929
#include "non_blocking_queue.h"
#include "utilities.h"

#include <assert.h>

void non_blocking_queue_create(NonBlockingQueueT* queue) {
  assert(queue); //expect queue
  queue->list = list_create(); //creating list
  assert(queue->list); //expect queue list to have been made
  pthread_mutex_init(&queue->queueLock, NULL); //initialising mutex
}

void non_blocking_queue_destroy(NonBlockingQueueT* queue) {
  assert(queue->list); //Ensuring queue exsists
  list_destroy(queue->list);
  pthread_mutex_destroy(&queue->queueLock); 
}

void non_blocking_queue_push(NonBlockingQueueT* queue, unsigned int value) {
  assert(queue); //Ensuring queue exsists
  assert(value); //Ensuring value exsists

  pthread_mutex_lock(&queue->queueLock); //Locking queue mutex
  list_append(queue->list, value); // Adding value to the end of the list
  pthread_mutex_unlock(&queue->queueLock); //Unlocking queue mutex
}

int non_blocking_queue_pop(NonBlockingQueueT* queue, unsigned int* value) {
  assert(queue); //Ensuring queue exsists
  assert(value);

  pthread_mutex_lock(&queue->queueLock); //Locking queue mutex

  if(list_empty(queue->list)){
    pthread_mutex_unlock(&queue->queueLock); //Unlocking queue mutex
    return 1;
  }

  *value = list_pop_front(queue->list);
  pthread_mutex_unlock(&queue->queueLock); //Unlocking queue mutex

  return 0;
}

int non_blocking_queue_empty(NonBlockingQueueT* queue) {
  assert(queue); //Ensuring queue exsists
  pthread_mutex_lock(&queue->queueLock); //Locking queue mutex

  if(list_empty(queue->list)){
    pthread_mutex_unlock(&queue->queueLock); //Unlocking queue mutex
    return 1;
  }
  pthread_mutex_unlock(&queue->queueLock); //Unlocking queue mutex
  return 0;
  
}

int non_blocking_queue_length(NonBlockingQueueT* queue) {
  assert(queue); //Ensuring queue exsists
  int lenghtList = 0;

  pthread_mutex_lock(&queue->queueLock); //Locking queue mutex
  lenghtList = list_length(queue->list);
  pthread_mutex_unlock(&queue->queueLock); //Unlocking queue mutex

  return lenghtList;
}
