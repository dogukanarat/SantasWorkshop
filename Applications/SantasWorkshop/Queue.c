#include "Queue.h"

Queue* queueConstruct(int limit) 
{
    Queue* self = (Queue*)malloc(sizeof(Queue));

    if (self == NULL) 
    {
        return NULL;
    }

    if (limit <= 0) 
    {
        limit = 65535;
    }

    self->limit = limit;
    self->count = 0;
    self->head = NULL;
    self->tail = NULL;

    pthread_mutex_init(&self->mutex, NULL);

    return self;
}

void queueDestruct(Queue* self) 
{
    // bad parameter
    if (self == NULL) 
    {
        return;
    }

    // wait to lock thread
    while( pthread_mutex_lock(&self->mutex) != 0);

    while (!queueIsEmpty(self))
    {
        queueDequeue(self);
    }

    free(self);

    pthread_mutex_unlock(&self->mutex);
}

int queueEnqueue(Queue* self, Task task) 
{
    Node* item = NULL;

    // bad parameter
    if (self == NULL) 
    {
        return FALSE;
    }

    // wait to lock mutex
    while( pthread_mutex_lock(&self->mutex) != 0);

    // the queue is full
    if (self->count == self->limit) 
    {
        pthread_mutex_unlock(&self->mutex);
        return FALSE;
    }

    // create the item
    item = (Node*)malloc(sizeof(Node));
    item->data = task;
    item->next = NULL;
    item->previous = NULL;

    // if the queue is empty, set the head and tail to the item
    if (self->count == 0) 
    {
        self->head = item;
        self->tail = item;
    }
    else 
    {
        // add the item to the tail of the queue
        self->tail->next = item;
        item->previous = self->tail;
        self->tail = item;
    }

    self->count++;

    // unlock mutex
    pthread_mutex_unlock(&self->mutex);

    return TRUE;
}

Task queueDequeue(Queue *self) 
{
    Node* item = NULL;
    Task task;

    // bad parameter
    if (self == NULL) 
    {
        task.id = -1;
        task.type = -1;
        return task;
    }

    // lock mutex
    while( pthread_mutex_lock(&self->mutex) != 0);

    // the queue is empty
    if (self->count == 0) 
    {
        task.id = -1;
        task.type = -1;
        return task;
    }

    // get the item from the head of the queue
    item = self->head;
    task = item->data;

    // remove the item from the queue
    self->head = item->previous;
    self->count--;

    // if the queue is empty, set the tail to NULL
    if (self->count == 0) 
    {
        self->tail = NULL;
    }

    free(item);

    // unlock mutex
    pthread_mutex_unlock(&self->mutex);

    return task;
}

int queueIsEmpty(Queue* self) 
{
    return queueCount(self) == 0;
}

int queueCount(Queue* self)
{
    int count = 0;
    while( pthread_mutex_lock(&self->mutex) != 0);
    count = self->count;
    pthread_mutex_unlock(&self->mutex);
    return count;
}

Task* queueHead(Queue* self)
{
    return &self->head->data;
}