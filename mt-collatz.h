/*
 ============================================================================
 Name        : mt-collatz.h
 Author      : chris, Branden Sherrell
 Description : Header file for mt-collatz.c implementation
 ============================================================================
 */


#ifndef MT_COLLATZ_H_
#define MT_COLLATZ_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*CONSTANTS*/
#define VALID_ARGC 		3 		//Number of args expected
#define MAX_NUM_INDEX 	1 		//Index of max number to calc
#define MIN_COLLATZ 	2 		//Smallest value to calc a Collatz
#define MAX_STOP_TIME	1000	//Given as the largest stopping time required for compuation

/*STRUCTURES*/
typedef struct{
	int currentNum;
	int maxNum;
	short numThreads;
}Collatz;

/* Checks passed arguments to ensure comformity to expected format */
void checkArgs(int, char**);

/* Starting point for thread creation and sequence calculation */
void start(int, char**);

/* Calculates the corresponding Collatz sequence for the passed integer */
/* Returns the "stopping time" for the passed integer */
int calcCollatz(int);


#endif /* MT_COLLATZ_C_ */
