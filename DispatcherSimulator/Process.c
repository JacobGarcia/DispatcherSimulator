/*************************************************************************/
/*                                                                       */
/* Copyright (c) 2014 Mario García Navarro. All rights reserved.         */
/*                                                                       */
/* File name: Process.c                                                  */
/*                                                                       */
/* Author:                                                               */
/*          Mario García Navarro                                         */
/*                                                                       */
/* Purpose:                                                              */
/*          Implement routines that simulate a dispatcher in 6           */
/*          different algorithms                                         */
/*                                                                       */
/* References:                                                           */
/*          Abelardo López Lagunas code shown during lab class           */
/*                                                                       */
/* Restrictions:                                                         */
/*          The code is implemented using static structures, due to this */
/*          this fact, the number of processes is limited to 10. If by   */
/*          any means this rule is not considered, the code simply will  */
/*          not work as expected                                         */
/*                                                                       */
/* Revision history:                                                     */
/*          Sep  17 01:21 2014 - File created                            */
/*                                                                       */
/*          Sep  20 17:52 2014 - Added FCFS and Non Preemptive           */
/*                               algorithms implementations              */
/*                                                                       */
/*          Sep  21 19:34 2014 - Added Preemptive and Round Robin        */
/*                                 algorithms implementations            */
/*                                                                       */
/*          Sep  22 01:12 2014 - Code Refactoring                        */
/*                                                                       */
/* Error handling:                                                       */
/*          None                                                         */
/*                                                                       */
/* Notes:                                                                */
/*          Six different algoritms will be implemented with the purpose */
/*          of simulate the behaviour of diverse dispatchers. The        */
/*          average time of the processes will be obtained in order      */
/*          to conclude which is the best algorithm in these terms       */
/*                                                                       */
/*************************************************************************/

#include <stdio.h>                        /* Used for the function printf */
#include "Process.h"                                   /* Function header */

#define NUMBERPROCESSES 10          /* The number of max processes that the
                                                    structure can handle */
int numberOfProcess = 0;                /* The number of processes that are
                                                     defined in the file */

struct process{
    int pID;                                           /* The process ID */
    int priority;        /* This is important for the Priority Algorithm */
    int cpuBurst;                             /* Lenght of the CPU burst */
    int arrivalTime;                         /* Time of process creation */
    int waitTime;                        /* Time spent in the wait queue */
    int firstExecuted;                        /* Time of first execution */
    int lastExecuted;                          /* Time of last execution */
};

struct process processList[NUMBERPROCESSES];    /* The main process list */

struct process contextProcessList[NUMBERPROCESSES * 2]; /* This is defined 
                                                         because is 
                                                         required another 
                                                         process list
                                                         that contains
                                                         the information
                                                         about the context
                                                         changes. Is
                                                         used on the 
                                                         preemptive 
                                                         algorithms */

struct process processListCopy[NUMBERPROCESSES];     /* This is defined in
                                                     order to keep a clean
                                                          copy of the main
                                                           process list */

/*************************************************************************/
/*                                                                       */
/*  Function: CreateProcessList                                          */
/*                                                                       */
/*  Purpose: The function will be in charge to create the main list      */
/*           of processes                                                */
/*                                                                       */
/*  Parameters:                                                          */
/*            Input :   The process ID, arrival time, CPU burst and      */
/*                      priority                                         */
/*                                                                       */
/*            Output:   The main process list                            */
/*                                                                       */
/*************************************************************************/
void CreateProcessList(int pID, int arrivalTime, int cpuBurst, int priority){
    /* All the paramaters obtained from the file are 
    assigned to the new process */
    processList[numberOfProcess].pID = pID;
    processList[numberOfProcess].arrivalTime = arrivalTime;
    processList[numberOfProcess].cpuBurst = cpuBurst;
    processList[numberOfProcess].priority = priority;
    numberOfProcess++; /* This variable is incremented in order to know how 
                        many processes we will be working with */
}

