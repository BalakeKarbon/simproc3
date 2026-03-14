/*
 * CS 440 – Operating Systems
 * Project 3 – CPU Scheduling Simulation
 * Names: Blake Karbon, Michael Gavina
 * Date: 2026-03-11
 */


#include <stdio.h>
#include <stdlib.h>

#define MAXP 10

typedef struct {
	int pid;
	int arrival;
	int burst;
	int remaining;
	
	int first_start;
	int completion;
	
	int started;
} Process;

int main(){
	int randomSeed;
	int numberOfProcesses;
	int lastArrivalTime;
	int maxBurstTime;
	int quantum;
	int latency; 

	// var set-up:
	printf("Enter random seed: ");

	scanf("%d", &randomSeed);


	printf("Enter number of processes (2-%d): ", MAXP);
	
	scanf("%d", &numberOfProcesses);
	while(numberOfProcesses > MAXP || numberOfProcesses < 2){
		printf("Sorry that number is not valid. Try again.\n");
		printf("Enter number of processes (2-%d): ", MAXP);
		scanf("%d", &numberOfProcesses);
	}


	printf("Enter last arrival time (0-99): ");

	scanf("%d", &lastArrivalTime);

	while(lastArrivalTime  > 99 || lastArrivalTime < 0){
		printf("Sorry that number is not valid. Try again.\n");
		printf("Enter last arrival time (0-99): ");
		scanf("%d", &lastArrivalTime);
	}


	printf("Enter maximum burst time (1-100): ");
	
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
	// End of var set-up
	srand(randomSeed);
	Process p[MAXP];
	printf("\nProcesses:\n");
	for(int i=0;i<numberOfProcesses;i++){
		p[i].pid=i+1;
		p[i].arrival=rand()%(lastArrivalTime+1);
		p[i].burst=rand()%maxBurstTime+1;
		p[i].remaining=p[i].burst;
		
		printf("P%d: arrival=%d burst=%d\n",
		       p[i].pid,p[i].arrival,p[i].burst);
	}

	double w,r;

	int completionSum=0;
	int wait100=0;
	int resp100=0;
	
	completionSum += runFCFS(p,n,L,&w,&r);
	wait100 += (int)(w*100+0.5);
	resp100 += (int)(r*100+0.5);
	
	completionSum += runSJF(p,n,L,&w,&r);
	wait100 += (int)(w*100+0.5);
	resp100 += (int)(r*100+0.5);
	
	completionSum += runSRTF(p,n,L,&w,&r);
	wait100 += (int)(w*100+0.5);
	resp100 += (int)(r*100+0.5);
	
	completionSum += runRR(p,n,q,L,&w,&r);
	wait100 += (int)(w*100+0.5);
	resp100 += (int)(r*100+0.5);
	
	completionSum += runRandom(p,n,L,&w,&r);
	wait100 += (int)(w*100+0.5);
	resp100 += (int)(r*100+0.5);
	
	int checksum = completionSum + wait100 + resp100;
	
	printf("\nCHECKSUM: %d\n",checksum);

	return 0;
}


