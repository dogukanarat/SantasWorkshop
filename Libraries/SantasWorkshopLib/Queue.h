#ifndef INCLUDED_QUEUE_H
#define INCLUDED_QUEUE_H

/*
    Queue.h
    A thread safe queue implementation
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include "Common.h"

/*
    A Task, holds the ID of the task and the type of the task
*/
typedef struct 
{
    int id;
    int type;
    int isPaintingDone;
    int isQualityCheckDone;
    int isAssemblyDone;
    pthread_mutex_t mutexPaint;
    pthread_mutex_t mutexQualityCheck;
    pthread_mutex_t mutexAssembly;
} Task;

/*
    A Node, holds the data and the next and previous nodes
*/
typedef struct NodeStruct 
{
    Task data;
    struct NodeStruct* next;
    struct NodeStruct* previous;
} Node;

/*
    A Queue, holds the head and tail of the queue, the number of items in the queue, and the limit of the queue
*/
typedef struct Queue 
{
    Node* head;
    Node* tail;
    int count;
    int limit;
    pthread_mutex_t mutex;
} Queue;

/*
    Construct a new Queue
    @param limit the maximum amount of nodes that can be in the queue
    @return a pointer to the new Queue
*/
Queue* queueConstruct(int limit);

/*
    Destruct a Queue
    @param self the Queue to destruct
*/
void queueDestruct(Queue* self);

/*
    Enqueue a Task to the Queue
    @param self the Queue to enqueue to
    @param task the Task to enqueue
    @return TRUE if the enqueue was successful, FALSE otherwise
*/
int queueEnqueue(Queue* self, Task task);

/*
    Dequeue a Task from the Queue
    @param self the Queue to dequeue from
    @return the Task that was dequeued
*/
Task queueDequeue(Queue* self);

/*
    Check if the Queue is empty
    @param self the Queue to check
    @return TRUE if the Queue is empty, FALSE otherwise
*/
int queueIsEmpty(Queue* self);

/*
    return the number of items in the Queue
    @param self the Queue to check
    @return the number of items in the Queue
*/
int queueCount(Queue* self);

/*
    Get the Task at the given index
    @param self the Queue to get the Task from
    @param index the index of the Task to get
    @return the Task at the given index
*/
Task* queuePeek(Queue* self, int index);



#endif