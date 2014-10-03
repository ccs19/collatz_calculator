/*
 ============================================================================
 Name        : mt-collatz.c
 Author      : Christopher Schneider, Branden Sherrell
 Description : Implementation file for collatz sequence calculations using pthreads
 ============================================================================
 */


#include "mt-collatz.h"



int main(int argc, char **argv){
	checkArgs(argc, argv);
	startTime(argc, argv);
	return EXIT_SUCCESS;
}



/** FUNCTION: checkArgs
 *
 *  Checks the passed arguments to ensure comformity to the expexted format:
 *  				./mt-collatz <Maximum value> <Number of threads> <options>
 *
 *  The program will terminate with an error to stdout if a syntax error is found.
 *
 *	@param		int Number of arguments passed from the OS to main
 *	@param		Char* array of passed arguments
 */
void checkArgs(int argc, char **argv){
	if(argc < VALID_ARGC_MIN || argc > VALID_ARGC_MAX){										//correct number of params?
		printf("Invalid input. Format: <Max Collatz number, Thread count, options>\n");
		exit(EXIT_FAILURE);
	}
	else
		checkRace(argc, argv);

	if(atoi(argv[MAX_NUM_INDEX]) < MIN_COLLATZ){											//Max Collatz number correct?
			printf("Arg[%d] must be integral and greater than 2\n", MAX_NUM_INDEX);
			exit(EXIT_FAILURE);
		}

}


/**	FUNCTION: startCalc
 *
 *  Starting point for calculating stopping times for the value passed to main. This function
 *  dynamically allocates memory for thread IDs, the number of which is a passed argument to main.
 *  Each thread is created and given a target function by an interal call to createThreads().
 *  Additionally, this function initializes a global mutex for synchronizing thread execution; the
 *  program will terminate if such initialization fails.
 *
 *	@param		Maximum value to calculate Collatz stopping times to
 *	@param		Number of threads to use
 */
void startCalc(int maxNum, int threadCount){
	pthread_t* tid = NULL;

	if(noRace)		//if noRace is set, we need to initialize the global mutex
		if(pthread_mutex_init(&currentNumLock, NULL) != 0){
			perror("Mutex init failed");
			exit(EXIT_FAILURE);
		}

	tid = createThreads(threadCount, (void*) &maxNum);
	for(threadCount-=1; threadCount >= 0; threadCount--)
		pthread_join(tid[threadCount], NULL);

	free(tid);
	if(noRace)		//if noRace is set, we need to destory the mutex created earlier
		pthread_mutex_destroy(&currentNumLock);
}


/** FUNCTION: createThreads
 *
 *  Dynamically allocates memory for the required number of threads.
 *
 *	@param		The number of threads to allocate memory for
 *	@oaram		void* typecasted data to be associated with a created thread
 *	@return 	Array of pointers to pthread_t type data
 */
pthread_t* createThreads(int numThreads, void* data){
	pthread_t* temp = malloc(sizeof(pthread_t)*numThreads);
	if(temp == NULL){
		perror("Memory allocation error");
		exit(EXIT_FAILURE);
	}

	if(noRace)
		for(numThreads-=1; numThreads >= 0; numThreads--)
			pthread_create(&temp[numThreads], NULL, (void*) &calcStoppingTimes_noRace, data);

	else
		for(numThreads-=1; numThreads >= 0; numThreads--)
			pthread_create(&temp[numThreads], NULL, (void*) &calcStoppingTimes, data);

	return temp;
}


/** FUNCTION: calcStoppingTimes_noRace
 *
 *  This function is exactly identical to calcStoppingTimes(), but implements the necessary
 *  safeguards to prevent race conditions using mutexes.
 *
 *	@param		void*	- Necessary function structure when using pthreads
 *	@return 	void	- Necessary function structure when using pthreads
 */
void calcStoppingTimes_noRace(void* data){
	int maxNum = *( (int*) data); 		//dereference and type cast locally
	int stopTime;
	unsigned long count; 

	while(1){						//while(1) because we cannot access currentNumLock non-atomically 
		count = getGlobalCount(); 	//inline atomic access function 
		if(count > maxNum)			//inclusive of last value
			break;

		stopTime = calcCollatz(count);
		assert(stopTime <= MAX_STOP_TIME); 
		histogram[stopTime]++;
		
	}
}
/** FUNCTION: calcStoppingTimes
 *
 *  Target function for created threads. Interally the program loops until until all stoping times
 *  have been exhausted in the set [2, N], where N represents the maximum Collatz stopping time of
 *  interest. Note that N is the first argument passed from the OS.
 *
 *	@param		void*	- Necessary function structure when using pthreads
 *	@return 	void	- Necessary function structure when using pthreads
 */
