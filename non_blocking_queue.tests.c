#include "non_blocking_queue.h"
#include "utilities.h"
#include "stdio.h"
#include <assert.h>

void test_que_creation_destruction() {
  NonBlockingQueueT *queue;
  printf("Test non blocking empty creation/destruction\n");
  
  non_blocking_queue_create(queue); //Creates queue
  assert(non_blocking_queue_empty(queue));
  assert(non_blocking_queue_length(queue) == 0);
  
  non_blocking_queue_destroy(queue);

  printf("Creation/destorying test successful!\n"); //prints out if test is succesful
}

void test_que_pop(){
  NonBlockingQueueT *queue; 
  unsigned int value; //stores unsigned int value popped from the queue.
  unsigned int* value_ptr = &value; // points to the unsigned int value address

  printf("Test que pop\n");

  non_blocking_queue_create(queue); // created list

  non_blocking_queue_push(queue, 26); //added 26 to the back of the que
  non_blocking_queue_push(queue, 7); //added 7 to the back of the que

  non_blocking_queue_pop(queue, value_ptr);
  assert(*value_ptr == 26); //checks if the number stored in the value adress is equal to 26
  assert(non_blocking_queue_length(queue) == 1); //checks if que length is equal to 1

  non_blocking_queue_pop(queue, value_ptr);
  assert(*value_ptr == 7); //checks if the number stored in the value adress is equal to 7
  assert(non_blocking_queue_empty(queue));  //Checks if que is empty

  printf("Pop que tests successful!\n"); //prints out if test is succesful
}

void test_que_push(){
  NonBlockingQueueT *queue; 
  printf("Test que push\n");

  non_blocking_queue_create(queue); //Create que

  non_blocking_queue_push(queue, 26); //Add 26 at the end of que
  assert(!non_blocking_queue_empty(queue)); // test if que is not empty
  assert(non_blocking_queue_length(queue) == 1); //Test if que length is equal to 1

  non_blocking_queue_push(queue, 3); //Add 3 at the end of the que
  assert(non_blocking_queue_length(queue) == 2); //Test i f que length is equal to 2

  printf("Push que test successful!\n");
}

void test_que_empty(){
  NonBlockingQueueT *queue; 
  printf("Test empty que\n");

  non_blocking_queue_create(queue); //Create que

  assert(non_blocking_queue_empty(queue)); //Checking if que is empty
  assert(non_blocking_queue_length(queue) == 0); //Checking if length of the que equals 0

  non_blocking_queue_push(queue, 97); //Adding 97 at the end of the que
  assert(!non_blocking_queue_empty(queue)); //Checking if que is not empty
  assert(non_blocking_queue_length(queue) != 0); //Checking if que length is not eual to 0

  printf("Empty que test successful!\n");
}

void test_que_length(){
  NonBlockingQueueT *queue; 
  printf("Test legnth of que\n");

  non_blocking_queue_create(queue); //Creating que
  assert(non_blocking_queue_length(queue) == 0); //Checking if the legnth of que is eual to 0

  non_blocking_queue_push(queue, 78); //Adding 78 to the end of the que
  assert(non_blocking_queue_length(queue) == 1); //Checking if length of que is equal to 1

  non_blocking_queue_push(queue, 99); //Adding 99 at the end of the que
  assert(non_blocking_queue_length(queue) == 2); //Cecking if the length of the list is equal to 2.

  printf("Length test successful!\n");

}



int main() {
  test_que_creation_destruction();
  test_que_pop();
  test_que_push();
  test_que_empty();
  test_que_length();

  printf("All tests are successful!!\n");
  return 0;
}
