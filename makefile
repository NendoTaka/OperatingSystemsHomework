#Course: CSC460, Homework 5 Programmers: Klayton Hawkins, Kendall Lewis Grok Usernames: kh, kl Contribution 
#Percentage: 50%, 50% Completed Subtasks: Klayton: Piping data to children, choosing process, and 
#commenting Kendall: Child file, Makefile, and data output Last Modified Date: December 1, 2015 
#Description:This program creates children to estimate integral Files: makefile Scheduler.cpp child.cpp 
#Instructions for compilation and execution: 1. type: make 2. type: ./scheduler n (to execute)
all: scheduler child scheduler: scheduler.cpp
	c++11 scheduler.cpp -o scheduler child: child.cpp
	c++11 child.cpp -o child
