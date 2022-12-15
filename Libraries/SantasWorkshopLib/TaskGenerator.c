#include "TaskGenerator.h"
#include <math.h>
#include <stdlib.h>

#define LUT_SIZE 20

int lutTaskIdProb[LUT_SIZE] = 
{
    // %40 probability of TASK_TYPE_1
    TASK_TYPE_1,
    TASK_TYPE_1,
    TASK_TYPE_1,
    TASK_TYPE_1,
    TASK_TYPE_1,
    TASK_TYPE_1,
    TASK_TYPE_1,
    TASK_TYPE_1,

    // %10 more probability of TASK_TYPE_2 for part 2
    TASK_TYPE_1,
    TASK_TYPE_1,

    // %20 probability of TASK_TYPE_2
    TASK_TYPE_2,
    TASK_TYPE_2,
    TASK_TYPE_2,
    TASK_TYPE_2,

    // %20 probability of TASK_TYPE_3
    TASK_TYPE_3,
    TASK_TYPE_3,
    TASK_TYPE_3,
    TASK_TYPE_3,

    // %5 probability of TASK_TYPE_4
    TASK_TYPE_4,

    // %5 probability of TASK_TYPE_5
    TASK_TYPE_5,
};

Task generateTask()
{
    Task task;
    static int taskId = 0;

    task.type = lutTaskIdProb[rand() % LUT_SIZE];
    task.id = taskId++;

    if(task.type == TASK_TYPE_1)
    {
        task.isPaintingDone = 1;
        task.isQualityCheckDone = 1;
        task.isAssemblyDone = 1;
    }

    if(task.type == TASK_TYPE_2)
    {
        task.isPaintingDone = 0;
        task.isQualityCheckDone = 1;
        task.isAssemblyDone = 1;
    }

    if(task.type == TASK_TYPE_3)
    {
        task.isPaintingDone = 1;
        task.isQualityCheckDone = 1;
        task.isAssemblyDone = 0;
    }

    if(task.type == TASK_TYPE_4)
    {
        task.isPaintingDone = 0;
        task.isQualityCheckDone = 0;
        task.isAssemblyDone = 1;
    }

    if(task.type == TASK_TYPE_5)
    {
        task.isPaintingDone = 1;
        task.isQualityCheckDone = 0;
        task.isAssemblyDone = 0;
    }

    pthread_mutex_init(&task.mutexPaint, NULL);
    pthread_mutex_init(&task.mutexAssembly, NULL);
    pthread_mutex_init(&task.mutexQualityCheck, NULL);

    return task;
}