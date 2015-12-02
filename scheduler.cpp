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
    if (argc != 5){ // error checking for number of arguments
        cout << "Invalid number of arguments: needs 4 got " << argc-1 << endl;
        exit(0);
    }

    int left, right, n, m; //variables for arguments
    int pipeline[9][2]; //array for pipelines
    int cid[8]; //array for child pids
    int count = 0; //keep track of which child it is
    int numTasks[8]; // array to keep track of how many tasks each child completes

    for (int i = 0; i < 8; i++){ // initialize each numTasks index to 0
        numTasks[i] = 0;
    }

    // reads arguments as int
    left = atoi(argv[1]);
    right = atoi(argv[2]);
    n = atoi(argv[3]);
    m = atoi(argv[4]);

    if (m <= 0 or n <= 0) { //if atoi fails on any argument or n or m is 0
        cout << "m or n was not an integer or was too small!" << endl;
        exit(0);
    }

    pid_t pid = getpid(); //saves parent id

    if (left >= right){ //error handling for right not greater than left
        cout << "The right value must be greater than the left value!" << endl;
        cout << "The provided right value was " << right;
        cout << " and the provided left value was " << left << endl;
        exit(0);
    }

    if (m > 8){ // error handling for number of children
        cout << "The provided value for m is " << m << " which is greater than 8" << endl;
        exit(0);
    }

    if (n < m){ // if less n than number of children reduce children
        m = n;
    }

    float inc = (right - left)/(n * 1.0); //size to increment for each set of numbers
    float currL, currR; //left and right passed to children
    int still = n; //keeping track of how many writes are left to children
    currL = left; //set original currL to left
    currR = left + inc; //set original currR to right
    pid_t child; //initialize child pid

    pipe(pipeline[8]); //opens pipe for children to write to

    for (int i = 0; i < m; i++){
        if (getpid() == pid){ // if parent
            count = i;
            pipe(pipeline[i]); // open child read pipe
            child = fork(); // fork and save pid
            if (-1 == child){ // if fork failed
                cout << "Parent failed to create child" << endl;
                exit(0);
            }
            if (getpid() == pid){ // if parent
                close(pipeline[i][0]); //close read end for parent
                write(pipeline[i][1], &currL, sizeof(currL)); //write left to child
                write(pipeline[i][1], &currR, sizeof(currR)); //write right to child
                currL = currR; //increments currL by inc
                currR = currR + inc; //increments currR by inc
                still -= 1; //-1 to remaining writes left
                cid[i] = child; // saves child id to cid
                numTasks[i] += 1;// adds one to number of tasks for child
            }
        }
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
        close(pipeline[count][1]); // closes write to childs pipeline
        close(pipeline[8][0]); // closes read end of parents pipeline
        //child executes file child
        execl("child", "", to_string(pipeline[count][0]).c_str(),
              to_string(pipeline[8][1]).c_str(), to_string(count).c_str(), NULL);
        cout << "Child failed to exec" << endl;
    }

    return 0;
}
