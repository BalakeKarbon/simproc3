/*
 * CS 440 – Operating Systems
 * Project 3 – CPU Scheduling Simulation
 * Names: Blake Karbon, Michael Gavina
 * Date: 2026-03-11
 */


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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

void copyProcesses(Process src[], Process dst[], int n)
{
	for(int i=0;i<n;i++){
		dst[i] = src[i];
		dst[i].remaining = src[i].burst;
		dst[i].started = 0;
	}
}

int allDone(Process p[], int n)
{
	for(int i=0;i<n;i++)
		if(p[i].remaining > 0) return 0;
	return 1;
}

int nextArrival(Process p[], int n, int time)
{
	int min = INT_MAX;
	for(int i=0;i<n;i++)
		if(p[i].remaining > 0 && p[i].arrival > time && p[i].arrival < min)
			min = p[i].arrival;

	return min;
}

void computeMetrics(Process p[], int n, double *avgWait, double *avgResp)
{
	double totalWait=0;
	double totalResp=0;

	for(int i=0;i<n;i++){
		int response = p[i].first_start - p[i].arrival;
		int waiting  = p[i].completion - p[i].arrival - p[i].burst;

		totalWait += waiting;
		totalResp += response;
	}

	*avgWait = totalWait/n;
	*avgResp = totalResp/n;
}

int runFCFS(Process base[], int n, int latency, double *avgWait, double *avgResp)
{
	Process p[MAXP];
	copyProcesses(base,p,n);

	printf("\nFCFS:\n");

	int time=0;
	int last=-1;

	while(!allDone(p,n))
	{
		int next=-1;
		int earliest=INT_MAX;

		for(int i=0;i<n;i++)
			if(p[i].remaining>0 && p[i].arrival<=time && p[i].arrival<earliest){
				earliest=p[i].arrival;
				next=i;
			}

		if(next==-1){
			time = nextArrival(p,n,time);
			continue;
		}

		if(last!=-1 && last!=next){
			printf("@t=%d, context switch occurs\n",time);
			time+=latency;
		}

		if(!p[next].started){
			p[next].first_start=time;
			p[next].started=1;
		}

		printf("@t=%d, P%d selected for %d units\n",
			   time,p[next].pid,p[next].remaining);

		time += p[next].remaining;
		p[next].remaining=0;
		p[next].completion=time;

		last=next;
	}

	printf("@t=%d, all processes complete\n",time);
	printf("Completed in %d cycles.\n",time);

	computeMetrics(p,n,avgWait,avgResp);

	printf("Average waiting time = %.2f\n",*avgWait);
	printf("Average response time = %.2f\n",*avgResp);

	return time;
}

int runSJF(Process base[], int n, int latency, double *avgWait, double *avgResp)
{
	Process p[MAXP];
	copyProcesses(base,p,n);

	printf("\nSJF:\n");

	int time=0;
	int last=-1;

	while(!allDone(p,n))
	{
		int next=-1;
		int best=INT_MAX;

		for(int i=0;i<n;i++)
			if(p[i].remaining>0 && p[i].arrival<=time && p[i].burst<best){
				best=p[i].burst;
				next=i;
			}

		if(next==-1){
			time = nextArrival(p,n,time);
			continue;
		}

		if(last!=-1 && last!=next){
			printf("@t=%d, context switch occurs\n",time);
			time+=latency;
		}

		if(!p[next].started){
			p[next].first_start=time;
			p[next].started=1;
		}

		printf("@t=%d, P%d selected for %d units\n",
			   time,p[next].pid,p[next].remaining);

		time += p[next].remaining;
		p[next].remaining=0;
		p[next].completion=time;

		last=next;
	}

	printf("@t=%d, all processes complete\n",time);
	printf("Completed in %d cycles.\n",time);

	computeMetrics(p,n,avgWait,avgResp);

	printf("Average waiting time = %.2f\n",*avgWait);
	printf("Average response time = %.2f\n",*avgResp);

	return time;
}

int runSRTF(Process base[], int n, int latency, double *avgWait, double *avgResp)
{
	Process p[MAXP];
	copyProcesses(base,p,n);

	printf("\nSRTF:\n");

	int time=0;
	int running=-1;

	while(!allDone(p,n))
	{
		int best=-1;
		int shortest=INT_MAX;

		for(int i=0;i<n;i++)
			if(p[i].remaining>0 && p[i].arrival<=time && p[i].remaining<shortest){
				shortest=p[i].remaining;
				best=i;
			}

		if(best==-1){
			time = nextArrival(p,n,time);
			continue;
		}

		if(running!=-1 && running!=best){
			printf("@t=%d, context switch occurs\n",time);
			time+=latency;
		}

		if(!p[best].started){
			p[best].first_start=time;
			p[best].started=1;
		}

		printf("@t=%d, P%d selected for 1 unit\n",time,p[best].pid);

		p[best].remaining--;
		time++;

		if(p[best].remaining==0)
			p[best].completion=time;

		running=best;
	}

	printf("@t=%d, all processes complete\n",time);
	printf("Completed in %d cycles.\n",time);

	computeMetrics(p,n,avgWait,avgResp);

	printf("Average waiting time = %.2f\n",*avgWait);
	printf("Average response time = %.2f\n",*avgResp);

	return time;
}