/*************************************************************************/
/*                                                                       */
/*  Function: SortProcessList                                            */
/*                                                                       */
/*  Purpose: This function sorts the process list considering different  */
/*           conditions for the sort. All the processes are sorted       */
/*           using the 'Bubble Sort' algorithm                           */
/*                                                                       */
/*  Parameters:                                                          */
/*            Input :   The sort condition                               */
/*                                                                       */
/*            Output:   The main process list ordered by the condition   */
/*                      that have been determined earlier                */
/*                                                                       */
/*************************************************************************/
void SortProcessList(int sortBy){
    switch (sortBy) {
        
        /* This case considers only the arrival time for the sort */
        case ARRIVALTIME:
            for (int i = 0; i < numberOfProcess; i++){
                for (int j = i + 1; j < numberOfProcess; j++){
                    if (processList[i].arrivalTime > processList[j].arrivalTime){
                        /* Exchange the processes if the arrival time is less */
                        struct process temporal = processList[i]; /* Create a temporal structure that stores a process
                                                                   in order to not lose it during the exchange */
                        processList[i] =  processList[j];
                         processList[j] = temporal;
                    }
                }
            }
            break;
            
        /* This condition is used on the SJF algorithms. Basically is
        used if there exists processes that arrive at the same time but
        differs on CPU burst. This orders the process list considering
        the lowest CPU burst */
        case CPUBURST:
            for (int i = 0; i < numberOfProcess; i++){
                for (int j = i + 1; j < numberOfProcess; j++){
                    /* Exchange the processes if the arrival time is the same and the CPU burst is less */
                    if (processList[i].arrivalTime == processList[j].arrivalTime && processList[i].cpuBurst > processList[j].cpuBurst){
                        struct process temporal = processList[i]; /* Create a temporal structure that stores a process
                                                                  in order to not lose it during the exchange */
                        processList[i] =  processList[j];
                        processList[j] = temporal;
                    }
                }
            }
            break;
        
        /* This condition is used on the Priority algorithms. Basically 
        is used if there exists processes that arrive at the same time but
        differs on priority. This orders the process list considering
        the highest priority */
        case PRIORITY:
            for (int i = 0; i < numberOfProcess; i++){
                for (int j = i + 1; j < numberOfProcess; j++){
                    /* Exchange the processes if the arrival time is the same and the priority is higher */
                    if (processList[i].arrivalTime == processList[j].arrivalTime && processList[i].priority > processList[j].priority){
                        struct process temporal = processList[i]; /* Create a temporal structure that stores a process
                                                                  in order to not lose it during the exchange */
                        processList[i] =  processList[j];
                        processList[j] = temporal;
                    }
                }
            }
            break;
            
        default:
            break;
    }
}

/*************************************************************************/
/*                                                                       */
/*  Function: Sort                                                       */
/*                                                                       */
/*  Purpose: This function sorts the process list considering different  */
/*           conditions for the sort. All the processes are sorted       */
/*           using the 'Bubble Sort' algorithm. Actually, this function
             does the same as SortListProcess, but it is needed since
             in this function the start and the end condition are 
             variable, recieved by the function. */
/*                                                                       */
/*  Parameters:                                                          */
/*            Input :   The sort condition                               */
/*                                                                       */
/*            Output:   The main process list ordered by the condition   */
/*                      that have been determined earlier                */
/*                                                                       */
/*************************************************************************/
void Sort(int start, int numberOfProcess){
    for (int i = start; i < numberOfProcess; i++){
        for (int j = i + 1; j < numberOfProcess; j++){
            if (processList[i].cpuBurst == processList[j].cpuBurst){
                if (processList[i].pID > processList[j].pID){
                    struct process temporal = processList[i];
                    processList[i] =  processList[j];
                    processList[j] = temporal;
                }
            }
            else if (processList[i].cpuBurst > processList[j].cpuBurst){
                struct process temporal = processList[i];
                processList[i] =  processList[j];
                processList[j] = temporal;
            }
        }
    }
}

