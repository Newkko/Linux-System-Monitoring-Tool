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
void clearTerminal() {
    printf("\033[2J\033[H");
}

//Function to move the cursor to a specific position
void moveCursor(int column, int row) {
    printf("\033[%d;%dH", row, column);
}

//Function to graphically display the number of cores
void printSquaresCores(int N) {
    int c = 0;
    int temp = 1;
    for (int i = 1; i <= N; i++) {
        printf("+-----+ ");
        c++;
        if (c == 4 || i == N) {
            printf("\n");
            moveCursor(1, 4 + temp);
            temp++;
            for (int j = 0; j < c; j++) {
                printf("|     | ");
            }
            printf("\n");
            moveCursor(1, 4 + temp);
            temp++;
            for (int j = 0; j < c; j++) {
                printf("|     | ");
            }
            printf("\n");
            moveCursor(1, 4 + temp);
            temp++;
            for (int j = 0; j < c; j++) {
                printf("+-----+ ");
            }
            printf("\n");
            moveCursor(1, 4 + temp);
            temp++;
            c = 0;
        }
    }
}

//Function to accurately plot memory usage on graph
void plotMemoryUsage(double value, int sample, int maxY){
    double step = maxY / 12.0;
    if (value <= 0){
        moveCursor(9 + sample,16);
        printf("#");
    }
    else if (value > 0 && value <= step){
        moveCursor(9 + sample,15);
        printf("#");
    }
    else if (value > step && value <= step * 2){
        moveCursor(9 + sample,14);
        printf("#");
    }
    else if (value > step * 2 && value <= step * 3){
        moveCursor(9 + sample,13);
        printf("#");
    }
    else if (value > step * 3 && value <= step * 4){
        moveCursor(9 + sample,12);
        printf("#");
    }
    else if (value > step * 4 && value <= step * 5){
        moveCursor(9 + sample,11);
        printf("#");
    }
    else if (value > step * 5 && value <= step * 6){
        moveCursor(9 + sample,10);
        printf("#");
    }
    else if (value > step * 6 && value <= step * 7){
        moveCursor(9 + sample,9);
        printf("#");
    }
    else if (value > step * 7 && value <= step * 8){
        moveCursor(9 + sample,8);
        printf("#");
    }
    else if (value > step * 8 && value <= step * 9){
        moveCursor(9 + sample,7);
        printf("#");
    }
    else if (value > step * 9 && value <= step * 10){
        moveCursor(9 + sample,6);
        printf("#");
    }
    else if (value > step * 10 && value <= step * 11){
        moveCursor(9 + sample,5);
        printf("#");
    }
    else if ((value > step * 11 && value <= step * 12) || (value >= step * 12)){
        moveCursor(9 + sample,4);
        printf("#");
    }
}

//Function to accurately plot cpu usage when memory graph is also printed
void plotCPUUsageWithMemory(double point, int sample){
    if (point <= 0){
        moveCursor(9 + sample,29);
        printf(":");
    }
    else if (point > 0 && point <= 10){
        moveCursor(9 + sample,28);
        printf(":");
    }
    else if (point > 10 && point <= 20){
        moveCursor(9 + sample,27);
        printf(":");
    }
    else if (point > 20 && point <= 30){
        moveCursor(9 + sample,26);
        printf(":");
    }
    else if (point > 30 && point <= 40){
        moveCursor(9 + sample,25);
        printf(":");
    }
    else if (point > 40 && point <= 50){
        moveCursor(9 + sample,24);
        printf(":");
    }
    else if (point > 50 && point <= 60){
        moveCursor(9 + sample,23);
        printf(":");
    }
    else if (point > 60 && point <= 70){
        moveCursor(9 + sample,22);
        printf(":");
    }
    else if (point > 70 && point <= 80){
        moveCursor(9 + sample,21);
        printf(":");
    }
    else if (point > 80 && point <= 90){
        moveCursor(9 + sample,20);
        printf(":");
    }
    else if (point > 90){
        moveCursor(9 + sample,19);
        printf(":");
    }
}

//Function to accurately plot cpu usage
void plotCPUUsage(double point, int sample){
    if (point <= 0){
        moveCursor(9 + sample,14);
        printf(":");
    }
    else if (point > 0 && point <= 10){
        moveCursor(9 + sample,13);
        printf(":");
    }
    else if (point > 10 && point <= 20){
        moveCursor(9 + sample,12);
        printf(":");
    }
    else if (point > 20 && point <= 30){
        moveCursor(9 + sample,11);
        printf(":");
    }
    else if (point > 30 && point <= 40){
        moveCursor(9 + sample,10);
        printf(":");
    }
    else if (point > 40 && point <= 50){
        moveCursor(9 + sample,9);
        printf(":");
    }
    else if (point > 50 && point <= 60){
        moveCursor(9 + sample,8);
        printf(":");
    }
    else if (point > 60 && point <= 70){
        moveCursor(9 + sample,7);
        printf(":");
    }
    else if (point > 70 && point <= 80){
        moveCursor(9 + sample,6);
        printf(":");
    }
    else if (point > 80 && point <= 90){
        moveCursor(9 + sample,5);
        printf(":");
    }
    else if (point > 90){
        moveCursor(9 + sample,4);
        printf(":");
    }
}