int runRR(Process base[], int n, int q, int latency, double *avgWait, double *avgResp)
{
	Process p[MAXP];
	copyProcesses(base, p, n);

	printf("\nRR (q=%d):\n", q);

	int queue[100];
	int front = 0, rear = 0;

	int time = 0;
	int running = -1;

	while(!allDone(p,n))
	{
		/* add newly arrived processes */
		for(int i=0;i<n;i++){
			if(p[i].arrival <= time && p[i].remaining > 0){
				int alreadyQueued = 0;
				for(int j=front;j<rear;j++)
					if(queue[j] == i) alreadyQueued = 1;

				if(!alreadyQueued && i != running)
					queue[rear++] = i;
			}
		}

		/* CPU idle */
		if(front == rear){
			int next = INT_MAX;
			for(int i=0;i<n;i++)
				if(p[i].remaining > 0 && p[i].arrival < next)
					next = p[i].arrival;

			time = next;
			continue;
		}

		int cur = queue[front++];

		if(running != -1 && running != cur){
			printf("@t=%d, context switch occurs\n", time);
			time += latency;
		}

		if(!p[cur].started){
			p[cur].first_start = time;
			p[cur].started = 1;
		}

		int run = p[cur].remaining < q ? p[cur].remaining : q;

		printf("@t=%d, P%d selected for %d units\n",
			   time, p[cur].pid, run);

		time += run;
		p[cur].remaining -= run;

		/* add arrivals during execution */
		for(int i=0;i<n;i++){
			if(p[i].arrival > time-run && p[i].arrival <= time)
				queue[rear++] = i;
		}

		if(p[cur].remaining > 0){
			queue[rear++] = cur;   // requeue
		} else {
			p[cur].completion = time;
		}

		running = cur;
	}

	printf("@t=%d, all processes complete\n", time);
	printf("Completed in %d cycles.\n", time);

	computeMetrics(p,n,avgWait,avgResp);

	printf("Average waiting time = %.2f\n", *avgWait);
	printf("Average response time = %.2f\n", *avgResp);

	return time;
}

int runRandom(Process base[], int n, int latency, double *avgWait, double *avgResp)
{
	Process p[MAXP];
	copyProcesses(base,p,n);

	printf("\nRandom:\n");

	int time = 0;
	int running = -1;

	while(!allDone(p,n))
	{
		int ready[MAXP];
		int count = 0;

		/* build ready list */
		for(int i=0;i<n;i++){
			if(p[i].remaining > 0 && p[i].arrival <= time){
				ready[count++] = i;
			}
		}

		/* CPU idle */
		if(count == 0){
			int next = INT_MAX;
			for(int i=0;i<n;i++){
				if(p[i].remaining > 0 && p[i].arrival < next)
					next = p[i].arrival;
			}

			time = next;
			continue;
		}

		/* random selection */
		int chosen = ready[rand() % count];

		if(running != -1 && running != chosen){
			printf("@t=%d, context switch occurs\n", time);
			time += latency;
		}

		if(!p[chosen].started){
			p[chosen].first_start = time;
			p[chosen].started = 1;
		}

		printf("@t=%d, P%d selected for %d units\n",
			   time, p[chosen].pid, p[chosen].remaining);

		time += p[chosen].remaining;
		p[chosen].remaining = 0;
		p[chosen].completion = time;

		running = chosen;
	}

	printf("@t=%d, all processes complete\n", time);
	printf("Completed in %d cycles.\n", time);

	computeMetrics(p,n,avgWait,avgResp);

	printf("Average waiting time = %.2f\n", *avgWait);
	printf("Average response time = %.2f\n", *avgResp);

	return time;
}

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
	
	completionSum += runFCFS(p,numberOfProcesses,latency,&w,&r);
	wait100 += (int)(w*100+0.5);
	resp100 += (int)(r*100+0.5);
	
	completionSum += runSJF(p,numberOfProcesses,latency,&w,&r);
	wait100 += (int)(w*100+0.5);
	resp100 += (int)(r*100+0.5);
	
	completionSum += runSRTF(p,numberOfProcesses,latency,&w,&r);
	wait100 += (int)(w*100+0.5);
	resp100 += (int)(r*100+0.5);
	
	completionSum += runRR(p,numberOfProcesses,quantum,latency,&w,&r);
	wait100 += (int)(w*100+0.5);
	resp100 += (int)(r*100+0.5);
	
	completionSum += runRandom(p,numberOfProcesses,latency,&w,&r);
	wait100 += (int)(w*100+0.5);
	resp100 += (int)(r*100+0.5);
	
	int checksum = completionSum + wait100 + resp100;
	
	printf("\nCHECKSUM: %d\n",checksum);

	return 0;
}


