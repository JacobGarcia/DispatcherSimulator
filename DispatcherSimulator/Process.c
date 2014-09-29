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
/*          not work as expected.                                        */
/*          There is no management if exists 'gaps' or 'spaces' between  */
/*          processes in the main process list                           */
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
                    /* Exchange the processes if the arrival time of the first process is higher than the second process */
                    if (processList[i].arrivalTime > processList[j].arrivalTime){
                        struct process temporal = processList[i]; /* Create a temporal structure that stores a process
                                                                   in order to not lose it during the exchange */
                        processList[i] =  processList[j];
                         processList[j] = temporal;
                    }
                }
            }
            break;
            
        /* This condition is used on the SJF algorithms. Basically is
        used if there exists processes that arrive at the same time at the very beginning but
        differs on CPU burst. This orders the process list considering
        the lowest CPU burst */
        case CPUBURST:
            for (int i = 0; i < numberOfProcess; i++){
                for (int j = i + 1; j < numberOfProcess; j++){
                    /* If the processes arrival time is the same, compare the CPU burst times of each process. Else, break the cycles
                     in order to decrease the execution time. Since this function is used only to sort the processes which arrive at the
                     very beginning, this can be done without compromising the results */
                    if (processList[i].arrivalTime == processList[j].arrivalTime){
                        if (processList[i].cpuBurst == processList[j].cpuBurst){
                            if (processList[i].pID > processList[j].pID){
                                struct process temporal = processList[i]; /* Create a temporal structure that stores a process
                                                                           in order to not lose it during the exchange */
                                
                                processList[i] =  processList[j];
                                processList[j] = temporal;
                            }
                        }
                        /* Exchange the processes if the CPU burst of the second process is lower than the first one */
                        else if(processList[i].cpuBurst > processList[j].cpuBurst){
                            struct process temporal = processList[i]; /* Create a temporal structure that stores a process
                                                                       in order to not lose it during the exchange */
                            processList[i] =  processList[j];
                            processList[j] = temporal;
                        }
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
                    /* If the processes arrival time is the same, compare the priority of each process. Else, break the cycles
                     in order to decrease the execution time. Since this function is used only to sort the processes which arrive at the
                     very beginning, this can be done without compromising the results */
                    if (processList[i].arrivalTime == processList[j].arrivalTime){
                        /* If the priority of the processes are the same, keep at top
                         the process who it's Process ID is less */
                        if (processList[i].priority == processList[j].priority){
                            if (processList[i].pID > processList[j].pID){
                                struct process temporal = processList[i]; /* Create a temporal structure that stores a process
                                                                           in order to not lose it during the exchange */
                                
                                processList[i] =  processList[j];
                                processList[j] = temporal;
                            }
                        }
                        /* Exchange the processes if the CPU burst of the second process is higher than the first one */
                        else if(processList[i].priority > processList[j].priority){
                            struct process temporal = processList[i]; /* Create a temporal structure that stores a process
                                                                  in order to not lose it during the exchange */
                            processList[i] =  processList[j];
                            processList[j] = temporal;
                        }
                    }
                }
            }
    }
}

/*************************************************************************/
/*                                                                       */
/*  Function: Sort                                                       */
/*                                                                       */
/*  Purpose: This function sorts the process list considering different  */
/*           conditions for the sort. All the processes are sorted       */
/*           using the 'Bubble Sort' algorithm. Actually, this function  */
/*           does the same as SortListProcess, but it is needed since    */
/*           in this function the start and the end condition are        */
/*           not constant, and are recieved by the function              */
/*                                                                       */
/*  Parameters:                                                          */
/*            Input :   The sort condition, the start and the end        */
/*                      parameters of the sorting                        */
/*                                                                       */
/*            Output:   The main process list ordered by the condition   */
/*                      that have been determined earlier, but may not   */
/*                      be considering all the processes in the list for */
/*                      the sort                                         */
/*                                                                       */
/*************************************************************************/
void Sort(int sortBy, int start, int end){
    switch (sortBy) {
            
        /* This case considers only the CPU burst time for the sort */
        case CPUBURST:
            /* The process list will be sorted considering a start parameter 
            and an end paramater */
            for (int i = start; i < end; i++){
                for (int j = i + 1; j < end; j++){
                    /* If the CPU burst of the processes are the same, keep at top
                     the process who it's Process ID is less */
                    if (processList[i].cpuBurst == processList[j].cpuBurst){
                        if (processList[i].pID > processList[j].pID){
                            struct process temporal = processList[i]; /* Create a temporal structure that stores a process
                                                                       in order to not lose it during the exchange */

                            processList[i] =  processList[j];
                            processList[j] = temporal;
                        }
                    }
                    /* Exchange the processes if the CPU burst time of the first process is higher than the second process */
                    else if (processList[i].cpuBurst > processList[j].cpuBurst){
                        struct process temporal = processList[i]; /* Create a temporal structure that stores a process
                                                                   in order to not lose it during the exchange */

                        processList[i] =  processList[j];
                        processList[j] = temporal;
                    }
                }
            }
            break;
            
        case PRIORITY:
            for (int i = start; i < end; i++){
                for (int j = i + 1; j < end; j++){
                    /* If the priority of the processes are the same, keep at top
                     the process who it's Process ID is less */
                    if (processList[i].priority == processList[j].priority){
                        if (processList[i].pID > processList[j].pID){
                            struct process temporal = processList[i]; /* Create a temporal structure that stores a process
                                                                       in order to not lose it during the exchange */

                            processList[i] =  processList[j];
                            processList[j] = temporal;
                        }
                    }
                    /* Exchange the processes if the Priority of the first process is lower than the second process */
                    else if (processList[i].priority > processList[j].priority){
                        struct process temporal = processList[i]; /* Create a temporal structure that stores a process
                                                                  in order to not lose it during the exchange */

                        processList[i] =  processList[j];
                        processList[j] = temporal;
                    }
                }
            }
            break;
    }

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
        printf(" --------------------------- ");
        
    }
}


