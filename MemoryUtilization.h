#ifndef MemoryUtilization_header
#define MemoryUtilization_header

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
int get_total_memory_gb();
///_|> descry: this function computes the maximum memory available on the currently running system
///_|> arg_i: N/A - there are no parameters
///_|> returning: this function does not return anything

//Function to get memory usage
void getMemoryUsage(double *x, double *y);
///_|> descry: this function computes the current memory usage of the running system
///_|> x: holds a value needed for computation, type doble pointer
///_|> y: holds a value needed for computation, type doble pointer
///_|> returning: this function does not return anything

#endif