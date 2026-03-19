# COMPE571 – Embedded Operating Systems – Programming Assignment 2
**Authors**: Vincent Pierce, Kyle Lainez
-
In this assignment, multiple scheduling algorithms are implemented at the user level, to accomplish four workloads represented in four different processes. The assignment does not attempt to answer which schedule is best, instead it aims to answer what are the advantages and drawbacks for each schedule by comparing, completion and response time as workload varies, and depending on the scheduling algorithm, quantum time.  

# Usage
Compile and run each program using the commands below:

### Round Robin
```
gcc RoundRobin.c -o RoundRobin
./RoundRobin
```

### First Come First Serve
```
gcc FCFS.c -o FCFS
./FCFS
```

### Shortest Job First
```
gcc SJF.c -o SJF
./SJF
```

### Multilevel Feedback Queue
```
gcc MLFQ.c -o MLFQ
./MLFQ
```