/*************************************************************************/
/*                                                                       */
/*  Function: WaitTime                                                   */
/*                                                                       */
/*  Purpose: Obtains the average wait time of all the processes. It is   */
/*           called after a scheduler algorithm, so none sort or         */
/*           treatment to the data is needed                             */
/*                                                                       */
/*  Parameters:                                                          */
/*            Input :   The process list and the number of processes     */
/*                      that it contains                                 */
/*                                                                       */
/*            Output:   The average wait time of all the processes       */
/*                                                                       */
/*************************************************************************/
float WaitTime(int numberOfProcesses, struct process processList[]){
    float averageWaitTime = 0; /* The variable that will store the average 
                                waiting time */
    for (int i = 0; i < numberOfProcesses; i++){
        /* All of the waiting times of each process is obtained and
         stored in one only variable */
        averageWaitTime = averageWaitTime + processList[i].waitTime;
    }
    
    /* The sum is divided by the number of processes in order to obtain 
    the average waiting time */
    return averageWaitTime = averageWaitTime / numberOfProcess;
}

/*************************************************************************/
/*                                                                       */
/*  Function: ContextChanges                                             */
/*                                                                       */
/*  Purpose: Prints the timeline of the context changes. It describes    */
/*           which process is running at a particular time               */
/*                                                                       */
/*  Parameters:                                                          */
/*            Input :   The process list and the number of processes     */
/*                      that it contains                                 */
/*                                                                       */
/*            Output:   The context changes timeline                     */
/*                                                                       */
/*************************************************************************/
void ContextChanges(int numberOfProcesses, struct process processList[]){
    for (int i = 0; i < numberOfProcesses; i++) {
        /* The time that the process is executed */
        printf("\n|        t    =   %2d        |\n", processList[i].firstExecuted);
        /* This indicates which process is running at a specific time */
        printf("|        Process: %2d        |\n", processList[i].pID);
        printf("-----------------------------");

    }
}

/*************************************************************************/
/*                                                                       */
/*  Function: FirstComeFS                                                */
/*                                                                       */
/*  Purpose: Generates the wait time for each of the processes using     */
/*           the First Come First Served algorithm. Furthermore, the     */
/*           first execution time of each of the processes is determined */
/*                                                                       */
/*  Parameters:                                                          */
/*            Input :   The process list which in will be computing the  */
/*                      average wait time                                */
/*                                                                       */
/*            Output:   All the process list information considering the */
/*                      average wait time and the context changes        */
/*                                                                       */
/*************************************************************************/
void FirstComeFS(){
    
    float averageWaitTime = 0;
    
    printf("-----------------------------\n");
    printf("|  First Come First Served  |\n");
    printf("-----------------------------");
    processList[0].waitTime = 0; /* Since the first process in the list always 
                                  executes at arrival, it's waiting time is always zero*/
    
    processList[0].firstExecuted = processList[0].arrivalTime; /* The first time that the 
                                                                initial process is executed is 
                                                                when it arrives*/
    
    for (int i = 1; i < numberOfProcess; i++) {
        
        /* The first time that a process is executed is when previous processes finishes its execution */
        processList[i].firstExecuted = processList[i-1].cpuBurst + processList[i-1].firstExecuted;
        
        /* The wait time of a process can be found if at its first execution it is substracted the process arrival time*/
        processList[i].waitTime = processList[i].firstExecuted - processList[i].arrivalTime;
    }
    
   
    averageWaitTime = WaitTime(numberOfProcess, processList); /* The average waiting time is obtained */
    ContextChanges(numberOfProcess, processList);   /* The context changes are printed */
    printf("\n| Average Wait Time:  %.2f  |\n", averageWaitTime); /* The average waiting time is shown */
    printf("-----------------------------\n");
}



void SortPriority(int start, int numberOfProcess){
    for (int i = start; i < numberOfProcess; i++){
        for (int j = i + 1; j < numberOfProcess; j++){
            if (processList[i].priority == processList[j].priority){
                if (processList[i].pID > processList[j].pID){
                    struct process temporal = processList[i];
                    processList[i] =  processList[j];
                    processList[j] = temporal;
                }
            }
            else if (processList[i].priority > processList[j].priority){
                struct process temporal = processList[i];
                processList[i] =  processList[j];
                processList[j] = temporal;
            }
        }
    }
}

