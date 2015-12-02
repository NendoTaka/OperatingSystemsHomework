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
#include <stdio.h>
#include <math.h>
#include <signal.h>
#include <vector>
using namespace std;

int main(int argc, char **argv)
{
    if (argc != 2){ // error checking for number of arguments
        cout << "Invalid number of arguments: needs 1 got " << argc-1 << endl;
        exit(0);
    }

    int n; //variable for arguments
    int pipeline[2][2]; //array for pipelines
    int child; // used for fork

    // reads arguments into int
    n = atoi(argv[1]);

    if ( n <= 0) { //if atoi fails on any argument or n or m is 0
        cout << "n was not an integer or was too small!" << endl;
        exit(0);
    }

    pid_t pid = getpid(); //saves parent pid

    pipe(pipeline[0]); // opens parent pipe
    pipe(pipeline[1]); // opens child pipe

    child = fork(); // fork and save pid
    if (-1 == child){ // if fork failed
        cout << "Parent failed to create child" << endl;
        exit(0);
    }

    if (pid == getpid()){ //if parent
        float total = 0.0; // initialize total
        struct out { // create struct
            int num; //which child
            float area; //area
        };

        out* o = new out; // initialize struct
        for (int i = 0; i < n; i++){ // reads n times
            close(pipeline[8][1]); //close write to parents read pipeline
            read(pipeline[8][0], o, sizeof(o)); //reads struct from pipe
            total += o->area; // adds area to total
            if (still > 0){ // if you still have remaining calculations
                write(pipeline[o->num][1], &currL, sizeof(currL)); //write left to child
                write(pipeline[o->num][1], &currR, sizeof(currR)); //write right to child
                currL = currR; //increments currL by inc
                currR = currR + inc; // increments currR by inc
                still -= 1; //-1 to remaining writes left
                numTasks[o->num] += 1; // adds one to number of tasks for child
            }
        }
        for (int i = 0; i < m; i++){ //loop to kill children
            close(pipeline[i][1]); // close write to childrens pipelines
            kill(cid[i], SIGKILL); //kills child
            cout << "Child " << i << " computed " << numTasks[i] << " trapezoids" << endl;
        }

        cout << endl << "The total calculated area is " << total << endl; //print total

        close(pipeline[8][0]); //close read
    }
    else {
        close(pipeline[1][1]); // closes write to childs pipeline
        close(pipeline[0][0]); // closes read end of parents pipeline
        //child executes file child
        execl("child", "", to_string(pipeline[1][0]).c_str(), to_string(pipeline[0][1]).c_str(), NULL);
        cout << "Child failed to exec" << endl;
    }

    return 0;
}
