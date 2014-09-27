/*
 ============================================================================
 Name        : mt-collatz.c
 Author      : chris, Branden Sherrell
 Description : Implementation file for collatz sequence calculations using pthreads
 ============================================================================
 */


#include "mt-collatz.h"


int currentThreadCount = 0;


int main(int argc, char **argv){
	struct timespec startTime, endTime; 

	checkArgs(argc, argv);

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startTime);	//get starting time
	startCalc(argc, argv);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endTime);		//get ending time 

	printHistogram();

	fprintf(stderr, "%ld %d, %.9lf\n", 	atol(argv[MAX_NUM_INDEX]), 
										atoi(argv[THREAD_CNT_INDEX]), 
										( (float)(endTime.tv_nsec - startTime.tv_nsec)/BILLION) + (endTime.tv_sec - startTime.tv_sec));	//time in seconds?

	return EXIT_SUCCESS;
}



//Verify valid number of args
void checkArgs(int argc, char **argv){
	if(argc < VALID_ARGC_MIN || argc > VALID_ARGC_MAX){										//correct number of params?
		printf("Invalid input. Format: <Max Collatz number, Thread count, options>\n");
		exit(EXIT_FAILURE);
	}
	else{	//valid argument count
		if(argc == VALID_ARGC_MAX){
			if(strncmp(argv[NO_RACE_INDEX], NO_RACE_STR, sizeof(NO_RACE_STR))){				//check if "options" are correct
				printf("Unknown option. Use %s\n", NO_RACE_STR);
				exit(EXIT_FAILURE);
			}
			else
				noRace = 1; 	//indicate we wish to suppress race conditions 
		}
		if(atoi(argv[MAX_NUM_INDEX]) < MIN_COLLATZ){											//Max Collatz number correct? 
			printf("Arg[%d] must be integral and greater than 2\n", MAX_NUM_INDEX);
			exit(EXIT_FAILURE);
		}
	}
}



//Strt teh maj0r win
void startCalc(int argc, char **argv){
	int maxNum, threadCnt, i;
	pthread_t* tid = NULL; 
	maxNum = atoi(argv[MAX_NUM_INDEX]); //atoi converts string to int
	threadCnt = atoi(argv[THREAD_CNT_INDEX]);

	if(noRace)
		if(pthread_mutex_init(&currentNumLock, NULL) != 0){
			perror("Mutex init failed");
			exit(EXIT_FAILURE);
		}

	tid = createThreads(threadCnt, (void*) &maxNum);
	for(i = 0; i < threadCnt; i++)
		pthread_join(tid[i], NULL);

	free(tid);
	if(noRace)
		pthread_mutex_destroy(&currentNumLock);
}



pthread_t* createThreads(int numThreads, void* data){
	int i; 

	pthread_t* temp = malloc(sizeof(pthread_t)*numThreads);
	if(temp == NULL){
		perror("Error");
		exit(EXIT_FAILURE);
	}

	if(noRace)
		for(i = 0; i < numThreads; i++)
			pthread_create(&temp[i], NULL, (void*) &calcStoppingTimes, data); 
	else
		for(i = 0; i < numThreads; i++)
			pthread_create(&temp[i], NULL, (void*) &calcStoppingTimes_noRace, data); 

	return temp;
} 



void calcStoppingTimes_noRace(void* data){
	int maxNum = *( (int*) data); 		//dereference and type cast locally
	int stopTime; 

	while(1){			//while(currentNum != maxNum){
		pthread_mutex_lock(&currentNumLock);
		if(currentNum == maxNum){
			pthread_mutex_unlock(&currentNumLock);
			break;
		}
		stopTime = calcCollatz(currentNum++);
		if(stopTime <= MAX_STOP_TIME)
			histogram[stopTime]++;
		pthread_mutex_unlock(&currentNumLock);
	}
}

void calcStoppingTimes(void* data){
	int maxNum = *( (int*) data); 		//dereference and type cast locally
	int stopTime; 

	while(currentNum != maxNum){
		stopTime = calcCollatz(currentNum++);
		if(stopTime <= MAX_STOP_TIME)
			histogram[stopTime]++;
	}
}



//Pass the number to be Collatized! 
int calcCollatz(unsigned long num){
	int i = 0;
	while(num != 1){
		if( num & 1 )	//LSB set - num is odd
			num = 3*num+1;
		else
			num = num/2;
		i++;
	}
	fflush(stdout);
	return i; 
}



void printHistogram(){
	int i; 
	for(i = 1; i <= MAX_STOP_TIME; i++)
		printf("<k = %d>, <%u>\n",i ,histogram[i]);
}






























