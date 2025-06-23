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



int main(int argc, char **argv){

    //Set up signal handling
    struct sigaction newact1;
    struct sigaction newact2;

    newact1.sa_handler = handlerQuit;
    newact1.sa_flags = 0;
    sigemptyset(&newact1.sa_mask);
    sigaction(SIGINT, &newact1, NULL);

    newact2.sa_handler = SIG_IGN;
    newact2.sa_flags = 0;
    sigemptyset(&newact2.sa_mask);
    sigaction(SIGTSTP, &newact2, NULL);


    sigaction(SIGUSR1, &newact2, NULL);

    //Set up default values
    int samples = 20;
    int tdelay = 500000;
    double seconds = convertToSeconds(tdelay);
    
    //Case: No arguments are passed
    if (argc == 1){
        
        //Print the number of samples and time delay to user
        clearTerminal();
        printf("Nbr of samples: %d -- every %d microSecs (%.3f secs)\n", samples, tdelay, seconds);
        printf("\n");

        //Create a pipe for --memory calculation
        int pipeMemory[2];
        if (pipe(pipeMemory) == -1){
            perror("Error: Could not create pipe\n");
            exit(1);
        }

        //Create the child1 process that will computer memory utilization
        int child1 = fork();

        //Check if child process was successfully created
        if (child1 == -1){
            perror("Error: Could not create child process\n");
            close(pipeMemory[0]);
            close(pipeMemory[1]);
            exit(1);
        }
        
        

        //Set the child1 process to compute memory utilization and write to parent
        if (child1 == 0){

            sigaction(SIGTSTP, &newact2, NULL);
            sigaction(SIGINT, &newact2, NULL);

            //Set up SIGUSR1 handler
            struct sigaction c;
            c.sa_handler = CustomHandler;
            sigemptyset(&c.sa_mask);
            c.sa_flags = 0;
            sigaction(SIGUSR1, &c, NULL);

            //Close the read end of the pipe
            close(pipeMemory[0]);

            ssize_t check = 1;
            for (int j = 0; (j < samples) && (check > 0); j++){
                double totalMemoryGB = 0.0;
                double memoryUsedGB = 0.0;
                getMemoryUsage(&totalMemoryGB, &memoryUsedGB);
                check = write(pipeMemory[1], &memoryUsedGB, sizeof(memoryUsedGB));

                //Wait tdelay specified amount before computing next value
                //sleep(seconds);
                usleep(tdelay);
            }

            //child1 process has finished computing memory and will now terminate
            close(pipeMemory[1]);
            exit(0);
        }

        //Continue with the parent process
        else if (child1 > 0){

            //Close the writing end pipe of the memory process
            close(pipeMemory[1]);
            
            //Create a pipe for --cpu utilization computation
            int pipeCPU[2];

            //Error check to see if pipe was successfully created
            if (pipe(pipeCPU) == -1){
                perror("Error: Could not create pipe\n");
                exit(1);
            }

            //Create child2 process for CPU utilization computation
            int child2 = fork();

            //Check if child process was successfully created
            if (child2 == -1){
                perror("Error: Could not create child process\n");
                close(pipeCPU[0]);
                close(pipeCPU[1]);
                exit(1);
            }

            

            //Set the child2 process to compute cpu utilization
            if (child2 == 0){

                sigaction(SIGTSTP, &newact2, NULL);
                sigaction(SIGINT, &newact2, NULL);

                //Set up SIGUSR1 handler
                struct sigaction c;
                c.sa_handler = CustomHandler;
                sigemptyset(&c.sa_mask);
                c.sa_flags = 0;
                sigaction(SIGUSR1, &c, NULL);

                //Close the reading end of the pipe
                close(pipeCPU[0]);

                ssize_t check = 1;
                for (int j = 0; (j < samples) && (check > 0); j++){
                    long t1;
                    long t2;
                    long u1;
                    long u2;
                    getCPU_Usage(&t1, &u1);
                    //sleep(seconds);
                    usleep(tdelay);
                    getCPU_Usage(&t2, &u2);
                    int numerator = u2 - u1;
                    int denominator = t2 - t1;
                    double cpuUsage = ((double)numerator / denominator) * 100;
                    if ((numerator <=0) || (denominator <= 0) || (cpuUsage <=0)){
                        cpuUsage = 0.00;
                    }
                    check = write(pipeCPU[1], &cpuUsage, sizeof(cpuUsage));
                }
                
                //child1 process has finished computing memory and will now terminate
                close(pipeCPU[1]);
                exit(0);
            }

            //Continue with the parent process
            else if (child2 >0){
                
                //Create a pipe for cores and frequency calculations
                int pipeCores[2];

                //Check if pipe was successfully created
                if (pipe(pipeCores) == -1){
                    perror("Error: Could not create pipe\n");
                    exit(1);
                }

                //Create child3 process for cores and freuqency calculations
                int child3 = fork();

                //Check if child3 process was successfully created
                if (child3 == -1){
                    perror("Error: Could not create child process\n");
                    close(pipeCores[0]);
                    close(pipeCores[0]);
                    exit(1);
                }

                

                //Set child3 process to compute cores and frequency calculations
                if (child3 == 0){

                    sigaction(SIGTSTP, &newact2, NULL);
                    sigaction(SIGINT, &newact2, NULL);

                    //Set up SIGUSR1 handler
                    struct sigaction c;
                    c.sa_handler = CustomHandler;
                    sigemptyset(&c.sa_mask);
                    c.sa_flags = 0;
                    sigaction(SIGUSR1, &c, NULL);

                    //Close the reading end of the pipe
                    close(pipeCores[0]);

                    //Compute cores and frequency calculations
                    int cores = getNumberOfCores();
                    double maxFrequency = getMaxFrequencyOfCores();
                    ssize_t check = 1;
                    int loop = 1;
                    while (check > 0 && loop<3){
                        if (loop == 1){
                            check = write(pipeCores[1], &cores, sizeof(cores));
                        }
                        else if (loop ==2){
                            check = write(pipeCores[1], &maxFrequency, sizeof(maxFrequency));
                        }
                        loop++;
                    }

                    //Child3 process is done and so it will terminate
                    close(pipeCores[1]);
                    exit(0);
                }

                //Continue with parent process
                else if (child3 > 0){
                    
                    //Now at this point we have set up all the processes and they will close their ends of the pipes
                    //and terminate themselves

                    //Print the memory graph
                    printf("  Memory\n");
                    double maxYValue = get_total_memory_gb();
                    printMemoryGrid(maxYValue, samples);
                    moveCursor(3,4);
                    printf("%d GB", (int)maxYValue);
                    moveCursor(4,16);
                    printf("0 GB");
                    moveCursor(1,19);

                    //Print the cpu graph
                    printCPUGrid(samples);
                    printf("\n");
                    moveCursor(6,18);
                    printf("CPU");
                    moveCursor(3,19);
                    printf("100 %%");
                    moveCursor(5,29);
                    printf("0 %%");
                    moveCursor(1,32);

                    //Read pipes and plot the points on the graphs accurately
                    ssize_t read1 = 1;
                    ssize_t read2 = 1;
                    double memoryPoint;
                    double cpuPoint;
                    for (int k = 0; k < samples; k++){
                        if (read1 > 0){
                            read1 = read(pipeMemory[0], &memoryPoint, sizeof(memoryPoint));
                            moveCursor(11,3);
                            printf("%.2f GB", memoryPoint);
                            plotMemoryUsage(memoryPoint, k + 1, (int)maxYValue);
                            fflush(stdout);
                        }
                        if (read2 > 0){
                            read2 = read(pipeCPU[0], &cpuPoint, sizeof(cpuPoint));
                            moveCursor(11,18);
                            printf("%.2f %%", cpuPoint);
                            plotCPUUsageWithMemory(cpuPoint, k + 1);
                            fflush(stdout);
                        }
                    }
                    moveCursor(1, 31);

                    //Close the read end of the pipes
                    close(pipeMemory[0]);
                    close(pipeCPU[0]);

                    //Display information on cores and frequency
                    ssize_t read3 = 1;
                    int cores;
                    double frequency;
                    for (int j = 0; (j < 2) && (read3 > 0); j++){
                        if (j == 0){
                            read3 = read(pipeCores[0], &cores, sizeof(cores));
                        }
                        else if (j == 1){
                            read3 = read(pipeCores[0], &frequency, sizeof(frequency));
                        }
                    }
                    close(pipeCores[0]);
                    moveCursor(1,31);
                    printf("Number of Cores:");
                    moveCursor(18, 31);
                    printf("%d @ %.2f GHz", cores, frequency);
                    moveCursor(1, 32);
                    printSquaresCoresWithTwoGraphs2(cores);
                    int move = cores / 4;
                    moveCursor(1, (move * 4) + 35);
                }
            }
        }
        return 0;
    }

    //Case: One or more arguements are passed
    if (argc > 1){

        //Set up variables to keep track of which arguments have been passed
        bool hasMemory = false;
        bool hasCPU = false;
        bool hasCores = false;
        bool hasSamplesFlag = false;
        bool hasTDelayFlag = false;
        bool hasPositionalSamples = false;
        bool hasPositionalTDelay = false;
        int errors = 0;
        int tempVal;
        int tempVal2;
        long tempVal3;

        //Check if samples is passed as positional argument
        tempVal = isValidInt(argv[1]);

        //Case: Samples is passed as positional argument
        if ( tempVal != -1){
            hasPositionalSamples = true;
            samples = tempVal;
        }

        //Record which arguments are passed and also error check for any invalid arguements
        int i = 1;
        while (i < argc){

            //Check other arguments given that samples is passed as positional argument
            if (hasPositionalSamples){

                //Check if tdelay is also passed as positional argument
                tempVal2 = isValidInt(argv[i]);
                if ((i == 2) && (tempVal2 != -1)){
                    hasPositionalTDelay = true;
                    tdelay = tempVal2;
                    seconds = convertToSeconds(tdelay);
                }

                //Check if --memory flag is passed
                else if (strcmp(argv[i], "--memory") == 0){
                    hasMemory = true;
                }

                //Check if --cpu flag is passed
                else if (strcmp(argv[i], "--cpu") == 0){
                    hasCPU = true;
                }

                //Check if --cores flag is passed
                else if (strcmp(argv[i], "--cores") == 0){
                    hasCores = true;
                }

                //Check if --samples=N is passed invalidly
                else if (strncmp(argv[i], "--samples=", 10) == 0){
                    errors++;
                    perror("Error: samples is passed as both a posiitonal and flagged argument\n");
                }

                //Check if --tdelay is passed and if it is valid or not
                else if (strncmp(argv[i], "--tdelay=", 9) == 0){

                    //tdelay was already passed as positional argument
                    if (hasPositionalTDelay){
                        errors++;
                        perror("Error: tdelay is passed as both a posiitonal and flagged argument\n");
                    }
                    else{
                        
                        //Check if --tdelay=T flag was passed more than once
                        if (hasTDelayFlag){
                            perror("Error: --tdelay=T flag has been passed multiple times\n");
                            errors++;
                        }
                        hasTDelayFlag = true;
                        
                        //Convert to int
                        char *temp = strchr(argv[i], '=');

                        //Check if --tdelay=T value passed is a numeric value
                        if( !temp || *(temp + 1) == '\0'){
                            perror("Error: --tdelay must have a numeric value\n");
                            errors++;
                        }
                        char *endp;
                        if (temp) {
                            tempVal3 = strtol(temp + 1, &endp, 10);
                            if(*endp != '\0'){
                                perror("Error: --tdelay must have an integer value\n");
                                errors++;
                            }
                            tdelay = tempVal3;
                            seconds = convertToSeconds(tdelay);
                        }
                    }
                }

                //Check if argument passed is invalid
                else{
                    if (i != 1){
                        errors++;
                        printf("Error: unknown argument %s\n", argv[i]);
                    }
                }
            }

            //Check other arguments given that samples is not passed as a positional argument
            if (!hasPositionalSamples){
                
                //Check if --memory flag is passed
                if (strcmp(argv[i], "--memory") == 0){
                    hasMemory = true;
                }

                //Check if --cpu flag is passed
                else if (strcmp(argv[i], "--cpu") == 0){
                    hasCPU = true;
                }

                //Check if --cores flag is passed
                else if (strcmp(argv[i], "--cores") == 0){
                    hasCores = true;
                }
                
                //Check if --tdelay=T is passed
                else if (strncmp(argv[i], "--tdelay=", 9) == 0){

                    //Check if --tdelay=T flag was passed more than once
                    if (hasTDelayFlag){
                        perror("Error: --tdelay=T flag has been passed multiple times\n");
                        errors++;
                    }
                    hasTDelayFlag = true;
                    
                    //Convert to int
                    char *temp = strchr(argv[i], '=');

                    //Check if --tdelay=T value passed is a numeric value
                    if( !temp || *(temp + 1) == '\0'){
                        perror("Error: --tdelay must have a numeric value\n");
                        errors++;
                    }
                    char *endp;
                    if (temp) {
                        tempVal3 = strtol(temp + 1, &endp, 10);
                        if(*endp != '\0'){
                            perror("Error: --tdelay must have an integer value\n");
                            errors++;
                        }
                        tdelay = tempVal3;
                        seconds = convertToSeconds((int)tempVal3);
                    }
                }

                //Check if --samples=N is passed
                else if (strncmp(argv[i], "--samples=", 10) == 0){
                    
                    //Check if --samples=N flag was passed more than once
                    if (hasSamplesFlag){
                        perror("Error: --samples=N flag has been passed multiple times\n");
                        errors++;
                    }
                    hasSamplesFlag = true;
                    
                    //Convert to int
                    char *temp = strchr(argv[i], '=');

                    //Check if --samples=N value passed is a numeric value
                    if( !temp || *(temp + 1) == '\0'){
                        perror("Error: --samples must have a numeric value\n");
                        errors++;
                    }
                    char *endp;
                    if (temp) {
                        tempVal3 = strtol(temp + 1, &endp, 10);
                        if(*endp != '\0'){
                            perror("Error: --samples must have an integer value\n");
                            errors++;
                        }
                        samples = (int)tempVal3;
                    }
                }

                //Check if argument passed is invalid
                else{
                    errors++;
                    printf("Error: unknown argument %s\n", argv[i]);
                }
            }
            i++;
        }

        //Exit program if invalid arguments were found
        if (errors != 0){
            exit(1);
        }

        //Update to ensure program works when only samples is passed as positional argument
        bool cond1 = hasPositionalSamples && !hasCores && !hasCPU && !hasMemory && !hasPositionalTDelay && !hasTDelayFlag;
        if (cond1){
            tdelay = 500000;
            seconds = convertToSeconds(tdelay);
            hasCores = true;
            hasCPU = true;
            hasMemory = true;
        }

        //Update to ensure program works when only samples and tdelay are passed as positional arguments
        bool cond2 = hasPositionalSamples && !hasCores && !hasCPU && !hasMemory && hasPositionalTDelay && !hasTDelayFlag;
        if (cond2){
            hasCores = true;
            hasCPU = true;
            hasMemory = true;
        }

        //Update to ensure program works when only samples is passed as positional and tdelay as flagged arguments
        bool cond3 = hasPositionalSamples && hasTDelayFlag && !hasCores && !hasCPU && !hasMemory;
        if (cond3){
            hasCores = true;
            hasCPU = true;
            hasMemory = true;
        }

        //Now at this point we have recorded all arguments passed and have ensured that there are no errors
        
        //Print the number of samples and time delay to user
        clearTerminal();
        printf("Nbr of samples: %d -- every %d microSecs (%.3f secs)\n", samples, tdelay, seconds);
        printf("\n");
        
        //Create a pipe for --memory calculation
        int pipeMemory[2];
        if (pipe(pipeMemory) == -1){
            perror("Error: Could not create pipe\n");
            exit(1);
        }

        //Create the child1 process that will computer memory utilization
        int child1 = fork();

        //Check if child process was successfully created
        if (child1 == -1){
            perror("Error: Could not create child process\n");
            close(pipeMemory[0]);
            close(pipeMemory[1]);
            exit(1);
        }

        

        //Set the child1 process to compute memory utilization and write to parent
        if (child1 == 0){

            sigaction(SIGTSTP, &newact2, NULL);
            sigaction(SIGINT, &newact2, NULL);

            //Set up SIGUSR1 handler
            struct sigaction c;
            c.sa_handler = CustomHandler;
            sigemptyset(&c.sa_mask);
            c.sa_flags = 0;
            sigaction(SIGUSR1, &c, NULL);

            //Close the read end of the pipe
            close(pipeMemory[0]);

            //Check if --memory flag was passed
            if (!hasMemory){
                close(pipeMemory[1]);
                exit(0);
            }
            ssize_t check = 1;
            for (int j = 0; (j < samples) && (check > 0); j++){
                double totalMemoryGB = 0.0;
                double memoryUsedGB = 0.0;
                getMemoryUsage(&totalMemoryGB, &memoryUsedGB);
                check = write(pipeMemory[1], &memoryUsedGB, sizeof(memoryUsedGB));

                //Wait tdelay specified amount before computing next value
                //sleep(seconds);
                usleep(tdelay);
            }

            //child1 process has finished computing memory and will now terminate
            close(pipeMemory[1]);
            exit(0);
        }

        //Continue with the parent process
        else if (child1 > 0){

            //Close the writing end pipe of the memory process
            close(pipeMemory[1]);
            
            //Create a pipe for --cpu utilization computation
            int pipeCPU[2];

            //Error check to see if pipe was successfully created
            if (pipe(pipeCPU) == -1){
                perror("Error: Could not create pipe\n");
                exit(1);
            }

            //Create child2 process for CPU utilization computation
            int child2 = fork();

            //Check if child process was successfully created
            if (child2 == -1){
                perror("Error: Could not create child process\n");
                close(pipeCPU[0]);
                close(pipeCPU[1]);
                exit(1);
            }

            

            //Set the child2 process to compute cpu utilization
            if (child2 == 0){

                sigaction(SIGTSTP, &newact2, NULL);
                sigaction(SIGINT, &newact2, NULL);

                //Set up SIGUSR1 handler
                struct sigaction c;
                c.sa_handler = CustomHandler;
                sigemptyset(&c.sa_mask);
                c.sa_flags = 0;
                sigaction(SIGUSR1, &c, NULL);
                
                //Close the reading end of the pipe
                close(pipeCPU[0]);

                //Check if --cpu flag was passed
                if (!hasCPU){
                    close(pipeCPU[1]);
                    exit(0);
                }
                //
                ssize_t check = 1;
                for (int j = 0; (j < samples) && (check > 0); j++){
                    long t1;
                    long t2;
                    long u1;
                    long u2;
                    getCPU_Usage(&t1, &u1);
                    //sleep(seconds);
                    usleep(tdelay);
                    getCPU_Usage(&t2, &u2);
                    int numerator = u2 - u1;
                    int denominator = t2 - t1;
                    double cpuUsage = ((double)numerator / denominator) * 100;
                    if ((numerator <=0) || (denominator <= 0) || (cpuUsage <=0)){
                        cpuUsage = 0.00;
                    }
                    check = write(pipeCPU[1], &cpuUsage, sizeof(cpuUsage));
                }
                
                //child1 process has finished computing memory and will now terminate
                close(pipeCPU[1]);
                exit(0);
            }

            //Continue with the parent process
            else if (child2 >0){
                
                //Create a pipe for cores and frequency calculations
                int pipeCores[2];

                //Check if pipe was successfully created
                if (pipe(pipeCores) == -1){
                    perror("Error: Could not create pipe\n");
                    exit(1);
                }

                //Create child3 process for cores and freuqency calculations
                int child3 = fork();

                //Check if child3 process was successfully created
                if (child3 == -1){
                    perror("Error: Could not create child process\n");
                    close(pipeCores[0]);
                    close(pipeCores[0]);
                    exit(1);
                }

                //Set child3 process to compute cores and frequency calculations
                if (child3 == 0){

                    sigaction(SIGTSTP, &newact2, NULL);
                    sigaction(SIGINT, &newact2, NULL);

                    //Set up SIGUSR1 handler
                    struct sigaction c;
                    c.sa_handler = CustomHandler;
                    sigemptyset(&c.sa_mask);
                    c.sa_flags = 0;
                    sigaction(SIGUSR1, &c, NULL);
                    
                    //Check if --cores flag was passed
                    if (!hasCores){
                        close(pipeCores[0]);
                        close(pipeCores[1]);
                        exit(0);
                    }

                    //Close the reading end of the pipe
                    close(pipeCores[0]);

                    //Compute cores and frequency calculations
                    int cores = getNumberOfCores();
                    double maxFrequency = getMaxFrequencyOfCores();
                    ssize_t check = 1;
                    int loop = 1;
                    while (check > 0 && loop<3){
                        if (loop == 1){
                            check = write(pipeCores[1], &cores, sizeof(cores));
                        }
                        else if (loop ==2){
                            check = write(pipeCores[1], &maxFrequency, sizeof(maxFrequency));
                        }
                        loop++;
                    }

                    //Child3 process is done and so it will terminate
                    close(pipeCores[1]);
                    exit(0);
                }

                //Continue with parent process
                else if (child3 > 0){

                    //Immediately close the reading end of the pipes that will not be used
                    if(!hasMemory){
                        close(pipeMemory[0]);
                    }
                    if(!hasCPU){
                        close(pipeCPU[0]);
                    }
                    if(!hasCores){
                        close(pipeCores[0]);
                    }

                    
                    //Now at this point we have set up all the processes and they will close their ends of the pipes
                    //and terminate themselves

                    //Display information of memory and cpu utilization
                    if (hasMemory && hasCPU){

                        //Print the memory graph
                        printf("  Memory\n");
                        double maxYValue = get_total_memory_gb();
                        printMemoryGrid(maxYValue, samples);
                        moveCursor(3,4);
                        printf("%d GB", (int)maxYValue);
                        moveCursor(4,16);
                        printf("0 GB");
                        moveCursor(1,19);

                        //Print the cpu graph
                        printCPUGrid(samples);
                        printf("\n");
                        moveCursor(6,18);
                        printf("CPU");
                        moveCursor(3,19);
                        printf("100 %%");
                        moveCursor(5,29);
                        printf("0 %%");
                        moveCursor(1,32);

                        //Read pipes and plot the points on the graphs accurately
                        ssize_t read1 = 1;
                        ssize_t read2 = 1;
                        double memoryPoint;
                        double cpuPoint;
                        for (int k = 0; k < samples; k++){
                            if (read1 > 0){
                                read1 = read(pipeMemory[0], &memoryPoint, sizeof(memoryPoint));
                                moveCursor(11,3);
                                printf("%.2f GB", memoryPoint);
                                plotMemoryUsage(memoryPoint, k + 1, (int)maxYValue);
                                fflush(stdout);
                            }
                            if (read2 > 0){
                                read2 = read(pipeCPU[0], &cpuPoint, sizeof(cpuPoint));
                                moveCursor(11,18);
                                printf("%.2f %%", cpuPoint);
                                plotCPUUsageWithMemory(cpuPoint, k + 1);
                                fflush(stdout);
                            }
                        }
                        moveCursor(1, 31);

                        //Close the read end of the pipes
                        close(pipeMemory[0]);
                        close(pipeCPU[0]);
                    }

                    //Display information about only memory utilization
                    else if (hasMemory){
                        
                        //Print the memory graph
                        printf("  Memory\n");
                        double maxYValue = get_total_memory_gb();
                        printMemoryGrid(maxYValue, samples);
                        moveCursor(3,4);
                        printf("%d GB", (int)maxYValue);
                        moveCursor(4,16);
                        printf("0 GB");
                        moveCursor(1,19);
                        
                        //Read pipe and plot the point on the graph accurately
                        ssize_t read1 = 1;
                        double memoryPoint;
                        for (int k = 0; k < samples; k++){
                            if (read1 > 0){
                                read1 = read(pipeMemory[0], &memoryPoint, sizeof(memoryPoint));
                                moveCursor(11,3);
                                printf("%.2f GB", memoryPoint);
                                plotMemoryUsage(memoryPoint, k + 1, (int)maxYValue);
                                fflush(stdout);
                            }
                        }


                        moveCursor(1, 18);

                        //Close the read end of the pipe
                        close(pipeMemory[0]);
                    }

                    //Display information about only cpu utilization
                    else if (hasCPU){
                        
                        //Print the cpu graph
                        printf("\n");
                        printCPUGrid(samples);
                        printf("\n");
                        moveCursor(6,3);
                        printf("CPU");
                        moveCursor(3,4);
                        printf("100 %%");
                        moveCursor(5,14);
                        printf("0 %%");
                        
                        //Read pipe and plot the point on the graph accurately
                        ssize_t read2 = 1;
                        double cpuPoint;
                        for (int k = 0; k < samples; k++){
                            if (read2 > 0){
                                read2 = read(pipeCPU[0], &cpuPoint, sizeof(cpuPoint));
                                moveCursor(11,3);
                                printf("%.2f %%", cpuPoint);
                                plotCPUUsage(cpuPoint, k + 1);
                                fflush(stdout);
                            }
                        }

                        moveCursor(1, 18);

                        //Close the read end of the pipe
                        close(pipeCPU[0]);
                    }

                    //Display information on cores and frequency
                    if (hasCores){
                        ssize_t read3 = 1;
                        int cores;
                        double frequency;
                        for (int j = 0; (j < 2) && (read3 > 0); j++){
                            if (j == 0){
                                read3 = read(pipeCores[0], &cores, sizeof(cores));
                            }
                            else if (j == 1){
                                read3 = read(pipeCores[0], &frequency, sizeof(frequency));
                            }
                        }
                        close(pipeCores[0]);
                        if (hasMemory && hasCPU){
                            moveCursor(1,31);
                            printf("Number of Cores:");
                            moveCursor(18, 31);
                            printf("%d @ %.2f GHz", cores, frequency);
                            moveCursor(1, 32);
                            printSquaresCoresWithTwoGraphs(cores);
                            int move = cores / 4;
                            moveCursor(1, (move * 4) + 35);
                        }
                        else if (hasMemory || hasCPU){
                            if(hasMemory){
                                moveCursor(1,18);
                                printf("Number of Cores:");
                                moveCursor(18, 18);
                                printf("%d @ %.2f GHz", cores, frequency);
                                moveCursor(1, 19);
                                printSquaresCoresWithOneGraph1(cores);
                                int move = cores / 4;
                                moveCursor(1, (move * 4) + 19);
                            }
                            else{
                                moveCursor(1,16);
                                printf("Number of Cores:");
                                moveCursor(18, 16);
                                printf("%d @ %.2f GHz", cores, frequency);
                                moveCursor(1, 17);
                                printSquaresCoresWithOneGraph2(cores);
                                int move = cores / 4;
                                moveCursor(1, (move * 4) + 18);
                            }
                        }
                        else{
                            moveCursor(1,3);
                            printf("Number of Cores:");
                            moveCursor(18, 3);
                            printf("%d @ %.2f GHz", cores, frequency);
                            moveCursor(1, 4);
                            printSquaresCores(cores);
                            int move = cores / 4;
                            moveCursor(1, (move * 4) + 4);
                        }
                    }
                }
            }
        }
        return 0;
    }

    //Something went wrong with the program
    else{
        perror("Error: Could not run program\n");
        exit(1);
    }
}

//Final version 07/04/25