void NonPreemptive(){
    
    printf("-----------------------------\n");
    printf("|     SJF Non Preemptive    |\n");
    printf("-----------------------------");
    
    int lastProcess = 0;
    int burst = 0;
    SortProcessList(CPUBURST);
    processList[0].waitTime = 0;
    processList[0].firstExecuted = processList[0].arrivalTime;
    
    for (int i = 0; i < numberOfProcess; i++) {
        lastProcess = 0;
        burst = processList[i].cpuBurst + burst;
        for (int j = i + 1; j < numberOfProcess; j++) {
            if (burst < processList[j].arrivalTime) {
                lastProcess = j;
                j = numberOfProcess;
            }
        }
        if (lastProcess == 0) {
            Sort(i + 1, numberOfProcess);
            i = numberOfProcess;
        }
        else
            Sort(i + 1, lastProcess);
    }
    
    for (int i = 1; i < numberOfProcess; i++) {
        processList[i].firstExecuted = processList[i-1].cpuBurst + processList[i-1].firstExecuted;
        processList[i].waitTime = processList[i].firstExecuted - processList[i].arrivalTime;
    }
    
    
    float averageWaitTime = WaitTime(numberOfProcess, processList);
    ContextChanges(numberOfProcess, processList);
    printf("\n| Average Wait Time:  %.2f  |\n", averageWaitTime); /* The average waiting time is shown */
    printf("-----------------------------\n");
}

void NonPreemptivePriority(){
    
    printf("-----------------------------\n");
    printf("|  Priority Non Preemptive  |\n");
    printf("-----------------------------");
    
    int lastProcess = 0;
    int burst = 0;
    SortProcessList(ARRIVALTIME);
    SortProcessList(PRIORITY);
    processList[0].waitTime = 0;
    processList[0].firstExecuted = processList[0].arrivalTime;
    
    for (int i = 0; i < numberOfProcess; i++) {
        lastProcess = 0;
        burst = processList[i].cpuBurst + burst;
        for (int j = i + 1; j < numberOfProcess; j++) {
            if (burst < processList[j].arrivalTime) {
                lastProcess = j;
                j = numberOfProcess;
            }
        }
        if (lastProcess == 0) {
            SortPriority(i + 1, numberOfProcess);
            i = numberOfProcess;
        }
        else
            SortPriority(i + 1, lastProcess);
    }
    
    for (int i = 1; i < numberOfProcess; i++) {
        processList[i].firstExecuted = processList[i-1].cpuBurst + processList[i-1].firstExecuted;
        processList[i].waitTime = processList[i].firstExecuted - processList[i].arrivalTime;
    }
    
    
    float averageWaitTime = WaitTime(numberOfProcess, processList);
    ContextChanges(numberOfProcess, processList);
    printf("\n| Average Wait Time:  %.2f  |\n", averageWaitTime); /* The average waiting time is shown */
    printf("-----------------------------\n");
    
}

int Treatment(){
    int difference = 0;
    if (processList[0].arrivalTime > 0) {
        difference = processList[0].arrivalTime;
        for (int i = 0; i < numberOfProcess; i++) {
            processList[i].arrivalTime = processList[i].arrivalTime - difference;
        }
    }
    
    return difference;
}

void inverseTreatment(int difference,  int numberOfProcesses){
    for (int i = 0; i < numberOfProcesses; i++) {
        contextProcessList[i].firstExecuted = contextProcessList[i].firstExecuted + difference;
    }
}

void copyProcessList(){
    for (int i = 0; i < numberOfProcess; i++) {
        processListCopy[i] = processList[i];
    }
}

void restoreProcessList(){
    for (int i = 0; i < numberOfProcess; i++) {
        processList[i] = processListCopy[i];
    }
}

