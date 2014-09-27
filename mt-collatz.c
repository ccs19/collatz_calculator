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
	if(argc != VALID_ARGC || atoi(argv[MAX_NUM_INDEX]) < MIN_COLLATZ){
		printf("Invalid arguments: Arg[%d] must be integral and greater than 2\n", MAX_NUM_INDEX);	//perror checks "errno" which has no indication of an error, so perror writes ".. : Success" 
		exit(EXIT_FAILURE);
	}
}



//Strt teh maj0r win
void startCalc(int argc, char **argv){
	int maxNum, threadCnt, i; 
	pthread_t* threads = NULL; 
	maxNum = atoi(argv[MAX_NUM_INDEX]); //atoi converts string to int
	threadCnt = atoi(argv[THREAD_CNT_INDEX]);

	threads = createThreads(threadCnt, (void*) &maxNum);
	for(i = 0; i < threadCnt; i++)
		pthread_join(threads[i], NULL);

	free(threads);
}



pthread_t* createThreads(int numThreads, void* data){
	int i; 

	pthread_t* temp = malloc(sizeof(pthread_t)*numThreads);
	if(temp == NULL){
		perror("Error");
		exit(EXIT_FAILURE);
	}

	for(i = 0; i < numThreads; i++)
		pthread_create(&temp[i], NULL, (void*) &calcStoppingTimes, data); 

	return temp;
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






























