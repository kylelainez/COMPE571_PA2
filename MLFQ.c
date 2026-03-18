#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h>  
#include <stdlib.h>  
#include <sys/wait.h> 
#include <string.h> 
#include <time.h> 
#include <signal.h>
#include <stdbool.h>
#include <linux/time.h>
/************************************************************************************************ 
		These DEFINE statements represent the workload size of each task
*************************************************************************************************/

#define WORKLOAD1 100000
#define WORKLOAD2 50000
#define WORKLOAD3 25000
#define WORKLOAD4 10000

// #define QUANTUM   100
#define QUANTUM   500


pid_t status[4] = {0}; // Initially all the pids are runnable, once they finish set to 0 

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
void handle_sigchld(int sig) {
    pid_t pid = 0; 
    pid = waitpid(-1, NULL, 0); // get the child who finished during usleep 
    for(int i = 0; i<4;i++) {
        if(status[i] == pid) { 
            status[i] = 0; // Make sure the terminated child is not run in Queue 2 
        }
    }

}
/************************************************************************************************/
void do_nothing(int sig) {
}

int main(int argc, char const *argv[])
{
    signal(SIGCHLD, do_nothing); // Interrupts usleep when Quantum > Workload
	pid_t pid[4];

	for(int i = 0; i < 4; i++){
        pid[i] = fork();

        if (pid[i] == 0){ // child
            myfunction(WORKLOAD1 / (1 << i)); // WORKLOAD1, WORKLOAD2, WORKLOAD3, WORKLOAD4

            exit(0);
        }
        else { // parent
            status[i] = pid[i]; 
            // using array status as a flag, in the sigchld handler we will index the array using
            // pid and set to 0 if the process completes during its RR quantum
        }
        kill(pid[i], SIGSTOP);
    }
	/************************************************************************************************ 
		At this point, all  newly-created child processes are stopped, and ready for scheduling.
	************************************************************************************************/



	/************************************************************************************************
		- Scheduling code starts here
		- Below is a sample schedule. (which scheduling algorithm is this?)
		- For the assignment purposes, you have to replace this part with the other scheduling methods 
		to be implemented.
	************************************************************************************************/

	//Clock
    struct timespec ready, completion_time[4];
    double response_time[4], average_response_time;

    clock_gettime(CLOCK_MONOTONIC, &ready); // Arrival Clock


    // MLFQ 
    // Queue 1 Using RR
    for(int i=0; i < 4; i++){
        kill(pid[i], SIGCONT);
        usleep(QUANTUM);  // Need SIGCHILD handler for QUANTUM > Workloads
        kill(pid[i], SIGSTOP);

        if(status[i] == 0) {
		    clock_gettime(CLOCK_MONOTONIC, &completion_time[i]);    // Completion Time
            response_time[i] = (completion_time[i].tv_sec - ready.tv_sec) + ((completion_time[i].tv_nsec - ready.tv_nsec) / 1e9);
            // printf("Response Time for Task %d: %.10f seconds\n", i+1, response_time[i]);
        }
    }


    // Queue 2 Using FCFS
    for(int i = 0; i < 4; i ++)
    {
        if(status[i] != 0){
            kill(pid[i], SIGCONT);
            waitpid(pid[i], NULL, 0);

            clock_gettime(CLOCK_MONOTONIC, &completion_time[i]);    // Completion Time
            response_time[i] = (completion_time[i].tv_sec - ready.tv_sec) + ((completion_time[i].tv_nsec - ready.tv_nsec) / 1e9);
            // printf("Response Time for Task %d: %.10f seconds\n", i+1, response_time[i]);
        }
    }



    average_response_time = (response_time[0] + response_time[1] + response_time[2] + response_time[3]) / 4;
    // printf("Average Response Time: %.10f seconds\n", average_response_time);
    // printf("All tasks completed.\n");
    printf("%.10f\n", average_response_time);
	/************************************************************************************************
		- Scheduling code ends here
	************************************************************************************************/

	return 0;
}