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


//Function to get the total memory available on the system
int get_total_memory_gb() {
    long p = sysconf(_SC_PHYS_PAGES);
    long p_size = sysconf(_SC_PAGE_SIZE);
    if (p == -1 || p_size == -1) {
        perror("sysconf");
        return 1.0;
    }
    return p * p_size / (1024 * 1024 * 1024);
}

//Function to get memory usage
void getMemoryUsage(double *x, double *y) {
    FILE *f = fopen("/proc/meminfo", "r");
    if (!f) {
        printf("Failed to open the /proc/meminfo");
        return;
    }

    char line[256];
    double memoryTotal = 0.0, memoryAvailable = 0.0;

    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "MemTotal:", 9) == 0) {
            sscanf(line + 9, "%lf", &memoryTotal);
        } else if (strncmp(line, "MemAvailable:", 13) == 0) {
            sscanf(line + 13, "%lf", &memoryAvailable);
        }
        if (memoryTotal > 0 && memoryAvailable > 0) {
            break;
        }
    }
    fclose(f);

    memoryTotal = get_total_memory_gb();
    memoryAvailable = memoryAvailable / (1024 * 1024);
    *x = memoryTotal;
    *y = memoryTotal - memoryAvailable;
}