/*************************************************************************/
/*                                                                       */
/*  Function: Treatment                                                  */
/*                                                                       */
/*  Purpose:  If a process does not arrive at time zero on preemptive    */
/*            algorithms, it's applied a treatment in order to calculate */
/*            the average waiting time considering that the first        */
/*            process arrives at time zero                               */
/*                                                                       */
/*  Parameters:                                                          */
/*            Input :   The process list                                 */
/*                                                                       */
/*            Output:   The arrival time of the first process            */
/*                                                                       */
/*************************************************************************/
int Treatment(){
    int firstArrival = 0; /* The arrival time of the first process */
    /* If the arrival time of the first process is not zero, then obtain it's arrival time */
    if (processList[0].arrivalTime > 0) {
        firstArrival = processList[0].arrivalTime;
        for (int i = 0; i < numberOfProcess; i++) {
            /* Substract the arrival time of the first process to every process in the list (The actual 
             treatment) */
            processList[i].arrivalTime = processList[i].arrivalTime - firstArrival;
        }
    }
    
    return firstArrival;
}

/*************************************************************************/
/*                                                                       */
/*  Function: InverseTreatment                                           */
/*                                                                       */
/*  Purpose:  If a process does not arrive at time zero on preemptive    */
/*            algorithms, it's applied a treatment in order to calculate */
/*            the average waiting time considering that the first        */
/*            process arrives at time zero. This function returns the    */
/*            process list to it's original state of arrival times, with */
/*            the purpose to be printed on the context changes timeline  */
/*                                                                       */
/*  Parameters:                                                          */
/*            Input :   The arrival time of the process and the number   */
/*                      of processes that are in the list                */
/*                                                                       */
/*            Output:   The list within it's original arrival times      */
/*                                                                       */
/*************************************************************************/
void InverseTreatment(int firstArrival,  int numberOfProcesses){
    for (int i = 0; i < numberOfProcesses; i++)
        /* Since this is only used for the context changes timeline, the arrival times are not modified 
         directly, instead, are modified through the first executed parameter */
        contextProcessList[i].firstExecuted = contextProcessList[i].firstExecuted + firstArrival;
}

/*************************************************************************/
/*                                                                       */
/*  Function: CopyProcessList                                            */
/*                                                                       */
/*  Purpose:  Create a copy of the main list of processes                */
/*                                                                       */
/*  Parameters:                                                          */
/*            Input :   The main process list                            */
/*                                                                       */
/*            Output:   An intact copy of the main process list          */
/*                                                                       */
/*************************************************************************/
void CopyProcessList(){
     /* Copy all elements in the list*/
    for (int i = 0; i < numberOfProcess; i++)
        processListCopy[i] = processList[i];
}

/*************************************************************************/
/*                                                                       */
/*  Function: RestoreProcessList                                         */
/*                                                                       */
/*  Purpose:  Restore the main list of processes through the contents    */
/*            of a previous made copy                                    */
/*                                                                       */
/*  Parameters:                                                          */
/*            Input :   The main process list                            */
/*                                                                       */
/*            Output:   The main process list within it's original       */
/*                      contents                                         */
/*                                                                       */
/*************************************************************************/
void RestoreProcessList(){
    for (int i = 0; i < numberOfProcess; i++)
        processList[i] = processListCopy[i];
}

