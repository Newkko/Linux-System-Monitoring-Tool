#ifndef CPUUtilization_header
#define CPUUtilization_header
 
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
void getCPU_Usage(long *T, long *U);
///_|> descry: this function computes the current cpu usage of the system
///_|> T: stores a value needed for computation, type long pointer
///_|> U: stores a value needed for computation, type long pointer
///_|> returning: this function does not return anything


#endif