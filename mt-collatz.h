/*
 ============================================================================
 Name        : mt-collatz.h
 Author      : Christopher Schneider, Branden Sherrell
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

#define VALID_ARGC_MIN		3 		//Minimum number of args expected
#define VALID_ARGC_MAX		4		//Maximum number of args expected (if -noRace is passed in)

#define MIN_COLLATZ 		2 		//Smallest value to calc a Collatz
#define MAX_STOP_TIME		1000	//Given as the largest stopping time required for compuation
#define BILLION				1000000000
#define TEN					10
#define NO_RACE_STR			"-noRace"


pthread_mutex_t currentNumLock;  				//Mutex for controlling access to currentNum
int noRace = 0; 								//Set if the user passes in option "-noRace"
unsigned long int currentNum = MIN_COLLATZ; 	//For Threads, current number in sequence to calculate
unsigned int histogram[MAX_STOP_TIME+1] = {0};	//+1 for index simplicity, throw away one index


/**Structures*/
typedef struct{
	double sec;
	double nsec;
	double total;
	struct timespec ts;
}timer;


/**
 *  Checks the passed arguments to ensure comformity to the expexted format:
 *  				./mt-collatz <Maximum value> <Number of threads> <options>
 *
 *  The program will terminate with an error to stdout if a syntax error is found.
 *
 *	@param		Number of arguments passed from the OS to main
 *	@param		Char* array of passed arguments
 *	@return 	No return value, program exits on improper input syntax
 */
void checkArgs(int, char**);


/**
 *  Starting point for calculating stopping times for the value passed to main. This function
 *  dynamically allocates memory for thread IDs, the number of which is a passed argument to main.
 *  Each thread is created and given a target function by an interal call to createThreads().
 *  Additionally, this function initializes a global mutex for synchronizing thread execution; the
 *  program will terminate if such initialization fails.
 *
 *	@param		Maximum value to calculate Collatz stopping times to
 *	@param		Number of threads to use
 */
void startCalc(int, int);

/* Calculates the corresponding Collatz sequence for the passed integer */
/* Returns the "stopping time" for the passed integer */

/**
 *  Calculates the stopping time for the passed in parameter. This function has been optimized
 *  to use bit-wise operations.
 *
 *	@param		The value to calculate the Collatz sequence stopping time for
 *	@return 	Stopping time for the passed integral value
 */
int calcCollatz(unsigned long);

/**
 *  Prints the global histogram array to stdout with the following format:
 *  			<k = 1> <Stopping Time>
 *
 *	It is assumed that the highest stopping time of interest is 1000.
 */
void printHistogram(); 

/**
 *  Dynamically allocates memory for the required number of threads.
 *
 *	@param		The number of threads to allocate memory for
 *	@oaram		void* typecasted data to be associated with a created thread
 *	@return 	Array of pointers to pthread_t type data
 */
pthread_t* createThreads(int, void*);

/**
 *  Target function for created threads. Interally the program loops until until all stoping times
 *  have been exhausted in the set [2, N], where N represents the maximum Collatz stopping time of
 *  interest. Note that N is the first argument passed from the OS.
 *
 *	@param		void*	- Necessary function structure when using pthreads
 *	@return 	void	- Necessary function structure when using pthreads
 */
void calcStoppingTimes(void*);

/**
 *  This function is exactly identical to calcStoppingTimes(), but implements the necessary
 *  safeguards to prevent race conditions using mutexes.
 *
 *	@param		void*	- Necessary function structure when using pthreads
 *	@return 	void	- Necessary function structure when using pthreads
 */
void calcStoppingTimes_noRace(void*);

/**
 *  This function begins by getting the starting time, calling startCalc, then getting the ending time.
 *	It then calls the printHistogram and printResults functions to show the results.
 *
 *	@param		int	    - argc from main
 *	@param 	    char**  - argv from main
 */
void startTime(int, char**);

/**
 *  This function merely prints the results of the calculations in the format:
 *  <Max collatz number>, <number of threads>, <time required>
 *
 *	@param		struct timespec	 - First timespec is starting time.
 *	@param		struct timespec	 - Second timespec is ending time.
 *	@param 	    char**  		     - argv from main
 */
void printResults(char**, timer*);

/**
 *  Calculates the time in seconds or milliseconds to help make the printResults
 *  fprintf statement less impossible to read.
 *
 *	@param		time_t start - Starting time.
 *	@param		time_t end   - Ending time.
 */
double findTime( time_t start, time_t end );

//Check if we want to accomodate for race comditions.
void checkRace(int, char**);

//Creates and returns a new timer structure
timer* newTimer();
//Fills in the timer structure variable "total."
void endTimer(timer* t);

#endif /* MT_COLLATZ_C_ */
