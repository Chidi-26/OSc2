// Student : Chidi Ikwunze ID : 20473929
#include "blocking_queue.h"
#include "utilities.h"
#include "stdio.h"
#include <assert.h>

void test_create_and_destroy_que() {
  BlockingQueueT* queue;
  printf("Test for creating and destroying queue\n");
  

  blocking_queue_create(queue);
  assert(blocking_queue_empty(queue));
  assert(blocking_queue_length(queue) == 0);
  blocking_queue_destroy(queue);  

  printf("Create and destroy test successful!\n");
}

void test_empty_que() {
  BlockingQueueT* queue;
  printf("Test for empty que\n");

  blocking_queue_create(queue);
  assert(blocking_queue_empty(queue));
  assert(blocking_queue_length(queue) == 0);

  blocking_queue_push(queue, 21);
  assert(!blocking_queue_empty(queue));
  assert(blocking_queue_length(queue) == 1);

  blocking_queue_destroy(queue);

  printf("Empty que tests successful!\n");
}

void test_blocking_push(){
  BlockingQueueT* queue;
  unsigned int value = 26;
  

  blocking_queue_create(queue);
  blocking_queue_push(queue, value);
  
  assert(blocking_queue_length(queue) == 1);

  blocking_queue_terminate(queue);
  blocking_queue_push(queue, 21);
  assert(blocking_queue_length(queue) == 1);

  blocking_queue_destroy(queue);
  printf("Pushing que test successful!\n");
}

void test_terminate_que() {
  BlockingQueueT* queue;
  unsigned int* value;
  printf("Test termination\n");

  blocking_queue_create(queue);
  blocking_queue_push(queue, 22);
  assert(blocking_queue_length(queue) == 1);
  
  blocking_queue_terminate(queue);
  
  blocking_queue_push(queue, 23);
  assert(blocking_queue_length(queue) == 1);

  int popResult = blocking_queue_pop(queue, value);
  assert(popResult != 0);

  printf("Termination test successful!\n");
}

void test_pop_queue(){
  BlockingQueueT* queue;
  unsigned int poppedNumber;
  printf("Test pop queue\n");

  blocking_queue_create(queue);
  //push 2 numbers into the queue
  blocking_queue_push(queue, 29);
  blocking_queue_push(queue, 99);
  int poppedResult = blocking_queue_pop(queue,&poppedNumber);
  assert(poppedResult == 0); // ensuring popping occured
  assert(poppedNumber == 29); //ensuring right value was popped

  //ensuring correct behaviour for popping occurs when termination occurs
  blocking_queue_terminate(queue);
  poppedResult = blocking_queue_pop(queue, &poppedNumber);
  assert(poppedResult != 0); //pop shouldn't occur, hense returning a non 0 value

  printf("Pop test successful\n");

}

void test_length_queue(){
  BlockingQueueT* queue;
  printf("Testing Length function\n");

  blocking_queue_create(queue);
  assert(blocking_queue_length(queue) == 0);

  blocking_queue_push(queue, 78);
  assert(blocking_queue_length(queue) == 1);

  blocking_queue_push(queue, 56);
  assert(blocking_queue_length(queue) == 2);

  blocking_queue_terminate(queue);
  blocking_queue_push(queue, 44);
  assert(blocking_queue_length(queue) == 2);

  printf("Length test successful!\n");
}

int main() {
  test_create_and_destroy_que();
  test_empty_que();
  test_blocking_push();
  test_terminate_que();
  test_pop_queue();
  test_length_queue();

  printf("All tests successful!!\n");
  return 0;
}