/*************************************************************************/
/*                                                                       */
/*  Function: WaitTime                                                   */
/*                                                                       */
/*  Purpose: Obtains the average wait time of all the processes. It      */
/*           is in charge to obtain the waiting time and the first       */
/*           execution of every process                                  */
/*                                                                       */
/*  Parameters:                                                          */
/*            Input :   The process list, the number of processes that   */
/*                      it contains and if, it's called on a preemptive  */
/*                      algorithm, the arrival time of the first process */
/*                                                                       */
/*            Output:   The average wait time of all the processes       */
/*                                                                       */
/*************************************************************************/
void WaitTime(int numberOfProcesses, struct process processList[], int firstArrival){
    
    
    float averageWaitTime = 0.0; /* The variable that will store the average
                                waiting time */
    
    
    processList[0].waitTime = 0; /* Since the first process in the list always
                                  executes at arrival, it's waiting time it's always zero */
    
    processList[0].firstExecuted = processList[0].arrivalTime; /* The first time that the
                                                                initial process is executed is
                                                                when it arrives */
    for (int i = 1; i < numberOfProcesses; i++) {
        
        /* The first time that a process is executed is when previous processes finishes its execution */
        processList[i].firstExecuted = processList[i-1].cpuBurst + processList[i-1].firstExecuted;
        
        /* The wait time of a process can be found if at its first execution it is substracted the process arrival time */
        processList[i].waitTime = processList[i].firstExecuted - processList[i].arrivalTime - processList[i - 1].lastExecuted;;
    }
    
    for (int i = 0; i < numberOfProcesses; i++){
        /* All of the waiting times of each process is obtained and
         stored in one only variable */
        averageWaitTime = averageWaitTime + processList[i].waitTime;
    }
    
    /* The sum is divided by the number of processes in order to obtain 
    the average waiting time */
    averageWaitTime = averageWaitTime / numberOfProcess;
    
    /* Since the average waiting time of the preemptive algorithms, is calculated considering that the first process arrives 
     at time zero, then if the arrival time of the first process is not zero, a treatment to the data is applied in order to
     print the actual times of the context changes */
    if (firstArrival != 0)
        InverseTreatment(firstArrival, numberOfProcesses);

    ContextChanges(numberOfProcesses, processList); /* Print the context changes */
    printf("\n|                           |");
    printf("\n| Average Wait Time:  %.2f  |\n", averageWaitTime); /* The average waiting time is shown */
    printf("|                           |\n");
    printf(" ---------------------------\n\n");
}

/*************************************************************************/
/*                                                                       */
/*  Function: FirstComeFS                                                */
/*                                                                       */
/*  Purpose: Generates the wait time for each of the processes using     */
/*           the First Come First Served algorithm                       */
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
    
    printf(" ---------------------------\n");
    printf("|  First Come First Served  |\n");
    printf(" ---------------------------");

    WaitTime(numberOfProcess, processList, 0); /* The average waiting time is obtained */
}

