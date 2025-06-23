#ifndef SignalHandle_header
#define SignalHandle_header
 
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


//Signal handler for SIGINT
void handlerQuit(int code);
///_|> descry: this function handles the signal control+C and terminates the program if the user confirms their choice
///_|> code: void and unused variable, type integer
///_|> returning: this function does not return anything

void CustomHandler(int signum);
///_|> descry: this function handles the signal control+Z and does nothing (ignores the signal)
///_|> signum: void and unused variable, type integer
///_|> returning: this function does not return anything


#endif