/*
 * Names: Blake Karbon, Michael Gavina
 *
 *
 *
 *
 *
 *
 */


#include <stdio.h>
#include <stdlib.h>





int main(){
	int randomSeed;
	int numberOfProcesses;
	int lastArrivalTime;
	int maxBurstTime;
	int quantum;
	int latency; 


	printf("Enter random seed: ");

	scanf("%d", &randomSeed);


	printf("Enter number of processes (2-10): ");
	
	scanf("%d", &numberOfProcesses);
	while(numberOfProcesses > 10 || numberOfProcesses < 2){
		printf("Sorry that number is not valid. Try again.\n");
		printf("Enter number of processes (2-10): ");
		scanf("%d", &numberOfProcesses);
	}


	printf("Enter last arrival time (0-99): ");

	scanf("%d", &lastArrivalTime);

	while(lastArrivalTime  > 99 || lastArrivalTime < 0){
		printf("Sorry that number is not valid. Try again.\n");
		printf("Enter last arrival time (0-99): ");
		scanf("%d", &lastArrivalTime);
	}


	printf("Enter maximun burst time (1-100): ");
	
	scanf("%d", &maxBurstTime);

	while(maxBurstTime > 100 || maxBurstTime < 1){
		printf("Sorry that number is not valid. Try again.\n");
		printf("Enter maximun burst time (1-100): ");
		scanf("%d", &maxBurstTime);
	}


	printf("Enter RR quantum (1-100): ");

	scanf("%d", &quantum);

	while(quantum > 100 || quantum < 1){
		printf("Sorry that number is not valid. Try again.\n");
		printf("Enter RR quantum (1-100): ");
		scanf("%d", &quantum);
	}


	printf("Enter context-switch latency (0-10): ");

	scanf("%d", &latency);

	while(latency > 100 || latency < 1){
		printf("Sorry that number is not valid. Try again.\n");
		printf("Enter context-switch latency (0-10): ");
		scanf("%d", &latency);
	}





	return 0;
}


