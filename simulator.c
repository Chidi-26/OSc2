// Student : Chidi Ikwunze ID : 20473929
#include "simulator.h"
#include "list.h"
#include "non_blocking_queue.h"
#include "blocking_queue.h"
#include "utilities.h"
#include "logger.h"
#include "stdio.h"

typedef struct ThreadData{
  int threadID; //unqie thread idenfifier
} ThreadDataT;

typedef struct Process{
  pthread_mutex_t pLock;
  sem_t semaphore;
  int isRunning; // flag to see if process is running
  ProcessIdT processId; // process id unique to current process
  EvaluatorCodeT evalCode; // simulate process execution
}ProcessT;


//Global variables
static pthread_t* threads = NULL; //initialising threads
static int thread_counter = 0; //thread counter
static ThreadDataT* threadData = NULL; // initialising thread data
BlockingQueueT pidQueue; //process id queue
BlockingQueueT readyQueue; // queue for ready processes
static unsigned int maxProcesses = 0; //initialising max processes
static pthread_mutex_t processTableLock; //process table lock
static ListT* pTable = NULL; //initialsing process table
BlockingQueueT eventQueue; // event queue


static ProcessT* getProcessId(ProcessIdT processId){
  pthread_mutex_lock(&processTableLock); //locking process table
  ProcessT* currentProcess = NULL; //initialising current process
  struct List* currentNode = list_find_first(pTable, processId); //finding first occurence of process id in process table

  if(currentNode){
    currentProcess = (ProcessT*)currentNode->value; //if current node isn't null then equate the node value to the current process id
  }
  pthread_mutex_unlock(&processTableLock); //unlocking table
  return currentProcess; 
}

void* simulator_routine(void *arg){
  ThreadDataT* tData = (ThreadDataT*)arg; //casting argument to ThreadDataT* type
  char message[100]; //message buffer

  snprintf(message, sizeof(message), "Thread %d is starting up", tData->threadID);
  logger_write(message); //writing message in logger

  while(1){
    ProcessIdT processId; //declare pid here (showing pid management)
    
    //getting a process id from the ready queue
    while(blocking_queue_pop(&readyQueue, &processId) != 0){
      break; // break if unable to get proceess id from the ready queue
    }

    ProcessT* currProcess = getProcessId(processId); //getting process id from the current running process
    if(!currProcess){
      continue; // if current process equals null skill to the next one
    }

    EvaluatorResultT resultState = evaluator_evaluate(currProcess->evalCode, 0); //simulating running code

    if(resultState.reason == reason_terminated){ // state terminating 
      snprintf(message, sizeof(message), "Process %u has now terminated", currProcess->processId);
      logger_write(message);

      simulator_wait(processId); //wait for process to finish executing
    }
    //if time slice ends, return the process id back into the ready queue 
    if(resultState.reason == reason_timeslice_ended){
      blocking_queue_push(&readyQueue, processId); // requeuing current process in the ready queue
      snprintf(message, sizeof(message), "Process %u has timesliced ended, so requeuing", currProcess->processId);
      logger_write(message);
    }
    // if blocked move process 
    if(resultState.reason == reason_blocked){
      blocking_queue_push(&eventQueue, currProcess->processId);
      snprintf(message, sizeof(message), "Process %u has blocked so moving to event queue", currProcess->processId);
      logger_write(message);
    }
    
  }

  return NULL;
  
}

void simulator_start(int thread_count, int max_processes) {
  //equating global variable with counter agruments
  thread_counter = thread_count;
  maxProcesses = max_processes;
  
  threads = (pthread_t*)checked_malloc(sizeof(pthread_t) * thread_count); //allocating memory for threads
  threadData = (ThreadDataT*)checked_malloc(sizeof(ThreadDataT) * thread_count); //allocating memory for thread data

  //creating ques and process table list
  blocking_queue_create(&pidQueue);
  blocking_queue_create(&readyQueue);
  blocking_queue_create(&eventQueue);
  pTable = list_create();

  pthread_mutex_init(&processTableLock, NULL); //initialising lock

  for(unsigned int i = 0; i < maxProcesses; i++){
    blocking_queue_push(&pidQueue, i);
  }

  logger_start(); // initialise logger

  //creating threads
  for(int i = 0; i < thread_counter; i++){
    threadData[i].threadID = i; // asigning unquie id number for thread data 

    if(pthread_create(&threads[i], NULL, simulator_routine, &threadData[i]) != 0){
      perror("ERROR: Thread unable to be created"); //error message when thread isn't able to create
      exit(1); 
    }
  } 

}

void simulator_stop() {
  //Join each an every thread created
  for(int i = 0; i < thread_counter; i++){
    pthread_join(threads[i], NULL);
  }
  //free memory
  checked_free(threadData); //freeing thread data memory
  checked_free(threads); //freeing thread memoty

  //Setting threads, thread data and thread counter to its initial state
  threads = NULL;
  thread_counter = 0;
  threadData = NULL;

  blocking_queue_destroy(&pidQueue);
  blocking_queue_destroy(&readyQueue);
  blocking_queue_destroy(&eventQueue);
  list_destroy(pTable);

  logger_stop(); // stopping logger
}

ProcessIdT simulator_create_process(EvaluatorCodeT const code) {
  ProcessIdT pid = 0;

  //Getting process id from blocking queue
  if(blocking_queue_pop(&pidQueue, &pid) != 0){
    fprintf(stderr,"ERROR,unable to retrive process id\n"); // error message
    exit(EXIT_FAILURE); //exit
  }

  ProcessT* newProcess = checked_malloc(sizeof(ProcessT)); // allocating space for ProcessT data
  newProcess->processId = pid; //setting unique process id
  newProcess->evalCode = code; //setting process evaluator code
  newProcess->isRunning = 1; //setting process flag state tto 1 - running
  
  pthread_mutex_init(&processTableLock, NULL); //initialising mutex
  
  pthread_mutex_lock(&processTableLock);
  list_append(pTable, (unsigned int)newProcess);// stores processes in the process table by casting process to unsigned int
  pthread_mutex_unlock(&processTableLock);

  char message[100]; //message
  snprintf(message, sizeof(message), "Process %u has been created", pid);
  logger_write(message);
  
  return pid;
}

void simulator_wait(ProcessIdT pid) {
  ProcessT* currentProcess = getProcessId(pid);

  if(!currentProcess){
    return; // return if current process is equal to NULL/ not found
  }
  sem_wait(&currentProcess->semaphore);

  pthread_mutex_lock(&processTableLock);
  list_remove(pTable, (struct List*)currentProcess);
  pthread_mutex_unlock(&processTableLock);

  checked_free(currentProcess);

  blocking_queue_push(&pidQueue, pid); //recycling into id queue

  char message[100];
  snprintf(message, sizeof(message), "Process %u has been finishsed", pid);
  logger_write(message); // write out logger message
}

void simulator_kill(ProcessIdT pid) {
}

void simulator_event() {
  ProcessIdT processId;

  // moving event queue process id into the front of the ready queue
  if(blocking_queue_pop(&eventQueue, &processId) == 0){
    blocking_queue_push(&readyQueue, processId); 
  }
  char message[100]; // message buffer
  snprintf(message, sizeof(message), "Event process id %u has been moved to ready queue", processId);
  logger_write(message); // printing message indicating the specific process id from the event queue which has been added the the frony of the ready queue
}
