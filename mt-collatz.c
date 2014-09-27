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

	fprintf(stderr, "%d, %d, %.9lf\n", atoi(argv[MAX_NUM_INDEX]), atoi(argv[THREAD_CNT_INDEX]), ((float)(endTime.tv_nsec - startTime.tv_nsec)/1000000000)+(endTime.tv_sec - startTime.tv_sec));	//time in seconds?
	//printf("\n%ld\n", (endTime.tv_sec - startTime.tv_sec));
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
	int i, count, number;
	number = MIN_COLLATZ;
	count = atoi(argv[MAX_NUM_INDEX]); //atoi converts string to int

	for(i = MIN_COLLATZ; i <= count; i++)
		calcCollatz(number++);
}





//Pass the number to be Collatized! Prints [num]:[stoppingTime]
int calcCollatz(unsigned long num){
//	if(num > 150000){
//		printf("Calculating for %ld\n", num);
//		printf("[%-2ld]:", num);
//	}
	long i = 0;
	while(num != 1){
		if( num & (1<<0) )
			num = 3*num+1;
		else
			num = num/2;
		//if(num > 150000)
			//printf("%ld \n", num);
		i++;
	}
		if(num > 150000)
		//printf("[%-2ld]\n", i);
	fflush(stdout);
	return i; 
}

void printHistogram(){
	int i; 
	for(i = 0; i < MAX_STOP_TIME; i++)
		printf("<k = %d>, <%u>\n",i ,histogram[i]);
}






























