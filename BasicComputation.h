#ifndef BasicComputation_header
#define BasicComputation_header

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
double convertToSeconds(int x);
///_|> descry: this function converts microseconds to seconds
///_|> x: represents the microseconds amount, type int
///_|> returning: this function returns a double representing seconds

//Function to check if an argument is a string representation of a valid int, and if so then convert it into an int
int isValidInt(const char *x);
///_|> descry: this function checks if the input is a valid int and converts it from a char to an int if it is valid
///_|> x: represents the potential representation of an int in char format, type constant char pointer
///_|> returning: this function returns an int representing the converted int


#endif