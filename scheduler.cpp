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
#include <fstream>
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
        int time = 0; // timer

        struct ready { // create struct
            int arrive; // arrival time
            int burst; // CPU burst
            int process; // process number
            int wait; // wait time
        };

        write(pipeline[1][1], &n, sizeof(n)); // writes n to pipe

        close(pipeline[0][1]);
        close(pipeline[1][0]);

        vector<ready> readyQueue;
        vector<ready> orderQueue;
        ready* o = new ready; // initialize struct

        for (int i = 0; i < n; i++){ // reads n times
            read(pipeline[0][0], o, sizeof(o)); //reads struct from pipe
            readyQueue.push_back(ready()); // adds a struct to the ready queue
            readyQueue[i].arrive = o->arrive; // saves arrival time
            readyQueue[i].burst = o->burst; // saves CPU burst
            readyQueue[i].process = i; // saves process number
        }

        for (int i = 0; i < n; i++){ // n times
            while (time < readyQueue[0].arrive){
                time += 1;
            }
            int j = 0;
            int loc = 0;
            while (j < readyQueue.size() and time >= readyQueue[j].arrive){
                if (readyQueue[j].burst < readyQueue[loc].burst){
                    loc = j;
                }
                j += 1;
            }
            readyQueue[loc].wait = time - arrive;
            time += readyQueue[loc].burst;
            orderQueue.push_back(readyQueue[loc]);
            readyQueue.erase(readyQueue.begin()+loc);
        }

        //write to file
        ofstream myfile("record.txt");
        myfile << "Arrival time" << " " << "Process" << " " << "CPU burst" << " " << "Waiting time" << endl;
        for (int i=0; i < n; i++){
            myfile << orderQueue[i].arrive << ' ';
            myfile << orderQueue[i].process << ' ';
            myfile << orderQueue[i].burst << ' ';
            myfile << orderQueue[i].wait << ' ';
            myfile << endl;
        }
        myfile.close();

        orderQueue.clear();
        orderQueue.resize(0);
        readyQueue.clear();
        readyQueue.resize(0);

        close(pipeline[1][1]); // close write to childrens pipelines
        close(pipeline[0][0]); //close read
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
