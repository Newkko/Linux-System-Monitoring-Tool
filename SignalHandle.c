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
void handlerQuit(int code) {
    (void)code;
    pid_t pgrp = getpgrp();
    fflush(stdout);

    //Stop all processes in the group using custom signal
    if (killpg(pgrp, SIGUSR1) == -1) {
        perror("Error: killpg() cannot send SIGTSTP to process group\n");
    }

    char answer[4];
    printf("Do you want to quit the program? (y/n): ");
    scanf("%2s", answer);

    if (strcmp(answer, "y") == 0 || strcmp(answer, "Y") == 0) {

        //Terminate all processes in the group using SIGTERM
        if (killpg(pgrp, SIGTERM) == -1) {
            perror("Error: killpg() cannot send SIGTERM to process group\n");
        }

        //Exit parent process
        exit(0);
    } 
    else if (strcmp(answer, "n") == 0 || strcmp(answer, "N") == 0) {

        //resume all stopped processes in the group using SIGCONT
        if (killpg(pgrp, SIGCONT) == -1) {
            perror("Error: killpg() cannot send SIGCONT to process group\n");
        }
    } 
    else {
        fprintf(stderr, "Error: Invalid input. Continuing program...\n");

        //resume all stopped processes in the group
        if (killpg(pgrp, SIGCONT) == -1) {
            perror("Error: killpg() cannot send SIGCONT to process group\n");
        }
    }
}

//Signal Handler for SIGUSR1
void CustomHandler(int signum) {
    (void)signum;

    //Temporarily set SIGTSTP to its default action
    struct sigaction temp;
    struct sigaction old_action;

    temp.sa_handler = SIG_DFL;
    sigemptyset(&temp.sa_mask);
    temp.sa_flags = 0;
    if (sigaction(SIGTSTP, &temp, NULL) < 0) {
        perror("Error: sigaction\n");
        exit(1);
    }

    //Save the current action and set to default
    if (sigaction(SIGTSTP, &temp, &old_action) < 0) {
        perror("Error: sigaction\n");
        exit(1);
    }

    raise(SIGTSTP);

    //When we resume, restore SIGTSTP to being ignored
    temp.sa_handler = SIG_IGN;
    if (sigaction(SIGTSTP, &temp, NULL) < 0) {
        perror("Error: sigaction cannot restore\n");
        exit(1);
    }
}