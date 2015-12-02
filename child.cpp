/*Course: CSC460, Homework 5
Programmers: Klayton Hawkins, Kendall Lewis
Grok Usernames: kh, kl
Contribution Percentage: 50%, 50%
Completed Subtasks:
Klayton: Piping data to children, choosing process, and commenting
Kendall: Child file, Makefile, and data output
Last Modified Date: December 1, 2015
Description:This program creates children to estimate integral
Files: makefile Scheduler.cpp child.cpp
Instructions for compilation and execution:
1. type: make
2. type: ./scheduler n (to execute)*/

#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

int main(int argc, char **argv)
{
    int pipeline[2]; // array for pipes
    int num; // int for number of children
    int timeLapse, cpuBurst, arrivalTime; // initialize variables
    struct out { // create struct
        int arrive; // arrival time
        int burst; // CPU burst
    };
    srand(time(NULL)); // initialize random seed
    out* o = new out; // initialize struct

    pipeline[0] = atoi(argv[1]); // saves read pipe read end
    pipeline[1] = atoi(argv[2]); // saves write pipe write end
    read(pipeline[0], &num, sizeof(num)); // read number of processes

    for (int i = 0; i < num; i++){ // loop n times
        timeLapse = rand() % 50; // generates random time lapse
        cpuBurst = rand() % 1000 + 1; // generates random CPU burst

        arrivalTime += timeLapse; // calculates the new arival time
        o->arrive = arrivalTime; // saves arrival time to PCB
        o->burst = cpuBurst; // saves Burst to PCB

        write(pipeline[1], o, sizeof(o)); // writes struct to pipe
    }
    close(pipeline[1]); // close write to childrens pipelines
    close(pipeline[0]); //close read
    return 0;
}
