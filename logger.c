// Student : Chidi Ikwunze ID : 20473929
#include "logger.h"
#include "utilities.h"
#include "pthread.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"

static pthread_mutex_t logMutex;
static unsigned int messageCounter = 0; //initialising counter

void logger_start() {
    pthread_mutex_init(&logMutex, NULL);
}

void logger_stop() {
    pthread_mutex_destroy(&logMutex);
}

void logger_write(char const* message) {
    //Grabing mutex
    pthread_mutex_lock(&logMutex);

    //Used unsigned int to ensure that the message id and the message counter can never be a negative number
    unsigned int messageId = messageCounter ++;

    char timeZoneBuffer[9]; // Correct formatting buffer
    time_t currentTime = time(NULL); //getting current time
    struct tm* localTime = localtime(&currentTime); // converting time to local time zone
    strftime(timeZoneBuffer, sizeof(timeZoneBuffer), "%H:%M:%S", localTime);

    //prints out the message with its ID and timezone
    printf("%u : %s : %s\n" , messageId, timeZoneBuffer, message);

    //Let mutex go
    pthread_mutex_unlock(&logMutex);



}
