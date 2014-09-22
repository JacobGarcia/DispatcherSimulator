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
/*          Abelardo López Lagunas code shown during class lab           */
/*                                                                       */
/* Restrictions:                                                         */
/*                                                                       */
/* Revision history:                                                     */
/*          Sep  17 01:21 2014 -- File created                           */
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

#include <stdio.h>
#include "Process.h"                                   /* Function header */

#define NUMBERPROCESSES 10          /* The number of max processes that the
                                                    structure can handle */
int numberOfProcess = 0;

struct process{
    int pID;                                           /* The process ID */
    int priority;        /* This is important for the Priority Algorithm */
    int cpuBurst;                             /* Lenght of the CPU burst */
    int arrivalTime;                         /* Time of process creation */
    int waitTime;                        /* Time spent in the wait queue */
    int firstExecuted;                        /* Time of first execution */
    int lastExecuted;                          /* Time of last execution */
};

struct process processList[NUMBERPROCESSES];

struct process contextProcessList[NUMBERPROCESSES * 2];

struct process processListCopy[NUMBERPROCESSES];


void CreateProcessList(int pID, int arrivalTime, int cpuBurst, int priority){
    processList[numberOfProcess].pID = pID;
    processList[numberOfProcess].arrivalTime = arrivalTime;
    processList[numberOfProcess].cpuBurst = cpuBurst;
    processList[numberOfProcess].priority = priority;
    numberOfProcess++;
}

