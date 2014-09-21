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

void contextChanges(){
    for (int i = 0; i < numberOfProcess; i++) {
        printf("\nt  =  %d\n", processList[i].firstExecuted);
        printf("Process: %d", processList[i].pID);
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
            if (processList[i].cpuBurst > processList[j].cpuBurst){
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
            if (processList[i].priority > processList[j].priority){
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

void Preemptive(){
    /*int lastProcess = 0;
    int burst = 0;*/
    SortProcessList(ARRIVALBURSTTIME);
    processList[0].waitTime = 0;
    processList[0].firstExecuted = processList[0].arrivalTime;
}

#ifdef DEBUG
void printProcesses(){
    for (int i = 0; i < numberOfProcess; i++) {
        printf("%d", processList[i].pID);
        printf("%d", processList[i].arrivalTime);
        printf("%d", processList[i].cpuBurst);
        printf("%d", processList[i].priority);
    }
}
#endif