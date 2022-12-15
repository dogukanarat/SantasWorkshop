#ifndef INCLUDED_COMMON_H
#define INCLUDED_COMMON_H

#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#define TRUE  1
#define FALSE 0

// pthread sleeper function
int pthread_sleep (int seconds);

#endif