//Function to print only CPU chart
void printCPUGrid(int maxX) {
    int yIncrements = 9;
    double yStep = 100 / yIncrements;

    for (int y = 0; y <= yIncrements; ++y) {
        int gridY = yIncrements - y;
        double yValue = gridY * yStep;

        //Print Y-axis labels
        if (yValue == 0 || yValue == 100) {
            printf("       |");
        } else {
            printf("       |");
        }

        printf("\n");
    }

    //Print the X-axis
    printf("        -");
    for (int x = 0; x < maxX; ++x) {
        printf("-");
    }
    printf("\n");
}

//Function to graphically display the number of cores when two graphs are displayed
void printSquaresCoresWithTwoGraphs(int N) {
    int c = 0;
    int temp = 1;
    for (int i = 1; i <= N; i++) {
        printf("+-----+ ");
        c++;
        if (c == 4 || i == N) {
            printf("\n");
            moveCursor(1, 32 + temp);
            temp++;
            for (int j = 0; j < c; j++) {
                printf("|     | ");
            }
            printf("\n");
            moveCursor(1, 32 + temp);
            temp++;
            for (int j = 0; j < c; j++) {
                printf("|     | ");
            }
            printf("\n");
            moveCursor(1, 32 + temp);
            temp++;
            for (int j = 0; j < c; j++) {
                printf("+-----+ ");
            }
            printf("\n");
            moveCursor(1, 32 + temp);
            temp++;
            c = 0;
        }
    }
}

//Function to graphically display the number of cores when only memory graph is displayed
void printSquaresCoresWithOneGraph1(int N) {
    int c = 0;
    int temp = 1;
    for (int i = 1; i <= N; i++) {
        printf("+-----+ ");
        c++;
        if (c == 4 || i == N) {
            printf("\n");
            moveCursor(1, 19 + temp);
            temp++;
            for (int j = 0; j < c; j++) {
                printf("|     | ");
            }
            printf("\n");
            moveCursor(1, 19 + temp);
            temp++;
            for (int j = 0; j < c; j++) {
                printf("|     | ");
            }
            printf("\n");
            moveCursor(1, 19 + temp);
            temp++;
            for (int j = 0; j < c; j++) {
                printf("+-----+ ");
            }
            printf("\n");
            moveCursor(1, 19 + temp);
            temp++;
            c = 0;
        }
    }
}

//Function to graphically display the number of cores when only cpu graph is displayed
void printSquaresCoresWithOneGraph2(int N) {
    int c = 0;
    int temp = 1;
    for (int i = 1; i <= N; i++) {
        printf("+-----+ ");
        c++;
        if (c == 4 || i == N) {
            printf("\n");
            moveCursor(1, 17 + temp);
            temp++;
            for (int j = 0; j < c; j++) {
                printf("|     | ");
            }
            printf("\n");
            moveCursor(1, 17 + temp);
            temp++;
            for (int j = 0; j < c; j++) {
                printf("|     | ");
            }
            printf("\n");
            moveCursor(1, 17 + temp);
            temp++;
            for (int j = 0; j < c; j++) {
                printf("+-----+ ");
            }
            printf("\n");
            moveCursor(1, 17 + temp);
            temp++;
            c = 0;
        }
    }
}

//Function to graphically display the number of cores when two graphs are displayed
void printSquaresCoresWithTwoGraphs2(int N) {
    int c = 0;
    int temp = 1;
    for (int i = 1; i <= N; i++) {
        printf("+-----+ ");
        c++;
        if (c == 4 || i == N) {
            printf("\n");
            moveCursor(1, 32 + temp);
            temp++;
            for (int j = 0; j < c; j++) {
                printf("|     | ");
            }
            printf("\n");
            moveCursor(1, 32 + temp);
            temp++;
            for (int j = 0; j < c; j++) {
                printf("|     | ");
            }
            printf("\n");
            moveCursor(1, 32 + temp);
            temp++;
            for (int j = 0; j < c; j++) {
                printf("+-----+ ");
            }
            printf("\n");
            moveCursor(1, 32 + temp);
            temp++;
            c = 0;
        }
    }
}

//Function to print the memory chart grid
void printMemoryGrid(double maxY, int maxX) {
    int yIncrements = 11;
    double yStep = maxY / yIncrements;

    for (int y = 0; y <= yIncrements; ++y) {
        int gridY = yIncrements - y;
        double yValue = gridY * yStep;

        //Print Y-axis labels
        if (yValue == 0){
            printf("       |");
        } else {
            printf("       |");
        }

        printf("\n");
    }

    //Print the X-axis
    printf("        -");
    for (int x = 0; x < maxX; ++x) {
        printf("-");
    }
    printf("\n");
}