#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h>  
#include <stdlib.h>  
#include <sys/wait.h> 
#include <string.h> 
#include <time.h> 
#include <signal.h>
#include <sys/time.h>
#include <linux/time.h>

/************************************************************************************************ 
		These DEFINE statements represent the workload size of each task and 
		the time quantum values for Round Robin scheduling for each task.
*************************************************************************************************/

#define WORKLOAD1 100000
#define WORKLOAD2 50000
#define WORKLOAD3 25000
#define WORKLOAD4 10000

#define QUANTUM1 1000
#define QUANTUM2 1000
#define QUANTUM3 1000
#define QUANTUM4 1000

/************************************************************************************************ 
					DO NOT CHANGE THE FUNCTION IMPLEMENTATION
*************************************************************************************************/
void myfunction(int param){

	int i = 2;
	int j, k;

	while(i < param){
		k = i; 
		for (j = 2; j <= k; j++)
		{
			if (k % j == 0){
				k = k/j;
				j--;
				if (k == 1){
					break;
				}
			}
		}
		i++;
	}
}
/************************************************************************************************/

int main(int argc, char const *argv[])
{
	pid_t pid1, pid2, pid3, pid4;
	int running1, running2, running3, running4;

	pid1 = fork();

	if (pid1 == 0){

		myfunction(WORKLOAD1);

		exit(0);
	}
	kill(pid1, SIGSTOP);

	pid2 = fork();

	if (pid2 == 0){

		myfunction(WORKLOAD2);

		exit(0);
	}
	kill(pid2, SIGSTOP);

	pid3 = fork();

	if (pid3 == 0){

		myfunction(WORKLOAD3);
 
		exit(0);
	}
	kill(pid3, SIGSTOP);

	pid4 = fork();

	if (pid4 == 0){

		myfunction(WORKLOAD4);

		exit(0);
	}
	kill(pid4, SIGSTOP);

	/************************************************************************************************ 
		At this point, all  newly-created child processes are stopped, and ready for scheduling.
	*************************************************************************************************/



	/************************************************************************************************
		- Scheduling code starts here
		- Below is a sample schedule. (which scheduling algorithm is this?)
		- For the assignment purposes, you have to replace this part with the other scheduling methods 
		to be implemented.
	************************************************************************************************/
    //Clock
    struct timespec ready, arrival1, arrival2, arrival3, arrival4;
    double response_time1, response_time2, response_time3, response_time4, average_response_time;
    int started1 = 0, started2 = 0, started3 = 0, started4 = 0;

	running1 = 1;
	running2 = 1;
	running3 = 1;
	running4 = 1;

    clock_gettime(CLOCK_MONOTONIC, &ready);          // Start Clock

	while (running1 > 0 || running2 > 0 || running3 > 0 || running4 > 0)
    
	{
		if (running1 > 0){
            if (started1 == 0){
                clock_gettime(CLOCK_MONOTONIC, &arrival1);    // Arrival Time
                response_time1 = (arrival1.tv_sec - ready.tv_sec) + ((arrival1.tv_nsec - ready.tv_nsec) / 1e9);
                printf("Response Time for Task 1: %.10f seconds\n", response_time1);
                started1 = 1;

            }
            
			kill(pid1, SIGCONT);
			usleep(QUANTUM1);
			kill(pid1, SIGSTOP);
		}
		if (running2 > 0){
            if (started2 == 0){
                clock_gettime(CLOCK_MONOTONIC, &arrival2);    // Arrival Time
                response_time2 = (arrival2.tv_sec - ready.tv_sec) + ((arrival2.tv_nsec - ready.tv_nsec) / 1e9);
                printf("Response Time for Task 2: %.10f seconds\n", response_time2);
                started2 = 1;

            }
			kill(pid2, SIGCONT);
			usleep(QUANTUM2);
			kill(pid2, SIGSTOP);
		}
		if (running3 > 0){
            if (started3 == 0){
                clock_gettime(CLOCK_MONOTONIC, &arrival3);    // Arrival Time
                response_time3 = (arrival3.tv_sec - ready.tv_sec) + ((arrival3.tv_nsec - ready.tv_nsec) / 1e9);
                printf("Response Time for Task 3: %.10f seconds\n", response_time3);
                started3 = 1;

            }
			kill(pid3, SIGCONT);
			usleep(QUANTUM3);
			kill(pid3, SIGSTOP);
		}
		if (running4 > 0){
            if (started4 == 0){
                clock_gettime(CLOCK_MONOTONIC, &arrival4);    // Arrival Time
                response_time4 = (arrival4.tv_sec - ready.tv_sec) + ((arrival4.tv_nsec - ready.tv_nsec) / 1e9);
                printf("Response Time for Task 4: %.10f seconds\n", response_time4);
                started4 = 1;

            }
			kill(pid4, SIGCONT);
			usleep(QUANTUM4);
			kill(pid4, SIGSTOP);
		}
		waitpid(pid1, &running1, WNOHANG);
		waitpid(pid2, &running2, WNOHANG);
		waitpid(pid3, &running3, WNOHANG);
		waitpid(pid4, &running4, WNOHANG);
	}
	average_response_time = (response_time1 + response_time2 + response_time3 + response_time4) / 4;
	printf("Average Response Time: %.10f seconds\n", average_response_time);
    printf("All tasks completed.\n");
	/************************************************************************************************
		- Scheduling code ends here
	************************************************************************************************/

	return 0;
}