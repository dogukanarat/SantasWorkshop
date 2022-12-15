#include "SantasWorkshopLib/Common.h"
#include "SantasWorkshopLib/Queue.h"
#include "SantasWorkshopLib/Print.h"
#include "SantasWorkshopLib/TaskGenerator.h"
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define TABLE_VIEW 1
#define QUEUE_SIZE 200

// simulation time
static int s_simulationTime = 50;

// seed for randomness
static int s_seed = 10;

// frequency of emergency gift requests from New Zealand
static int s_emergencyFrequency = 30;

static int s_currentTime = 0;

static int s_isRunning = TRUE;

// list of queues
static Queue *s_requestQueue;
static Queue *s_paintingQueue;
static Queue *s_assemblyQueue;
static Queue *s_packagingQueue;
static Queue *s_deliveryQueue;
static Queue *s_paintingQaQueue;
static Queue *s_assemblyQaQueue;

void printTableHeader()
{
#if TABLE_VIEW
    print(
        "Time\tRequest\t\t Painting\t Assembly\t Packaging\t Delivery\t PaintingQa\t AssemblyQa\n"
    );
#endif
}

void printTable()
{
#if TABLE_VIEW
    print(
        "%d\t%d\t\t %d\t\t %d\t\t %d\t\t %d\t\t %d\t\t %d\n",
        s_currentTime,
        queueCount(s_requestQueue),
        queueCount(s_paintingQueue),
        queueCount(s_assemblyQueue),
        queueCount(s_packagingQueue),
        queueCount(s_deliveryQueue),
        queueCount(s_paintingQaQueue),
        queueCount(s_assemblyQaQueue)
    );
#endif
}

void waitForPaint() { pthread_sleep(3); }
void waitForAssembly() { pthread_sleep(2); }
void waitForPack() { pthread_sleep(1); }
void waitForDelivery() { pthread_sleep(1); }
void waitForQa() { pthread_sleep(1); }

void handlePaint()
{
    if(queueIsEmpty(s_paintingQueue) == FALSE)
    {
        Task task = queueDequeue(s_paintingQueue);
        
        if(task.id != -1)
        {
            waitForPaint();

            queueEnqueue(s_packagingQueue, task);

            info("Task %d: Painting -> Packing (Elf A)\n", task.id);
        }
    }
}

void handlePack()
{
    if(queueIsEmpty(s_packagingQueue) == FALSE)
    {
        Task task = queueDequeue(s_packagingQueue);
        
        if(task.id != -1)
        {
            waitForPack();

            queueEnqueue(s_deliveryQueue, task);

            info("Task %d: Packaging -> Delivery (Elf A)\n", task.id);
        }
    }
}

void handleAssembly()
{
    if(queueIsEmpty(s_assemblyQueue) == FALSE)
    {
        Task task = queueDequeue(s_assemblyQueue);
        
        if(task.id != -1)
        {
            waitForAssembly();

            queueEnqueue(s_packagingQueue, task);

            info("Task %d: Assembly -> Packing (Elf B)\n", task.id);
        }
    }
}

void handleDelivery()
{
    if(queueIsEmpty(s_deliveryQueue) == FALSE)
    {
        Task task = queueDequeue(s_deliveryQueue);

        if(task.id != -1)
        {
            waitForDelivery();

            info("Task %d: Delivery -> \n", task.id);
        }
    }
}

void *threadElfA(void *arg)
{
    info("Elf A has started\n");

    while (s_isRunning)
    {
        // elf a can paint and pack
        handlePack();
        handlePaint();
    }

    return NULL;
}

void *threadElfB(void *arg)
{
    info("Elf B has started\n");

    while (s_isRunning)
    {
        // elf b can assemble and pack
        handlePack();
        handleAssembly();
    }

    return NULL;
}

void *threadSanta(void *arg)
{
    info("Santa has started\n");

    while (s_isRunning)
    {
        handleDelivery();
    }

    return NULL;
}

void *threadManager(void *arg)
{
    info("Manager has started\n");

    while (s_isRunning)
    {
        if(queueIsEmpty(s_requestQueue) == FALSE)
        {
            Task task = queueDequeue(s_requestQueue);

            if(task.type == TASK_TYPE_1)
            {
                queueEnqueue(s_packagingQueue, task);

                info("Task %d: Request -> Packaging\n", task.id);
            }

            if(task.type == TASK_TYPE_2)
            {
                queueEnqueue(s_paintingQueue, task);
                
                info("Task %d: Request -> Painting\n", task.id);
            }

            if(task.type == TASK_TYPE_3)
            {
                queueEnqueue(s_assemblyQueue, task);

                info("Task %d: Request -> Assembly\n", task.id);
            }

            if(task.type == TASK_TYPE_4)
            {
                queueEnqueue(s_paintingQaQueue, task);

                info("Task %d: Request -> PaintingQa\n", task.id);
            }

            if(task.type == TASK_TYPE_5)
            {
                queueEnqueue(s_assemblyQaQueue, task);

                info("Task %d: Request -> AssemblyQa\n", task.id);
            }

            printTable();
        }
    }

    return NULL;
}

void *threadCustomer(void *arg)
{
    info("Customer has started\n");

    while (s_isRunning)
    {
        Task generatedTask = generateTask();

        queueEnqueue(s_requestQueue, generatedTask);

        info("Task %d: -> Request\n", generatedTask.id);

        pthread_sleep(1);
    }

    return NULL;
}

int initialize()
{
    // create queuesQUEUE_SIZE
    s_requestQueue = queueConstruct(QUEUE_SIZE);
    s_paintingQueue = queueConstruct(QUEUE_SIZE);
    s_assemblyQueue = queueConstruct(QUEUE_SIZE);
    s_packagingQueue = queueConstruct(QUEUE_SIZE);
    s_deliveryQueue = queueConstruct(QUEUE_SIZE);
    s_paintingQaQueue = queueConstruct(QUEUE_SIZE);
    s_assemblyQaQueue = queueConstruct(QUEUE_SIZE);

    return TRUE;
}

int main(int argc, char **argv)
{
    // -t (int) => simulation time in seconds
    // -s (int) => change the random seed
    for(int i = 1; i < argc; i++)
    {
        if(!strcmp(argv[i], "-t")) {s_simulationTime = atoi(argv[++i]);}
        if(!strcmp(argv[i], "-s")) {s_seed = atoi(argv[++i]);}
    }
    
    srand(s_seed); // feed the seed

    pthread_t elfA;
    pthread_t elfB;
    pthread_t santa;
    pthread_t manager;
    pthread_t customer;

    int result = 0;

    print("Santas Workshop\n");

    initialize();

    printTableHeader();

    // create threads
    pthread_create(&elfA, NULL, threadElfA, NULL);
    pthread_create(&elfB, NULL, threadElfB, NULL);
    pthread_create(&santa, NULL, threadSanta, NULL);
    pthread_create(&manager, NULL, threadManager, NULL);
    pthread_create(&customer, NULL, threadCustomer, NULL);

    while(s_simulationTime-- > 0)
    {
        pthread_sleep(1);
        s_currentTime++;
    }

    s_isRunning = FALSE;

    pthread_join(elfA, NULL);
    pthread_join(elfB, NULL);
    pthread_join(santa, NULL);
    pthread_join(manager, NULL);
    pthread_join(customer, NULL);

    print("Simulation is finished\n");

    return 0;
}