/*************************************************************************/
/*                                                                       */
/*  Function: NonPreemptive                                              */
/*                                                                       */
/*  Purpose: Generates the wait time for each of the processes using     */
/*           the Non Preemptive algorithms                               */
/*                                                                       */
/*  Parameters:                                                          */
/*            Input :   The process list which in will be computing the  */
/*                      average wait time and the algorithm that is      */
/*                      going to be used for the procedure               */
/*                                                                       */
/*            Output:   All the process list information considering the */
/*                      average wait time and the context changes        */
/*                                                                       */
/*************************************************************************/
void NonPreemptive(int algorithm){
    
    int lastProcess = 0; /* This variable stores which will be the last process consider in the sorting */
    int burst = 0; /* This stores an accumulated of the CPU burst times of the processes in order to know
                    which processes have arrived and can be included in the sorting */
    SortProcessList(ARRIVALTIME); /* Order all the processes in the list considering it's arrival time */
    
    switch (algorithm) {
        case CPUBURST:
            /* Print a header */
            printf(" ---------------------------\n");
            printf("|     SJF Non Preemptive    |\n");
            printf(" ---------------------------");
            
            /* In case there are processes in the list that arrives on the same time at the very beginning, order
            only them considering the lowest CPU burst time of each other */
            SortProcessList(CPUBURST);
        break;
            
        case PRIORITY:
            /* Print a header */
            printf(" ---------------------------\n");
            printf("|  Priority Non Preemptive  |\n");
            printf(" ---------------------------");
            
            /* In case there are processes in the list that arrives on the same time at the very beginning, order
             only them considering the highest priority of each other */
            SortProcessList(PRIORITY);
        break;
    }
    
    /* In the Non Preemptive algorithms, the order of execution of the processes can be seen as a simple reordered list. In that case, 
     the processes are ordered considering it's arrival time versus the processes who have been executed already or are being executed */
    for (int i = 0; i < numberOfProcess; i++) {
        
        lastProcess = 0; /* Reset the last process variable */
        burst += processList[i].cpuBurst; /* Store an accumulated of the CPU burst times of the processes in order to know
                                                  which processes have arrived and can be included in the sorting */
        
        for (int j = i + 1; j < numberOfProcess; j++) {
            /* If the accumulated of the burst is lower than the arrival time of the next process, it means that that process
             have not arrived and can not be included in the sorting */
            
            if (burst < processList[j].arrivalTime) {
                lastProcess = j; /* Indicate which will be the last process consider for the sorting */
                j = numberOfProcess; /* Break the inner cycle. Since the processes are ordered initially considering it's arrival time, if one 
                                      process arrives after the execution of another process, then all the processes which came after, will
                                      have a higher arrival time and there is no reason to consider them again */
            }
        }
        /* If the last process is zero, then it means that all processes can be included in the sort since them have already arrived */
        if (lastProcess == 0) {
            Sort(algorithm, i + 1, numberOfProcess); /* Sort the process considering the algorithm that have been indicated earlier. Since
                                                      the "i" process is already being executed, then the start parameter for the sorting,
                                                      is the next process. All the processes are being consider for the sorting, so the 
                                                      end parameter corresponds to the number of processes that are in the main list */
            
            i = numberOfProcess; /* Finish the ordering process since we can conclude that all the processes have arrived and have been
                                  included in the sort */
        }
        else
            Sort(algorithm, i + 1, lastProcess); /* Sort the process considering the algorithm that have been indicated earlier. Since
                                                  the "i" process is already being executed, then the start parameter for the sorting,
                                                  is the next process. It is indicated which will be the last process considered in the
                                                  sorting */
    }
    
    WaitTime(numberOfProcess, processList, 0); /* Calculate the average waiting time */
}

