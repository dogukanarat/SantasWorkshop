#ifndef INCLUDED_TASKGENERATOR_H
#define INCLUDED_TASKGENERATOR_H

#include "Queue.h"

typedef enum 
{
    TASK_TYPE_1 = 0,
    TASK_TYPE_2,
    TASK_TYPE_3,
    TASK_TYPE_4,
    TASK_TYPE_5,
    TASK_TYPE_COUNT,
} TaskType;

Task generateTask();

#endif