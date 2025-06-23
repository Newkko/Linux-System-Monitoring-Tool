#ifndef GraphicDisplay_header
#define GraphicDisplay_header
 
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


//Function to clear the terminal screen
void clearTerminal();
///_|> descry: this function clears the terminal screen
///_|> arg_i: N/A - no parameters
///_|> returning: this function does not return anything

//Function to move the cursor to a specific position
void moveCursor(int column, int row);
///_|> descry: this function moves the cursor to the specified location in the terminal
///_|> column: specifies which column in the terminal to move the cursor to, type int
///_|> row: specifies which row in the terminal to move the cursor to, type int
///_|> returning: this function does not return anything

//Function to graphically display the number of cores
void printSquaresCores(int N);
///_|> descry: this function prints one square for each core the system has
///_|> N: represents the number of cores, type int
///_|> returning: this function does not return anything

//Function to accurately plot memory usage on graph
void plotMemoryUsage(double value, int sample, int maxY);
///_|> descry: this function prints the current memory usage value in the terminal screen
///_|> value: represents the value of the point to plot, type double
///_|> sample: represents the number of values to plot, type int
///_|> maxY: represents the maximum Y value of the memory graph, type int
///_|> returning: this function does not return anything

//Function to accurately plot cpu usage when memory graph is also printed
void plotCPUUsageWithMemory(double point, int sample);
///_|> descry: this function prints the current cpu usage value in the terminal screen when the memory graph is present
///_|> point: represents the value of the point to plot, type double
///_|> sample: represents the number of values to plot, type int
///_|> returning: this function does not return anything

//Function to accurately plot cpu usage
void plotCPUUsage(double point, int sample);
///_|> descry: this function prints the current cpu usage value in the terminal screen when the memory graph is not present
///_|> point: represents the value of the point to plot, type double
///_|> sample: represents the number of values to plot, type int
///_|> returning: this function does not return anything

//Function to print only CPU chart
void printCPUGrid(int maxX);
///_|> descry: this function prints the cpu graph value in the terminal screen when the memory graph is not present
///_|> maxX: represents the value the x axis of the graph has to reach, type int
///_|> returning: this function does not return anything

//Function to graphically display the number of cores when two graphs are displayed
void printSquaresCoresWithTwoGraphs(int N);
///_|> descry: this function prints one square for each core the system has when two graphs are already printed
///_|> N: represents the number of cores, type int
///_|> returning: this function does not return anything

//Function to graphically display the number of cores when only memory graph is displayed
void printSquaresCoresWithOneGraph1(int N);
///_|> descry: this function prints one square for each core the system has when only one graph is already printed.
///_|> N: represents the number of cores, type int
///_|> returning: this function does not return anything

//Function to graphically display the number of cores when only cpu graph is displayed
void printSquaresCoresWithOneGraph2(int N);
///_|> descry: this function prints one square for each core the system has when only one graph is present
///_|> N: represents the number of cores, type int
///_|> returning: this function does not return anything

//Function to graphically display the number of cores when two graphs are displayed
void printSquaresCoresWithTwoGraphs2(int N);
///_|> descry: this function prints one square for each core the system has when two graphs are present
///_|> N: represents the number of cores, type int
///_|> returning: this function does not return anything

//Function to print the memory chart grid
void printMemoryGrid(double maxY, int maxX);
///_|> descry: this function prints the memory graph
///_|> maxY: represents the number the Y axis must reach, type double
///_|> maxX: represents the number the X axis must reach, type int
///_|> returning: this function does not return anything


#endif