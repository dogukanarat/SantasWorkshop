#include "Print.h"
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>

/*
    Print a message to the console
    @param message the message to print
*/
void print(const char* format, ...)
{
    static pthread_mutex_t printMutex = PTHREAD_MUTEX_INITIALIZER;

    va_list args;
    va_start(args, format);
    pthread_mutex_lock(&printMutex);
    vprintf(format, args);
    pthread_mutex_unlock(&printMutex);
    va_end(args);
}

/*
    Print an info message to the console
    @param message the message to print
*/
void info(const char* format, ...)
{
#if PRINT_INFO_ENABLED
    static pthread_mutex_t printMutex = PTHREAD_MUTEX_INITIALIZER;

    va_list args;
    va_start(args, format);
    pthread_mutex_lock(&printMutex);
    printf("INFO: ");
    vprintf(format, args);
    pthread_mutex_unlock(&printMutex);
    va_end(args);
#endif
}