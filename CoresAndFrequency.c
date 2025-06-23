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


//Function to compute the total number of processors the system physcially has
int getNumberOfCores(){
    return sysconf(_SC_NPROCESSORS_CONF);
}



//Function to compute the max frequency of cores
double getMaxFrequencyOfCores() {
    FILE *f = fopen("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq", "r");
    long max;
    if (fscanf(f, "%ld", &max) != 1) {
        fclose(f);
        perror("Error: cannot read max frequency\n");
        exit(1);
    }
    fclose(f);
    return (max / 1e6);
}