#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <signal.h>
#include <sys/types.h>

#include "GraphicDisplay.h"
#include "MemoryUtilization.h"
#include "CPUUtilization.h"
#include "CoresAndFrequency.h"
#include "SignalHandle.h"
#include "BasicComputation.h"


//Function to compute CPU utilization
void getCPU_Usage(long *T, long *U){
    FILE *f = fopen("/proc/stat", "r");
    long user;
    long nice;
    long sys;
    long idle;
    long IOwait;
    long irq;
    long softirq;
    fscanf(f, "cpu %ld %ld %ld %ld %ld %ld %ld", &user, &nice, &sys, &idle, &IOwait, &irq, &softirq);
    fclose(f);
    long cpuUtilizationTime = user + nice + sys + idle + IOwait + irq + softirq;
    *U = cpuUtilizationTime - idle;
    *T = cpuUtilizationTime;
}