#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>

cpu_set_t cpuset;
#define MAX_CPU_COUNT 4

void *thread_func1()
{
    // show process PID
    printf("[Nebula-Thread-1]: Child process PID: %d, Parent Process PID %d\n", getpid(), getppid());
    // show logical CPU info
    printf("[Nebula-Thread-1]: Logical CPU info: %d\n", sched_getcpu());
    // show scheduling algorithm's information
    // if sched_getscheduler(getpid()) == SCHED_FIFO return 0 then RR else FIFO
    printf("[Nebula-Thread-1]: Scheduling algorithm: %s\n", sched_getscheduler(getpid()) == SCHED_FIFO ? "FIFO" : "RR");
    printf("\n\n");

    return NULL;
}

void *thread_func2()
{
    // show process PID
    printf("[Nebula-Thread-2]: Child process PID: %d, Parent Process PID %d\n", getpid(), getppid());
    // show logical CPU info
    printf("[Nebula-Thread-2]: Logical CPU info: %d\n", sched_getcpu());
    // show scheduling algorithm's information
    // if sched_getscheduler(getpid()) == SCHED_FIFO return 0 then RR else FIFO
    printf("[Nebula-Thread-2]: Scheduling algorithm: %s\n", sched_getscheduler(getpid()) == SCHED_FIFO ? "FIFO" : "RR");
    printf("\n\n");

    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t X, Y;

    for (int i = 0; i < 10; i++)
    {
        pthread_create(&X, NULL, thread_func1, NULL);
        pthread_create(&Y, NULL, thread_func2, NULL);

        CPU_ZERO(&cpuset);
        CPU_SET(i % MAX_CPU_COUNT, &cpuset);
        sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);

        struct sched_param sp = {.sched_priority = 80};
        int ret = sched_setscheduler(0, SCHED_FIFO, &sp);
        printf("[DEBUG]: %d\n", ret);
    }

    pthread_join(X, NULL);
    pthread_join(Y, NULL);

    printf("[Nebula]: Main completed. Exiting.\n");
    return 0;
}