void Preemptive(){
    
    printf("-----------------------------\n");
    printf("|       SJF  Preemptive     |\n");
    printf("-----------------------------");
    
    
    SortProcessList(ARRIVALTIME);
    
    copyProcessList();
    
    SortProcessList(CPUBURST);
    
    int difference = Treatment();
    
    int burst = 0;
    int processRunning = 0;
    int cont = 0;
    int lastProcess = 0;
    int contextSwitch = 0;
    int displacer = 0;
    
    while (processRunning < numberOfProcess - 1) {
        lastProcess = 0;
        burst = 0;
        contextSwitch = 0;
        displacer = 0;
        for (int i = 0; i < processRunning + cont; i++) {
            burst = burst + contextProcessList[i].cpuBurst;
        }
        if (burst != 0) {
            for (int k = processRunning; k < numberOfProcess; k++) {
                if (burst >= processList[k].arrivalTime) {
                    lastProcess = k;
                }
            }
            
            if(lastProcess != 0)
                Sort(processRunning, lastProcess + 1);
        }
        for (int j = 1; j < processList[processRunning].cpuBurst + 1; j++) {
            processList[processRunning].lastExecuted = j;
            if (j >= processList[processRunning + 1 + contextSwitch + displacer].arrivalTime) {
                if (processList[processRunning].cpuBurst - j > processList[processRunning + 1 + contextSwitch + displacer].cpuBurst) {
                    contextProcessList[processRunning] = processList[processRunning];
                    contextProcessList[processRunning].cpuBurst = contextProcessList[processRunning].lastExecuted;
                    processList[processRunning].cpuBurst = processList[processRunning].cpuBurst - j;
                    struct process temporal = processList[processRunning];
                    processList[processRunning] =  processList[processRunning + 1 + displacer];
                    processList[processRunning + 1 + displacer] = temporal;
                    j = 1;
                    displacer = 0;
                    cont++;
                    contextSwitch++;
                }
                else
                   displacer = (displacer + 1) % (numberOfProcess - (processRunning + contextSwitch + 1));
            }
        }
        
        if (processList[processRunning].cpuBurst == processList[processRunning].lastExecuted) {
            contextProcessList[processRunning + cont] = processList[processRunning];
            contextProcessList[processRunning + cont].lastExecuted = 0;
            processRunning++;
        }
    }
    
    contextProcessList[processRunning + cont] = processList[processRunning];
    
     contextProcessList[0].waitTime = 0;
     contextProcessList[0].firstExecuted = contextProcessList[0].arrivalTime;
    
    for (int i = 1; i < processRunning + cont + 1; i++) {
        contextProcessList[i].firstExecuted = contextProcessList[i-1].cpuBurst + contextProcessList[i-1].firstExecuted;
        contextProcessList[i].waitTime = contextProcessList[i].firstExecuted - contextProcessList[i].arrivalTime - contextProcessList[i - 1].lastExecuted;
    }
    
    
    float averageWaitTime = WaitTime(processRunning + cont + 1, contextProcessList);
    
    if (difference != 0)
        inverseTreatment(difference, processRunning + cont + 1);
    
    ContextChanges(processRunning + cont + 1, contextProcessList);
    printf("\n| Average Wait Time:  %.2f  |\n", averageWaitTime); /* The average waiting time is shown */
    printf("-----------------------------\n");
    restoreProcessList();
}

void printProcesses(){
    for (int i = 0; i < numberOfProcess; i++) {
        printf("\n");
        printf("%d", processList[i].pID);
        printf("%d", processList[i].arrivalTime);
        printf("%d", processList[i].cpuBurst);
        printf("%d", processList[i].priority);
    }
}

