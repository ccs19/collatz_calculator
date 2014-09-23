/*
 * mt-collatz.c
 *
 *  Created on: Sep 22, 2014
 *      Author: christopher
 */

#ifndef MT_COLLATZ_H_
#define MT_COLLATZ_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*CONSTANTS*/
#define VALID_ARGC 3 //Number of args expected
#define MAX_NUM_INDEX 2 //Index of max number to calc
#define MIN_COLLATZ 2 //Smallest value to calc a Collatz


/*STRUCTURES*/
typedef struct{
	int currentNum;
	int maxNum;
	short numThreads;
}Collatz;


void checkArgs(int, char**);
void start(int, char**);
void calcCollatz(int);
#endif /* MT_COLLATZ_C_ */
