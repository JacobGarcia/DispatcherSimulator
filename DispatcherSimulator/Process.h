/*************************************************************************/
/*                                                                       */
/* Copyright (c) 2014 Mario García Navarro. All rights reserved.         */
/*                                                                       */
/* File name: Process.h                                                  */
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


/************************************************************************/
/*                  Declare the function prototypes                     */
/************************************************************************/

enum processKeys {PID = 0, PRIORITY, CPUBURST, ARRIVALTIME, ARRIVALBURSTTIME, ARRIVALPRIORITYTIME};

void CreateProcessList(int pID, int arrivalTime, int cpuBurst, int priority);
void printProcesses();
void SortProcessList(int sortBy);
void FirstComeFS();
float waitTime();
void NonPreemptive();
void NonPreemptivePriority();
