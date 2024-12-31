#include "event_source.h"
#include "utilities.h"
#include "simulator.h"
#include "pthread.h"
#include "stdio.h"
#include "logger.h"

static pthread_t eventThread = NULL;
static unsigned int evInternalMicroSec = 0;
static unsigned int isRunningEvent = 0;

void* event_source_routine(void* arg){
    while(isRunningEvent == 1){
            usleep(evInternalMicroSec);
            simulator_event();
    }
    return NULL;
}

void event_source_start(useconds_t interval) {
    evInternalMicroSec = interval;
    isRunningEvent = 1; // setting running flag to 1 which means that it is running;
 
    if(pthread_create(&eventThread, NULL, event_source_routine, NULL) != 0){
            fprintf(stderr, "ERROR: unable to create event source\n");
            exit(2);
    }
    logger_write("Event source has begun");
    
}

void event_source_stop() {
    isRunningEvent = 0; // setting flag to 0 which mean that it isnt running anymore
    
    if(pthread_join(eventThread, NULL) != 0){
        fprintf(stderr, "ERROR: unable to join event thread\n");
        exit(EXIT_FAILURE);
    }
    logger_write("Event thread has stopped");
}