/*************************************************************************/
/*                                                                       */
/*  Function: Preemptive                                                 */
/*                                                                       */
/*  Purpose: Generates the wait time for each of the processes using     */
/*           the Preemptive algorithms                                   */
/*                                                                       */
/*  Parameters:                                                          */
/*            Input :   The process list which in will be computing the  */
/*                      average wait time and the algorithm that is      */
/*                      going to be used for the procedure               */
/*                                                                       */
/*            Output:   All the process list information considering the */
/*                      average wait time and the context changes        */
/*                                                                       */
/*************************************************************************/
void Preemptive(int algorithm){
    
    SortProcessList(ARRIVALTIME); /* Order all the processes in the list considering it's arrival time */
    CopyProcessList(); /* Since the main process list it's going to change, it's created a copy of it */
    
    int firstArrival = Treatment(); /* It's obtained the arrival time of the first process */
    
    int burst = 0; /* This stores an accumulated of the CPU burst times of the processes in order to know
                    which processes have arrived and can be included in the sorting */
    
    int processRunning = 0; /* This variable indicates which process is being executed and is used to know
                             how many processes have finished it's execution */
    
    int interrumptions = 0; /* This counter indicates how many interrumptions have take place through all the process */
    
    int lastProcess = 0;  /* This variable stores which will be the last process consider in the sorting */
    
    int doubleInterrumptions = 0; /* This counter stores how many interruptions of the interruptions are made */
    
    int displacer = 0; /* This displacer is used in order to compare new incoming processes */
    
    int time = 0; /* This variable stores all the execution time at a specific point */
    
    int displacerCondition = 1; /* This is a switch indicating if a displace must be donde in order to not compromise the correct
                                 functionality of the procedure */
    
    
    switch (algorithm) {
        case CPUBURST:
            /* Print a header */
            printf(" ---------------------------\n");
            printf("|       SJF Preemptive      |\n");
            printf(" ---------------------------");
            
            /* In case there are processes in the list that arrives on the same time at the very beginning, order
             only them considering the lowest CPU burst time of each other */
            SortProcessList(CPUBURST);
            
            /* Repeat the procedure until the procesess completed are equal to the number of processes in the list minus 1 */
            while (processRunning < numberOfProcess - 1) {
                /* Reset the variables */
                lastProcess = 0;
                burst = 0;
                doubleInterrumptions = 0;
                displacer = 0;
                
                /* Store an accumulated of the CPU burst times of the processes of a second process list in order to know
                 which processes have arrived and can be included in the sorting */
                for (int i = 0; i < processRunning + interrumptions; i++) {
                    burst += contextProcessList[i].cpuBurst;
                }
                /* If it's the first time that this procedure is being executed, ignore it. Else, indicate how many process have arrived
                 and can be included in the sorting through the last process variable */
                if (burst != 0) {
                    
                    /* Start in the number of the process running, since we do not want to include in the sorting, processes that have
                     been already executed totally */
                    for (int k = processRunning; k < numberOfProcess; k++) {
                        if (burst >= processList[k].arrivalTime)
                            lastProcess = k; /* Indicate which will be the last process consider for the sorting */
                    }
                    
                    /* If the last process is not zero, then it means that a sorting must be done, since we must indicate which will
                     be the next process to be executed */
                    if(lastProcess != 0)
                        Sort(CPUBURST, processRunning, lastProcess + 1);/* Sort the process considering the algorithm that have been indicated earlier.                  The process that is going to be executed must be included in the sorting. It is indicated which will be the last process considered in the sorting. Since the comparison in the sorting is made through the operator '<' a '+1' is send in the parameter section */
                }
                
                /* Initiate the reordering and interruptions procedure. This will simulate that the processes are being executed over time
                 so this will be done until all the cpu burst time of a process expires. Since the operator used for the comparison is a '<' a "+1" is
                 included in the comparison */
                for (int j = 1; j < processList[processRunning].cpuBurst + 1; j++) {
                    
                    time++; /* Increase the time */
                    
                    processList[processRunning].lastExecuted = j; /* This indicates how much time the process have been executed */
                    
                    /* If the execution time is higher or equal to the next process arrival time, then it means that process have arrived at
                     this point, so this one can either interrupt or not the process that being executed. The "doubleInterrumptions", the "displacer" and
                     the "processRunning" variables helps to indicate the next better process that can be compared to */
                    if (time >= processList[processRunning + 1 + doubleInterrumptions + displacer].arrivalTime) {
                        
                        /* If the CPU burst time of the process that is being executed, consider the time that has already executed, is higher than
                         the next better process in the list, there must be an interruption */
                        if (processList[processRunning].cpuBurst - j > processList[processRunning + 1 + doubleInterrumptions + displacer].cpuBurst) {
                            
                            /* Store the information of the process that is being executed on a new process list. The "interruptions" let us
                             create new information about processes interrupted in the list */
                            contextProcessList[processRunning + interrumptions] = processList[processRunning];
                            
                            /* Since this code is only executed when there is an interruption, we must indicate that the new process in the new list,
                             have only executed a partial time of it's CPU burst, which is stored in the last executed parameter */
                            contextProcessList[processRunning + interrumptions].cpuBurst = contextProcessList[processRunning + interrumptions].lastExecuted;
                            
                            /* It's indicated that the CPU burst now have reduced since it have been executed before "j" time unities */
                            processList[processRunning].cpuBurst -= j;
                            
                            /* Create a temporal structure that stores a process
                             in order to not lose it during the exchange */
                            struct process temporal = processList[processRunning];
                            
                            /* Exchange the process which is running with which one who have interrupted */
                            processList[processRunning] =  processList[processRunning + 1 + displacer + doubleInterrumptions];
                            processList[processRunning + 1 + displacer + doubleInterrumptions] = temporal;
                            
                            doubleInterrumptions++; /* Increase this variable to indicate that an interruption have been made and the next process in
                                                     the list must not be compared to */
                            
                            /* If the arrival time of the next process is the same, then you increase the variable "doubleInterruptions". Since
                             the processes with the same arrival time have been ordered before, and the next process in the list could have an arrival
                             time of just one unity of time higher than the process that is going to execute, we must validate it's information, not
                             the process that we already know it has a higher CPU burst time */
                            if(processList[processRunning].arrivalTime == processList[processRunning + 1 + displacer + doubleInterrumptions].arrivalTime)
                                doubleInterrumptions++;
                            
                            j = 0; /* Reset this variable since a new process is going to execute */
                            displacer = 0; /* Reset this variable for the same reason as before */
                            interrumptions++; /* Indicate that a new interrumption has been made */
                        }
                        else{
                            /* If the process that we are comparing to have been arrived but it has a higher CPU burst time, there is no reason
                             to be stuck at it, so a displacer variable is increased. Since this can overflow the process list elements, it is
                             used the mod function. It must be substracted the "processRunning" and the "doubleInterruptions" since there can
                             overflow the process list too */
                            displacer = (displacer + 1) % (numberOfProcess - (processRunning + doubleInterrumptions + 1));
                        }
                    }
                    /* Since there can be a lot of interruptions of interruptions, we must return to it's initial value in order to not
                     overflow the main process list. It is compared to the "number of process - 1" since this is the value which will overflow the
                     process list */
                    if (doubleInterrumptions >= numberOfProcess - 1) {
                        doubleInterrumptions = 0; /* Reset this variable */
                    }
                }
                
                /* If the CPU burst of the process that is being executed is equals to the unities that have been executed, then this means
                 that the process have finished it's execution time */
                if (processList[processRunning].cpuBurst == processList[processRunning].lastExecuted) {
                    /* We store the process information in the other list */
                    contextProcessList[processRunning + interrumptions] = processList[processRunning];
                    
                    /* The last executed parameter is deleted, since this is used for the calcutions of the average time and it must be only
                     kept during interruptions */
                    contextProcessList[processRunning + interrumptions].lastExecuted = 0;
                    processRunning++; /* It's indicated that a process have finished */
                }
            }
        break;
            
        case PRIORITY:
            /* Print a header */
            printf(" ---------------------------\n");
            printf("|    Priority Preemptive    |\n");
            printf(" ---------------------------");
            
            
            /* In case there are processes in the list that arrives on the same time, order
             only them considering the highest priority of each other */
            SortProcessList(PRIORITY);

            /* Repeat the procedure until the procesess completed are equal to the number of processes in the list minus 1 */
            while (processRunning < numberOfProcess - 1) {
                /* Reset the variables */
                lastProcess = 0;
                burst = 0;
                doubleInterrumptions = 0;
                displacer = 0;
                displacerCondition = 1;
                
                /* Store an accumulated of the CPU burst times of the processes of a second process list in order to know
                 which processes have arrived and can be included in the sorting */
                for (int i = 0; i < processRunning + interrumptions; i++) {
                    burst += contextProcessList[i].cpuBurst;
                }
                /* If it's the first time that this procedure is being executed, ignore it. Else, indicate how many process have arrived
                 and can be included in the sorting through the last process variable */
                if (burst != 0) {
                    
                    /* Start in the number of the process running, since we do not want to include in the sorting, processes that have
                     been already executed totally */
                    for (int k = processRunning; k < numberOfProcess; k++) {
                        if (burst >= processList[k].arrivalTime)
                            lastProcess = k; /* Indicate which will be the last process consider for the sorting */
                    }
                    
                    /* If the last process is not zero, then it means that a sorting must be done, since we must indicate which will
                     be the next process to be executed */
                    if(lastProcess != 0)
                        Sort(PRIORITY, processRunning, lastProcess + 1);/* Sort the process considering the algorithm that have been indicated earlier. The process that is going to be executed must be included in the sorting. It is indicated which will be the last process considered in the sorting. Since the comparison in the sorting is made through the operator '<' a '+1' is send in the parameter section */
                }
                
                
                /* This is a condition indicating if a displace must be donde in order to not compromise the correct
                 functionality of the procedure. It is considered that if the next process have already been executed, and it's found after
                 the process that is consider to be executed next, then ignore it. Also, if the next process has the same time than the process
                 to be executed, it have been already validated that process has a higher CPU burst time, so it could be ignored in order
                 to start with the next process. The condition of the "processRunning < numberOfProcess - 2 - displacer" is used in order to 
                 evade overflow conditions in the process list */
                while (displacerCondition == 1) {
                    if ((processList[processRunning + 1 + doubleInterrumptions + displacer].lastExecuted != 0 && processRunning < numberOfProcess - 2 - displacer) ||  (processList[processRunning].arrivalTime == processList[processRunning + 1 + doubleInterrumptions + displacer].arrivalTime && processRunning < numberOfProcess - 2 - displacer)){
                        displacer++;
                    }
                else
                    displacerCondition = 0;
                }
                /* Initiate the reordering and interruptions procedure. This will simulate that the processes are being executed over time
                 so this will be done until all the cpu burst time of a process expires. Since the operator used for the comparison is a '<' a "+1" is
                 included in the comparison */
                for (int j = 1; j < processList[processRunning].cpuBurst + 1; j++) {
                    
                    time++; /* Increase the time */
                    
                    processList[processRunning].lastExecuted = j; /* This indicates how much time the process have been executed */
                    
                    /* If the execution time is higher or equal to the next process arrival time, then it means that process have arrived at
                     this point, so this one can either interrupt or not the process that being executed. The "doubleInterrumptions", the "displacer"
                     and the "processRunning" variables helps to indicate the next better process that can be compared to */
                    if (time >= processList[processRunning + 1 + doubleInterrumptions + displacer].arrivalTime) {
                        
                        /* If the priority of the process that is being executed, is lower than the next better process in the list, there must be an
                         interruption */
                        if (processList[processRunning].priority > processList[processRunning + 1 + doubleInterrumptions + displacer].priority) {
                            
                            /* Store the information of the process that is being executed on a new process list. The "interruptions" let us
                             create new information about processes interrupted in the list */
                            contextProcessList[processRunning + interrumptions] = processList[processRunning];
                            
                            /* Since this code is only executed when there is an interruption, we must indicate that the new process in the new list, have only executed a partial time of it's CPU burst, which is stored in the last executed parameter */
                            contextProcessList[processRunning + interrumptions].cpuBurst = contextProcessList[processRunning + interrumptions].lastExecuted;
                            
                            /* It's indicated that the CPU burst now have reduced since it have been executed before "j" time unities */
                            processList[processRunning].cpuBurst -= j;
                            
                            /* Create a temporal structure that stores a process
                             in order to not lose it during the exchange */
                            struct process temporal = processList[processRunning];
                            
                            /* Exchange the process which is running with which one who have interrupted */
                            processList[processRunning] =  processList[processRunning + 1 + displacer + doubleInterrumptions];
                            processList[processRunning + 1 + displacer + doubleInterrumptions] = temporal;
                            
                            doubleInterrumptions++; /* Increase this variable to indicate that an interruption have been made and the next process in the list must not be compared to */
                            
                            /* If the arrival time of the next process is the same, then you increase the variable "doubleInterruptions". Since
                             the processes with the same arrival time have been ordered before, and the next process in the list could have an arrival
                             time of just one unity of time higher than the process that is going to execute, we must validate it's information, not
                             the process that we already know it has a higher CPU burst time */
                            if(processList[processRunning].arrivalTime == processList[processRunning + 1 + displacer + doubleInterrumptions].arrivalTime)
                                doubleInterrumptions++;
                            
                            j = 0; /* Reset this variable since a new process is going to execute */
                            displacer = 0; /* Reset this variable for the same reason as before */
                            interrumptions++; /* Indicate that a new interrumption has been made */
                        }
                        else{
                            /* If the process that we are comparing to have been arrived but it has a higher CPU burst time, there is no reason
                             to be stuck at it, so a displacer variable is increased. Since this can overflow the process list elements, it is
                             used the mod function. It must be substracted the "processRunning" and the "doubleInterruptions" since there can
                             overflow the process list too */
                            displacer = (displacer + 1) % (numberOfProcess - (processRunning + doubleInterrumptions + 1));
                        }
                    }
                    /* Since there can be a lot of interruptions of interruptions, we must return to it's initial value in order to not
                     overflow the main process list. It is compared to the "number of process - 1" since this is the value which will overflow the
                     process list */
                    if (doubleInterrumptions >= numberOfProcess - 1) {
                        doubleInterrumptions = 0; /* Reset this variable */
                    }
                }
                
                /* If the CPU burst of the process that is being executed is equals to the unities that have been executed, then this means
                 that the process have finished it's execution time */
                if (processList[processRunning].cpuBurst == processList[processRunning].lastExecuted) {
                    /* We store the process information in the other list */
                    contextProcessList[processRunning + interrumptions] = processList[processRunning];
                    
                    /* The last executed parameter is deleted, since this is used for the calcutions of the average time and it must be only
                     kept during interruptions */
                    contextProcessList[processRunning + interrumptions].lastExecuted = 0;
                    processRunning++; /* It's indicated that a process have finished */
                }
            }
            
        break;
            
    }
    
    /* Since this is the last process in the main list, we just include it in the other process list */
    contextProcessList[processRunning + interrumptions] = processList[processRunning];
    
    /* The average waiting time is calculated considering the other process list that is created in the procedure. It's dimensions
     are explicitly indicated as a parameter, and the first arrival for the inverse treatment is send to */
    WaitTime(processRunning + interrumptions + 1, contextProcessList, firstArrival);
    
    /* Due to the changes that are made through this procedure, the main process list is returned to it's original state */
    RestoreProcessList();
    
}

