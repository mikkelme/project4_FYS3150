# FYS 3150 - Project 4
This repository contains the work of Fredrik Hoftun and Mikkel Metzsch Jensen for project 4 in Computational Physics FYS3150. 

The repository is divided into two folders: `article` and `code`. 

* In the article folder we have the article it self and all the figures included in it. 
* In the code folder we have the main code main.cpp with the additional functions.cpp and functions.h used by the main file. The version of the main.cpp outputs a variety files when used such as writing a expectation values (logfile for first temperature step), accepted spins and energy states per cycle. A similar set of files is found inside the folder `Parallelization` which is a streamlined version used for the phase transistion. This is parallelized using MPI and only have the ability to print timing results (you just have to uncomment this inside the main.cpp). A second folder called `Plotting` contains different Python-scripts for plotting the computed values.

To use the compiler just type `make` in the commandline in the `/code` or `/code/Parallelization` directory.
The compiler will generate a file `main.exe` which takes the following arguments in the command line: Output filename, number of spins, MC cycles, initial temperature, final temperature, tempurate step. For the orginal version you will have to write "log" as the last command line argument to produce the logfile of expectationvalues.