void SortProcessList(int sortBy){
    switch (sortBy) {
            
        case ARRIVALTIME:
            for (int i = 0; i < numberOfProcess; i++){
                for (int j = i + 1; j < numberOfProcess; j++){
                    if (processList[i].arrivalTime > processList[j].arrivalTime){
                        struct process temporal = processList[i];
                        processList[i] =  processList[j];
                         processList[j] = temporal;
                    }
                }
            }
            break;
            
        case ARRIVALBURSTTIME:
            for (int i = 0; i < numberOfProcess; i++){
                for (int j = i + 1; j < numberOfProcess; j++){
                    if (processList[i].arrivalTime == processList[j].arrivalTime && processList[i].cpuBurst > processList[j].cpuBurst){
                        struct process temporal = processList[i];
                        processList[i] =  processList[j];
                        processList[j] = temporal;
                    }
                }
            }
            break;
            
        case ARRIVALPRIORITYTIME:
            for (int i = 0; i < numberOfProcess; i++){
                for (int j = i + 1; j < numberOfProcess; j++){
                    if (processList[i].arrivalTime == processList[j].arrivalTime && processList[i].priority > processList[j].priority){
                        struct process temporal = processList[i];
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

float waitTime(){
    float averageWaitTime = 0;
    for (int i = 0; i < numberOfProcess; i++)
        averageWaitTime = averageWaitTime + processList[i].waitTime;
    
        #ifdef DEBUG
            printf("%f  ",averageWaitTime);
        #endif
        
    return averageWaitTime = averageWaitTime / numberOfProcess;
}

float waitTimePreemptive(int numberOfProcesses){
    float averageWaitTime = 0;
    for (int i = 0; i < numberOfProcesses; i++)
        averageWaitTime = averageWaitTime + contextProcessList[i].waitTime;
    
#ifdef DEBUG
    printf("%f  ",averageWaitTime);
#endif
    
    return averageWaitTime = averageWaitTime / numberOfProcess;
}

void contextChanges(){
    for (int i = 0; i < numberOfProcess; i++) {
        printf("\nt  =  %d\n", processList[i].firstExecuted);
        printf("Process: %d", processList[i].pID);
    }
}

void contextChangesPreemptive(int numberOfProcesses){
    for (int i = 0; i < numberOfProcesses; i++) {
        printf("\nt  =  %d\n", contextProcessList[i].firstExecuted);
        printf("Process: %d", contextProcessList[i].pID);
    }
}

void FirstComeFS(){
    processList[0].waitTime = 0;
    processList[0].firstExecuted = processList[0].arrivalTime;
    
    for (int i = 1; i < numberOfProcess; i++) {
        processList[i].firstExecuted = processList[i-1].cpuBurst + processList[i-1].firstExecuted;
        processList[i].waitTime = processList[i].firstExecuted - processList[i].arrivalTime;
    }
    
   
    float averageWaitTime = waitTime();
    
    printf("First Come First Served \nAverage Wait Time : %.2f", averageWaitTime);
    contextChanges();
}

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
    int lastProcess = 0;
    int burst = 0;
    SortProcessList(ARRIVALBURSTTIME);
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
    
    
    float averageWaitTime = waitTime();
    
    printf("SJF Non Preemptive \nAverage Wait Time : %.2f", averageWaitTime);
    contextChanges();

}

void NonPreemptivePriority(){
    int lastProcess = 0;
    int burst = 0;
    SortProcessList(ARRIVALPRIORITYTIME);
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
    
    
    float averageWaitTime = waitTime();
    
    printf("Priority Non Preemptive \nAverage Wait Time : %.2f", averageWaitTime);
    contextChanges();
    
}

void Treatment(){
    if (processList[0].arrivalTime > 0) {
        int difference = processList[0].arrivalTime;
        for (int i = 0; i < numberOfProcess; i++) {
            processList[i].arrivalTime = processList[i].arrivalTime - difference;
        }
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
    /*int lastProcess = 0;
    int burst = 0;*/
    SortProcessList(ARRIVALBURSTTIME);
    /*processList[0].waitTime = 0;
    processList[0].firstExecuted = processList[0].arrivalTime;*/
    
    copyProcessList();
    
    Treatment();
    int burst = 0;
    int processRunning = 0;
    int cont = 0;
    //int processesCompleted = 0;
    int lastProcess = 0;
    int contextSwitch = 0;
    
    while (processRunning < numberOfProcess - 1) {
        lastProcess = 0;
        burst = 0;
        contextSwitch = 0;
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
            if (j >= processList[processRunning + 1 + contextSwitch].arrivalTime) {
                if (processList[processRunning].cpuBurst - j > processList[processRunning + 1 + contextSwitch].cpuBurst) {
                    contextProcessList[processRunning] = processList[processRunning];
                    contextProcessList[processRunning].cpuBurst = contextProcessList[processRunning].lastExecuted;
                    processList[processRunning].cpuBurst = processList[processRunning].cpuBurst - j;
                    struct process temporal = processList[processRunning];
                    processList[processRunning] =  processList[processRunning + 1];
                    processList[processRunning + 1] = temporal;
                    j = 1;
                    //processRunning++;
                    cont++;
                    contextSwitch++;
                }
            }
        }
        
        if (processList[processRunning].cpuBurst == processList[processRunning].lastExecuted) {
           // processesCompleted++;
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
    
    
    float averageWaitTime = waitTimePreemptive(processRunning + cont + 1);
    
    printf("SJF Preemptive \nAverage Wait Time : %.2f", averageWaitTime);
    contextChangesPreemptive(processRunning + cont + 1);
    
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
    /*int lastProcess = 0;
     int burst = 0;*/
    SortProcessList(ARRIVALPRIORITYTIME);
    /*processList[0].waitTime = 0;
     processList[0].firstExecuted = processList[0].arrivalTime;*/
    Treatment();
    
    int burst = 0;
    int processRunning = 0;
    int cont = 0;
    //int processesCompleted = 0;
    int lastProcess = 0;
    int contextSwitch = 0;
    
    while (processRunning < numberOfProcess - 1) {
        lastProcess = 0;
        burst = 0;
        contextSwitch = 0;
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
            if (j >= processList[processRunning + 1 + contextSwitch].arrivalTime) {
                if (processList[processRunning].priority > processList[processRunning + 1 + contextSwitch].priority) {
                    contextProcessList[processRunning] = processList[processRunning];
                    contextProcessList[processRunning].cpuBurst = contextProcessList[processRunning].lastExecuted;
                    processList[processRunning].cpuBurst = processList[processRunning].cpuBurst - j;
                    struct process temporal = processList[processRunning];
                    processList[processRunning] =  processList[processRunning + 1];
                    processList[processRunning + 1] = temporal;
                    j = 1;
                    //processRunning++;
                    cont++;
                    contextSwitch++;
                }
            }
        }
        
        if (processList[processRunning].cpuBurst == processList[processRunning].lastExecuted) {
            // processesCompleted++;
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
    
    
    float averageWaitTime = waitTimePreemptive(processRunning + cont + 1);
    
    printf("Priority Preemptive \nAverage Wait Time : %.2f", averageWaitTime);
    contextChangesPreemptive(processRunning + cont + 1);
    
    restoreProcessList();
}

void RoundRobin(int quantum){
    
    int processesCompleted = 0;
    int difference = 0;
    int time = 0;
    int cont = 0;
    
    copyProcessList();
    
    while (processesCompleted < numberOfProcess) {
        printf("\nt  =  %d", time);
        printf("\nProcess:  %d\n", processList[0].pID);
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
    
     printf("Priority Preemptive \nAverage Wait Time : %.2f", averageWaitTime);
}