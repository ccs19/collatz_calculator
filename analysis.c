#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int THREADS;
int VAL;

int main(int argc, char** argv){
	VAL = atoi(argv[1]);
	THREADS = atoi(argv[2]);
	char command[100];
	int t = 1;
	int i = 0;
	while(i < THREADS){
		command[0] = '\0';
		sprintf(command, "./mt-collatz %d %d -noRace | grep [0-9]+\.[0-9]+", VAL, t);
		system(command);
		t++;
		i++;
	}


}
