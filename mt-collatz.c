/*
 ============================================================================
 Name        : sysnet1p2.c
 Author      : chris
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include "mt-collatz.h"


int currentThreadCount = 0;


int main(int argc, char **argv) {
	checkArgs(argc, argv);
	start(argc, argv);
	return EXIT_SUCCESS;
}



//Verify valid number of args
void checkArgs(int argc, char **argv){
	if(argc != VALID_ARGC || atoi(argv[MAX_NUM_INDEX]) < 2){
		perror("Invalid arguments\n");
		exit(EXIT_FAILURE);
	}
}



//Strt teh maj0r win
void start(int argc, char **argv){
	int i, count, number;
	number = MIN_COLLATZ;
	count = atoi(argv[MAX_NUM_INDEX]); //atoi converts string to int

	for(i = MIN_COLLATZ; i < count+1; i++){
		calcCollatz(number);
		printf("num %d = %d\n", i, number);
		number++;
	}

}




//Pass the number to be Collatized!
void calcCollatz(int num){
	while(num != 1){
		if( num % 2 == 0 )
			num /= 2;
		else
			num = 3*num+1;
		printf("%d \n", num);
	}
	fflush(stdout);
}






























