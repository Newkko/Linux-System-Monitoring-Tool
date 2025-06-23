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


//Function to convert microseconds to seconds
double convertToSeconds(int x){
    double seconds = x / 1000000.0;
    return seconds;
}

//Function to check if an argument is a string representation of a valid int, and if so then convert it into an int
int isValidInt(const char *x) {
    char *endptr;
    long value = strtol(x, &endptr, 10);
    if ((*endptr != '\0') || (value < INT_MIN) || (value > INT_MAX)) {
        return -1;
    }
    return (int)value;
}