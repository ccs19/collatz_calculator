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
#include <time.h>		
#include <string.h>

/*CONSTANTS*/
#define MAX_NUM_INDEX 		1 		//Index of max number to calc
#define THREAD_CNT_INDEX	2
#define NO_RACE_INDEX		3
#define VALID_ARGC_MIN		3 		//Number of args expected
#define VALID_ARGC_MAX		4
#define MIN_COLLATZ 		2 		//Smallest value to calc a Collatz
#define MAX_STOP_TIME		1000	//Given as the largest stopping time required for compuation
#define BILLION				1000000000
#define NO_RACE_STR			"-noRace"

/*STRUCTURES*/
typedef struct{
	int currentNum;
	int maxNum;
	short numThreads;
}Collatz;

int noRace = 0; 
pthread_mutex_t currentNumLock;  
unsigned long int currentNum = MIN_COLLATZ; 
unsigned int histogram[MAX_STOP_TIME+1] = {0};	//+1 for index simplicity, throw away one index

/* Checks passed arguments to ensure comformity to expected format */
void checkArgs(int, char**);

/* Starting point for thread creation and sequence calculation */
void startCalc(int, char**);

/* Calculates the corresponding Collatz sequence for the passed integer */
/* Returns the "stopping time" for the passed integer */
int calcCollatz(unsigned long);

/* Prints the information from the global histogram array */
void printHistogram(); 

/* Allocates memory for and creates the threads */
pthread_t* createThreads(int, void*);

/* Target function for each thread - loops through and calculates stopping times */
void calcStoppingTimes(void*);

/* Same as above but with guards for preventing race conditions */
void calcStoppingTimes_noRace(void*);

#endif /* MT_COLLATZ_C_ */
