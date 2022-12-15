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
        // set the tail's next to the item
        self->tail->next = item;
        item->previous = self->tail;
        self->tail = item;
    }

    // increment the count
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

    // if the queue is not empty, set the head to the next item
    if (self->count > 1) 
    {
        self->head = item->next;
        self->head->previous = NULL;
    }
    else 
    {
        self->head = NULL;
        self->tail = NULL;
    }

    // decrement the count
    self->count--;

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

Task* queuePeek(Queue* self, int index)
{
    Node* item = NULL;
    int i = 0;

    // bad parameter
    if (self == NULL) 
    {
        return NULL;
    }

    // lock mutex
    while( pthread_mutex_lock(&self->mutex) != 0);

    // the queue is empty
    if (self->count == 0) 
    {
        pthread_mutex_unlock(&self->mutex);
        return NULL;
    }

    // get the item from the head of the queue
    item = self->head;

    // if the queue is not empty, set the head to the next item
    if (self->count > 1) 
    {
        for (i = 0; i < index; i++)
        {
            item = item->next;
        }
    }

    // unlock mutex
    pthread_mutex_unlock(&self->mutex);

    return &item->data;
}