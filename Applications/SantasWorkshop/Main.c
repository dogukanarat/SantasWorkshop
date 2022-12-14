#include "Common.h"
#include "Queue.h"
#include "Print.h"
#include "TaskGenerator.h"
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

// simulation time
static int s_simulationTime = 120;

// seed for randomness
static int s_seed = 10;

// frequency of emergency gift requests from New Zealand
static int s_emergencyFrequency = 30;

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
    // print(
    //     "Request\t\t Painting\t Assembly\t Packaging\t Delivery\t PaintingQa\t AssemblyQa\n"
    // );
}

void printTable()
{
    // print(
    //     "%d\t\t %d\t\t %d\t\t %d\t\t %d\t\t %d\t\t %d\n",
    //     queueCount(s_requestQueue),
    //     queueCount(s_paintingQueue),
    //     queueCount(s_assemblyQueue),
    //     queueCount(s_packagingQueue),
    //     queueCount(s_deliveryQueue),
    //     queueCount(s_paintingQaQueue),
    //     queueCount(s_assemblyQaQueue)
    // );
}

void workPaint() { pthread_sleep(3); }
void workAssembly() { pthread_sleep(2); }
void workPack() { pthread_sleep(1); }
void workDelivery() { pthread_sleep(1); }
void workQa() { pthread_sleep(1); }

void *threadElfA(void *arg)
{
    info("Elf A has started\n");

    while (TRUE)
    {
        if(queueIsEmpty(s_packagingQueue) == FALSE)
        {
            Task task = queueDequeue(s_packagingQueue);
            
            if(task.id != -1)
            {
                workPack();

                queueEnqueue(s_deliveryQueue, task);

                info("Task %d: Packaging -> Delivery\n", task.id);
            }
        }
    }

    return NULL;
}

void *threadElfB(void *arg)
{
    info("Elf B has started\n");

    while (TRUE)
    {
        // if(queueIsEmpty(s_packagingQueue) == FALSE)
        // {
        //     Task task = queueDequeue(s_packagingQueue);
            
        //     if(task.id != -1)
        //     {
        //         workPack();

        //         queueEnqueue(s_deliveryQueue, task);

        //         info("Task %d: Packaging -> Delivery\n", task.id);
        //     }
        // }
    }

    return NULL;
}

void *threadSanta(void *arg)
{
    info("Santa has started\n");

    while (TRUE)
    {
        pthread_sleep(1);
    }

    return NULL;
}

void *threadManager(void *arg)
{
    info("Manager has started\n");

    while (TRUE)
    {
        if(queueIsEmpty(s_requestQueue) == FALSE)
        {
            Task task = queueDequeue(s_requestQueue);

            if(task.type == TASK_TYPE_1)
            {
                queueEnqueue(s_packagingQueue, task);

                info("Task %d: Request -> Packaging\n", task.id);

                continue;
            }

            if(task.type == TASK_TYPE_2)
            {
                queueEnqueue(s_paintingQueue, task);
                
                info("Task %d: Request -> Painting\n", task.id);

                continue;
            }

            if(task.type == TASK_TYPE_3)
            {
                queueEnqueue(s_assemblyQueue, task);

                info("Task %d: Request -> Assembly\n", task.id);

                continue;
            }

            if(task.type == TASK_TYPE_4)
            {
                queueEnqueue(s_paintingQaQueue, task);

                info("Task %d: Request -> PaintingQa\n", task.id);

                continue;
            }

            if(task.type == TASK_TYPE_5)
            {
                queueEnqueue(s_assemblyQaQueue, task);

                info("Task %d: Request -> AssemblyQa\n", task.id);

                continue;
            }
        }
    }

    return NULL;
}

void *threadCustomer(void *arg)
{
    info("Customer has started\n");

    while (TRUE)
    {
        Task generatedTask = generateTask();

        queueEnqueue(s_requestQueue, generatedTask);

        info("Task %d: -> Request\n", generatedTask.id);

        printTable();

        pthread_sleep(1);
    }

    return NULL;
}

int initialize()
{
    // create queues
    s_requestQueue = queueConstruct(100);
    s_paintingQueue = queueConstruct(100);
    s_assemblyQueue = queueConstruct(100);
    s_packagingQueue = queueConstruct(100);
    s_deliveryQueue = queueConstruct(100);
    s_paintingQaQueue = queueConstruct(100);
    s_assemblyQaQueue = queueConstruct(100);

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

    pthread_join(elfA, NULL);
    pthread_join(elfB, NULL);
    pthread_join(santa, NULL);
    pthread_join(manager, NULL);
    pthread_join(customer, NULL);

    return 0;
}