void calcStoppingTimes(void* data){
	int maxNum = *( (int*) data); 		//dereference and type cast locally
	int stopTime;

	while(currentNum <= maxNum){
		stopTime = calcCollatz(currentNum++);
		if(stopTime <= MAX_STOP_TIME)
			histogram[stopTime]++;
	}
}

unsigned long getGlobalCount(){
	long count; 
	pthread_mutex_lock(&currentNumLock);
	count = currentNum++; 
	pthread_mutex_unlock(&currentNumLock);

	return count; 
}

/** FUNCTION: calcCollatz
 *
 *  Calculates the stopping time for the passed in parameter. This function has been optimized
 *  to use bit-wise operations.
 *
 *	@param		The value to calculate the Collatz sequence stopping time for
 *	@return 	Stopping time for the passed integral value
 */
int calcCollatz(unsigned long num){
	int i = 0;
	while(num != 1){
		if( num & 1 )			//LSB set - num is odd
			num = 3*num+1;
		else
			num >>= 1;	 		//Bit-wise division by 2
		i++;
	}
	return i;
}


/** FUNCTION: printHistogram
 *
 *  Prints the global histogram array to stdout with the following format:
 *  			<k = 1> <Stopping Time>
 *
 *	The highest stopping time is based on MAX_STOP_TIME (default: 1000)
 */
void printHistogram(){
	int i;
	for(i = 1; i <= MAX_STOP_TIME; i++)
		printf("<k = %d>, <%u>\n",i ,histogram[i]);
}

/** FUNCTION startTime
 *
 *  This function begins by getting the starting time, calling startCalc, then getting the ending time.
 *	It then calls the printHistogram and printResults functions to show the results.
 *
 *	@param		int	    - argc from main
 *	@param 	    char**  - argv from main
 */
void startTime(int argc, char **argv){

	timer *t = newTimer();
	startCalc(atoi(argv[MAX_NUM_INDEX]), atoi(argv[THREAD_CNT_INDEX]));
	endTimer(t);

	printHistogram();
	printResults(argv, t);
	free(t);
}

/** FUNCTION: printResults
 *
 *  Prints the results of the calculations in the format:
 *  <Max collatz number>, <number of threads>, <time required>
 *
 *	@param		timer*				 - Struct containing calculated times.
 *	@param 	    char**  		     - argv from main
 */
void printResults(char** argv, timer *t){
	long cNum = atol(argv[MAX_NUM_INDEX]);
	int tNum = atoi(argv[THREAD_CNT_INDEX]);
	fprintf(stderr, "%ld %d, %.9lf\n", 	cNum, tNum, t->total);
}


/** FUNCTION: newTimer
 *
 *  Generates a new timer struct and returns a pointer.
 *  t->sec is set to the current time
 *  t->nsec is set to 0
 *  t->total is set to 0
 *  t->ts is set to current time (To track nanoseconds)
 *
 *	@return 	timer*		- Pointer to initialized timer struct
 */
timer *newTimer(){
	timer *t = malloc(sizeof(timer));
	t->sec = time(0);
	t->nsec = 0;
	t->total = 0;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &(t->ts));
	return t;
}

/** FUNCTION: endTimer
 *
 *  Calculates the stopping point to nanoseconds and places the result in
 * 	t->total
 *
 *	@param		timer* t 	- Time tracking struct.
 */
void endTimer(timer* t){
	struct timespec e;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &e);
	t->nsec = (double)(e.tv_nsec - t->ts.tv_nsec)/BILLION;
	t->sec = (double)time(0) - t->sec;
	t->total = t->nsec + t->sec;
}

/** FUNCTION: checkRace
 *
 *  Calculates the time in seconds or milliseconds to help make the printResults
 *  fprintf statement less impossible to read.
 *
 *	@param		time_t start - Starting time.
 *	@param		time_t end   - Ending time.
 */
void checkRace(int argc, char** argv){
		if(argc == VALID_ARGC_MAX){
			if(strncmp(argv[NO_RACE_INDEX], NO_RACE_STR, sizeof(NO_RACE_STR))){				//check if "options" are correct
				printf("Unknown option. If desired, add flag %s\n", NO_RACE_STR);
					exit(EXIT_FAILURE);
			}
		}
		else
			noRace = 1; 	//indicate we wish to suppress race conditions
}