/*************************************************************************/
/*                                                                       */
/*  Function: RoundRobin                                                 */
/*                                                                       */
/*  Purpose: Generates the wait time for each of the processes using     */
/*           the Round Robin algorithm. Furthermore, the context changes */
/*           are printed and shown to the user                           */
/*                                                                       */
/*  Parameters:                                                          */
/*            Input :   The process list which in will be computing the  */
/*                      average wait time and the quantum                */
/*                                                                       */
/*            Output:   All the process list information considering the */
/*                      average wait time and the context changes        */
/*                                                                       */
/*************************************************************************/
void RoundRobin(int quantum){
    
    printf(" ---------------------------\n");
    printf("|        Round Robin        |\n");
    printf(" ---------------------------");
    
    int processesCompleted = 0; /* This variable indicates which process is being executed and is used to know
                                 how many processes have finished it's execution */

    int difference = 0; /* This variable indicates if a process finishes it's execution before it's quantum expires */
    int time = processList[0].arrivalTime; /* This variable stores all the execution time at a specific point. The arrival time
                                            of the first process indicates at which moment the processes begin their execution */
    
    float averageWaitTime = 0.0; /* The variable that will store the average
                                  waiting time */
    
    CopyProcessList(); /* Since the main process list it's going to change, it's created a copy of it */
    
     /* Repeat the procedure until the procesess completed are equal to the number of processes in the list */
    while (processesCompleted < numberOfProcess) {
        /* Print the context changes */
        printf("\n|        t    =   %2d        |\n", time);
        /* This indicates which process is running at a specific time */
        printf("|        Process: %2d        |\n", processList[0].pID);
        printf(" ---------------------------");
        
        /* The CPU burst less the quantum is stored */
        difference = processList[0].cpuBurst - quantum;
        /* If the difference is positive, that means that the process have not finished yet */
        if (difference > 0) {
            time += quantum; /* The quantum it's added to the total execution time */
            processList[0].cpuBurst = difference /* The CPU burst now is less, since have executed an specific quantum */;
            /* It is found in the copy of the process list the process that have been executed, in order to indicate that */
            for (int i = 0; i < numberOfProcess; i++) {
                if (processList[0].pID == processListCopy[i].pID) {
                    processListCopy[i].lastExecuted++; /* Increase by one the last execution */
                }
            }
            
            /* Send the process that have been executed to the last position in the queue. It is indicated that this must be done
             considering the process that are already completed. This in order to not include garbage that are at the last positions
             in the list */
            for (int j = 1; j < numberOfProcess - processesCompleted; j++) {
                if (time >= processList[j].arrivalTime) {
                    
                    /* Create a temporal structure that stores a process
                     in order to not lose it during the exchange */
                    struct process temporal = processList[j - 1];
                    /* It is denoted a "j - 1" since the process which the exchange it is going to be done, is at the previous position */
                    processList[j - 1] =  processList[j];
                    processList[j] = temporal;
                }
            }
        }
        /* If the difference is negative or zero, then the process already finished it's execution */
        else {
            /* It is found in the copy of the process list the process that have been executed, in order to indicate it's first
             execution time */
                for (int i = 0; i < numberOfProcess; i++) {
                if (processList[0].pID == processListCopy[i].pID) {
                    processListCopy[i].firstExecuted = time;
                }
            }
            /* Since the difference is negative, it must be added to the quantum in order the actual execution time of the process */
            time += quantum + difference;
            processesCompleted++; /* It's indicated that a process finished it's execution procedure */
            for (int k = 1; k < numberOfProcess; k++) {
                /* The processes are hauled at the top and the process at the top position it's "destroyed" since none of it's information 
                 it is stored elsewhere */
                processList[k - 1] =  processList[k];
            }
        }
    }
    
    /* The sum of the waiting times of the processes is found */
    for (int i = 0; i < numberOfProcess; i++) {
        /* The "classical" solution it's applied, the only thing to consider it's to multiply the last executions to the quantum, due to the fact
         that the last execution unities indicate only a fully section with full quantum already executed */
        processListCopy[i].waitTime = processListCopy[i].firstExecuted - processListCopy[i].arrivalTime - (processListCopy[i].lastExecuted * quantum);
        averageWaitTime = averageWaitTime + processListCopy[i].waitTime; /* The waiting times are added to the accumulated variable */
    }
    
    /* The sum is divided by the number of processes in order to obtain
     the average waiting time */
    averageWaitTime /= numberOfProcess;

    printf("\n|                           |");
    printf("\n| Average Wait Time:  %2.2f  |\n", averageWaitTime); /* The average waiting time is shown */
    printf("|                           |\n");
    printf(" ---------------------------\n\n");
}