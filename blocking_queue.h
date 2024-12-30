// Student : Chidi Ikwunze ID : 20473929
#ifndef _BLOCKING_QUEUE_H_
#define _BLOCKING_QUEUE_H_

#include "list.h"
#include "pthread.h"
#include "semaphore.h"

typedef struct BlockingQueue {
  /* Add fields as needed */
  pthread_mutex_t queueLock; //mutex id
  ListT* list; // holds queue elements
  sem_t semQue; //semaphore id
  int terminate; // flag to see if que has terminated
} BlockingQueueT;

void blocking_queue_create(BlockingQueueT* queue);
void blocking_queue_destroy(BlockingQueueT* queue);

void blocking_queue_push(BlockingQueueT* queue, unsigned int value);
int blocking_queue_pop(BlockingQueueT* queue, unsigned int* value);

int blocking_queue_empty(BlockingQueueT* queue);
int blocking_queue_length(BlockingQueueT* queue);

void blocking_queue_terminate(BlockingQueueT* queue);

#endif
