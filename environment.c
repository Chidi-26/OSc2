// Student : Chidi Ikwunze ID : 20473929
#include "environment.h"
#include "simulator.h"
#include "utilities.h"
#include "evaluator.h"
#include "list.h"
#include "pthread.h"
#include "stdio.h"
#include "logger.h"

typedef struct EnvThreadData{
	unsigned int iterations; //number of iteraionss
	unsigned int threadId; //unique thread identifier
	unsigned int batchSize; //process made per itertations
}EnvThreadDataT;

static unsigned int globalThreadCount = 0;
static pthread_t* threadEnv = NULL;
static EnvThreadDataT* threadData = NULL;

void* blocking_routine(void* agrument){
	EnvThreadDataT *tData = (EnvThreadDataT*)agrument; //casting argument to environment thread data type
	ProcessIdT processIds[tData->batchSize]; // holds all process ids

	char message[110]; //message buffer
	snprintf(message, sizeof(message), "Environment thread %u is starting up", tData->threadId);
	logger_write(message);

	//iterration loop
	for(unsigned int i = 0; i < tData->iterations; i++){
		snprintf(message, sizeof(message), "For environment thread %u, iterration %u has begun", tData->threadId, i+1);
		logger_write(message);
		
		//creates processes
		for(unsigned int l = 0; l < tData->batchSize; l++){
			processIds[l] = simulator_create_process(evaluator_blocking_terminates_after(5)); // finishes after 5processe have been simulated
		}

		//waits for process to finish - used a seperate for loop for concurrency reasons - run multiple threads at once
		for(unsigned int k = 0; k < tData->batchSize; k++){
			simulator_wait(processIds[k]);
		}
		snprintf(message, sizeof(message), "Environment thread %u - itteration %u has finished", tData->threadId, i+1);
		logger_write(message);
	}
	return NULL;


}

void environment_start(unsigned int thread_count,
		       unsigned int iterations,
		       unsigned int batch_size) {
				globalThreadCount = thread_count;

				threadEnv = (pthread_t*)checked_malloc(sizeof(pthread_t) * globalThreadCount); //allocating memory for thread environment
				threadData = (EnvThreadDataT*)checked_malloc((sizeof(EnvThreadDataT) * globalThreadCount)); //allocating memory for thread data

				for(unsigned int i = 0; i < globalThreadCount; i++){
					threadData[i].batchSize = batch_size; //assigning batch size within thread with bstch size in arg
					threadData[i].iterations = iterations; //assigning iterations within thread with iterations arg
					threadData[i].threadId = i; // assigning thread id with unique unsigned int

					if(pthread_create(threadEnv, NULL, blocking_routine, &threadData[i]) != 0){ //If thread is unable to be created
						fprintf(stderr, "ERRPR: unable to make an environment thread\n"); //error message
						exit(EXIT_FAILURE); // exxit program
					}

				}


}

void environment_stop() {
	//joins all the threads together
	for(unsigned int i = 0; i < globalThreadCount; i++){
		pthread_join(threadEnv[i], NULL);
	}

	//free memory
	checked_free(threadData);
	checked_free(threadEnv);

	//reset global variables
	threadData = NULL;
	globalThreadCount = 0;
	threadEnv = NULL;

	logger_stop(); //stops logger
}
