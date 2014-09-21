/*************************************************************************/
/*                                                                       */
/* Copyright (c) 2012 Abelardo Lopez Lagunas                             */
/*                                                                       */
/* File name: Scheduler.h                                                */
/*                                                                       */
/* Author:                                                               */
/*          Abelardo Lopez Lagunas                                       */
/*                                                                       */
/* Edited by:                                                            */
/*          Mario García Navarro                                         */
/*                                                                       */
/* Purpose:                                                              */
/*          This program implements the basic process scheduling         */
/*          algorithms for the TC2008 class.                             */
/*                                                                       */
/* Usage:   The program reads a text file with the processes. The first  */
/*          integer in the file is the quantum, followed by four integer */
/*          numbers per line describing the process id, arrival time,    */
/*          cpu burst and priority. The usage form is:                   */
/*                                                                       */
/*          schedule file.txt                                            */
/*                                                                       */
/* References:                                                           */
/*          The material that describe the scheduling algorithms is      */
/*          covered in the class notes for TC2008                        */
/*                                                                       */
/* File formats:                                                         */
/*          The input file should have four numbers per list in ASCII    */
/*          format. The exception is the first line which only has one   */
/*          integer number that represents the quantum                   */
/*                                                                       */
/* Restrictions:                                                         */
/*          If the input file is not in ASCII format the program exits   */
/*                                                                       */
/* Revision history:                                                     */
/*                                                                       */
/*          Feb 16 11:57 2011 - File created                             */
/*                                                                       */
/*          May 24 11:56 2012 - Code refactoring & big fixes             */
/*                                                                       */
/* Error handling:                                                       */
/*          On any unrecoverable error, the program exits                */
/*                                                                       */
/* Notes:                                                                */
/*          This code presents a solution for the first assignment       */
/*                                                                       */
/*************************************************************************/

#include <stdio.h>                       /* Used for the function printf */
#include <stdlib.h>                        /* Used for malloc definition */
#include "FileIO.h"       /* Definition of file access support functions */
#include "Process.h"           /* Definition of processes creation and
                            computing operations that find the average time 
                                                      for each algorithm */

/*************************************************************************/
/*                         Global constant values                        */
/*************************************************************************/

#define NUMPARAMS 2
#define NUMVAL    4 /* Number of columns in the processes */

/*************************************************************************/
/*                            Main entry point                           */
/*************************************************************************/

int main (int argc, const char * argv[]) {

    FILE   *fp;                                  /* Pointer to the file */
    int    quantum = 0;                /* Quantum value for round robin */
    int    parameters[NUMVAL];        /* Process parameters in the line */
    int    i;                    /* Number of parameters in the process */
    
    /* Check if the parameters in the main function are not empty */
    if (argc < NUMPARAMS){
        printf("Need a file with the process information\n");
        printf("Abnormal termination\n");
        return (EXIT_FAILURE);
    }
    else {
        /* Open the file and check that it exists */
        fp = fopen (argv[1],"r");       /* Open file for read operation */
        if (!fp)                            /* The file does not exists */
            ErrorMsg("main","Filename does not exist or is corrupted");
        else {
            /* The first number in the file is the quantum */
            quantum = GetInt(fp);
        
            if (quantum == EXIT_FAILURE)
                ErrorMsg("main","The quantum was not found");
            else {
                /*
                 * Read the process information until the end of file
                 * is reached.
                 */
                while (!feof(fp)){
                    /* For every four parameters create a new process */
                    for (i = 0; ((i < NUMVAL) && (!feof(fp))); i++) {
                        parameters[i] = GetInt(fp);
                    }
                    
                    /* Do we have only four parameters? */
                    if (i == NUMVAL) {
                        /* Create a new process with its information */
                        CreateProcessList(parameters[0],parameters[1], parameters[2], parameters[3]);
                    }
                }
            }
        }
        
         /* Start by sorting the processes by arrival time */
        SortProcessList(ARRIVALTIME);
        
        /*
         * Apply all the scheduling algorithms and print the results
         */
        
        FirstComeFS();
        NonPreemptive();
        NonPreemptivePriority();
        
    }
}