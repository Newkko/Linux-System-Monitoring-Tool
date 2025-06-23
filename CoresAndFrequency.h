#ifndef CoresAndFrequency_header
#define CoresAndFrequency_header
 
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
int getNumberOfCores();
///_|> descry: this function computes the current number of cores the system has
///_|> arg_i: N/A - no parameters
///_|> returning: this function returns an int representing the number of cores



//Function to compute the max frequency of cores
double getMaxFrequencyOfCores();
///_|> descry: this function computes the maximum frequency of cores the system has
///_|> arg_i: N/A - no parameters
///_|> returning: this function returns a double representing the maximum frequency of cores


#endif