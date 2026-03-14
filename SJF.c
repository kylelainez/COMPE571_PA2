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
		These DEFINE statements represent the workload size of each task
*************************************************************************************************/

#define WORKLOAD1 100000
#define WORKLOAD2 50000
#define WORKLOAD3 25000
#define WORKLOAD4 10000

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
struct Process {
    pid_t pid;
    int workload;
    int task_number;
};

int main(int argc, char const *argv[])
{
	pid_t pid[4];
	struct Process processes[4];

	for(int i = 0; i < 4; i++){
        pid[i] = fork();

        if (pid[i] == 0){
            myfunction(WORKLOAD1 / (1 << i)); // WORKLOAD1, WORKLOAD2, WORKLOAD3, WORKLOAD4

            exit(0);
        }
        kill(pid[i], SIGSTOP);
        processes[i].pid = pid[i]; 
        processes[i].workload = WORKLOAD1 / (1 << i);
        processes[i].task_number = i + 1;
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

    //Sort processes
    for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3 - i; j++){
			if(processes[j].workload > processes[j+1].workload){
				struct Process temp = processes[j];
				processes[j] = processes[j+1];
				processes[j+1] = temp;
			}
		}
	}


    for(int i=0; i < 4; i++){
        kill(processes[i].pid, SIGCONT);
        waitpid(processes[i].pid, NULL, 0);

		clock_gettime(CLOCK_MONOTONIC, &completion_time[i]);    // Completion Time
        response_time[i] = (completion_time[i].tv_sec - ready.tv_sec) + ((completion_time[i].tv_nsec - ready.tv_nsec) / 1e9);
        printf("Response Time for Task %d: %.10f seconds\n", processes[i].task_number, response_time[i]);
    }
    average_response_time = (response_time[0] + response_time[1] + response_time[2] + response_time[3]) / 4;
    printf("Average Response Time: %.10f seconds\n", average_response_time);
    printf("All tasks completed.\n");
	/************************************************************************************************
		- Scheduling code ends here
	************************************************************************************************/

	return 0;
}