void PreemptivePriority(){
    
    printf("-----------------------------\n");
    printf("|     Priority Preemptive   |\n");
    printf("-----------------------------");
    
    SortProcessList(PRIORITY);
    
    int difference = Treatment();
    
    int burst = 0;
    int processRunning = 0;
    int cont = 0;
    int lastProcess = 0;
    int contextSwitch = 0;
    int displacer = 0;
    
    while (processRunning < numberOfProcess - 1) {
        lastProcess = 0;
        burst = 0;
        contextSwitch = 0;
        displacer = 0;
        for (int i = 0; i < processRunning + cont; i++) {
            burst = burst + contextProcessList[i].cpuBurst;
        }
        if (burst != 0) {
            for (int k = processRunning; k < numberOfProcess; k++) {
                if (burst >= processList[k].arrivalTime) {
                    lastProcess = k;
                }
            }
            
            if(lastProcess != 0)
                SortPriority(processRunning, lastProcess + 1);
        }
        for (int j = 1; j < processList[processRunning].cpuBurst + 1; j++) {
            processList[processRunning].lastExecuted = j;
            if (j >= processList[processRunning + 1 + displacer + contextSwitch].arrivalTime) {
                if (processList[processRunning].priority > processList[processRunning + 1 + displacer + contextSwitch].priority) {
                    contextProcessList[processRunning] = processList[processRunning];
                    contextProcessList[processRunning].cpuBurst = contextProcessList[processRunning].lastExecuted;
                    processList[processRunning].cpuBurst = processList[processRunning].cpuBurst - processList[processRunning].lastExecuted;
                    struct process temporal = processList[processRunning];
                    processList[processRunning] =  processList[processRunning + 1 + displacer];
                    processList[processRunning + 1 + displacer] = temporal;
                    displacer = 0;
                    j = 0;
                    cont++;
                    contextSwitch++;
                }
                else
                    displacer = (displacer + 1) % (numberOfProcess - (processRunning + contextSwitch + 1));
            }
        }
        
        if (processList[processRunning].cpuBurst == processList[processRunning].lastExecuted) {
            contextProcessList[processRunning + cont] = processList[processRunning];
            contextProcessList[processRunning + cont].lastExecuted = 0;
            processRunning++;
        }
    }
    
    contextProcessList[processRunning + cont] = processList[processRunning];
    
    contextProcessList[0].waitTime = 0;
    contextProcessList[0].firstExecuted = contextProcessList[0].arrivalTime;
    
    for (int i = 1; i < processRunning + cont + 1; i++) {
        contextProcessList[i].firstExecuted = contextProcessList[i-1].cpuBurst + contextProcessList[i-1].firstExecuted;
        contextProcessList[i].waitTime = contextProcessList[i].firstExecuted - contextProcessList[i].arrivalTime - contextProcessList[i - 1].lastExecuted;
    }
    
    
    float averageWaitTime = WaitTime(processRunning + cont + 1, contextProcessList);
    
    if (difference != 0)
        inverseTreatment(difference, processRunning + cont + 1);
    
    ContextChanges(processRunning + cont + 1, contextProcessList);
    printf("\n| Average Wait Time:  %.2f  |\n", averageWaitTime); /* The average waiting time is shown */
    printf("-----------------------------\n");
    
    restoreProcessList();
}

void RoundRobin(int quantum){
    
    printf("-----------------------------\n");
    printf("|         Round Robin       |\n");
    printf("-----------------------------");
    
    int processesCompleted = 0;
    int difference = 0;
    int time = processList[0].arrivalTime;
    int cont = 0;
    
    copyProcessList();
    
    while (processesCompleted < numberOfProcess) {
        printf("\n|        t    =   %2d        |\n", time);
        /* This indicates which process is running at a specific time */
        printf("|        Process: %2d        |\n", processList[0].pID);
        printf("-----------------------------");
        difference = processList[0].cpuBurst - quantum;
        if (difference > 0) {
            time = time + quantum;
            processList[0].cpuBurst = difference;
            for (int i = 0; i < numberOfProcess; i++) {
                if (processList[0].pID == processListCopy[i].pID) {
                    processListCopy[i].lastExecuted = processListCopy[i].lastExecuted + 1;
                }
            }
            for (int j = 1; j < numberOfProcess - cont; j++) {
                if (time >= processList[j].arrivalTime) {
                    struct process temporal = processList[j - 1];
                    processList[j - 1] =  processList[j];
                    processList[j] = temporal;
                }
            }
        }
        else {
                for (int i = 0; i < numberOfProcess; i++) {
                if (processList[0].pID == processListCopy[i].pID) {
                    processListCopy[i].firstExecuted = time;
                }
            }
            time = time + quantum + difference;
            processesCompleted++;
            for (int k = 1; k < numberOfProcess; k++) {
                processList[k - 1] =  processList[k];
            }
            cont++;
        }
    }
    
    float averageWaitTime = 0.0;
    for (int i = 0; i < numberOfProcess; i++) {
        processListCopy[i].waitTime = processListCopy[i].firstExecuted - processListCopy[i].arrivalTime - (processListCopy[i].lastExecuted * quantum);
        averageWaitTime = averageWaitTime + processListCopy[i].waitTime;
    }
    averageWaitTime = averageWaitTime / numberOfProcess;
    
    printf("\n| Average Wait Time: %.2f  |\n", averageWaitTime); /* The average waiting time is shown */
    printf("-----------------------------\n");
}