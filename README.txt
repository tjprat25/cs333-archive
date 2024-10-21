CS333 - Project 8 - README
Timothy Pratt
12/08/2023
Google Sites URL: https://sites.google.com/colby.edu/tim-pratt-cs333?usp=sharing

Directory Layout:
Project8_tjprat25
C	benford_par1.c	benford_par2.c	benford_par3.c	benford_par4.c	benford_par5.c	benford_par6.c	benford_sequential.c	bold.ppm	colorize_old.c	colorize.c	IMG_4203.ppm	longer_nonBenford.bin	longer.bin	medium.bin	my_timing.c	my_timing.h	ppmIO.c	ppmIO.h	super_short.bin
Java	Colorizer.java	IMG_4203.ppm	output.ppm	Pixel.java	PPMFile.java
Run and build configuration for C:
Apple clang version 14.0.3 (clang-1403.0.22.14.1)

Description for each subsection:
C:
This is the primary programming task in C. There are 9 main C files, seven for the Benford task, and two for the colorizer task.

Compile:
gcc -o benford_sequential my_timing.c benford_sequential.c -lm -lpthread
OR
gcc -o benford_par[1-6] my_timing.c benford_par[1-6].c -lm -lpthread

	
Run:
./benford_sequential
OR
./benford_par[1-6]

	Task 1:
	The seven Benford files each print the leading digit counts of a set of numbers (in this case they are given in longer.bin). Each approaches the problem in a slightly different way. benford_sequential.c is a standard sequential version. The other six use some sort of parallelism. The mean runtime of each is shown below. Most have comparable runtimes. The only version with a significantly higher runtime was the 2nd parallel version. This version had the most mutex calls, which is likely the reason for the long runtime. The best runtime was from the 4th parallel version, which is unexpected because it does not have the least amount of mutex calls, as far as the parallel versions go. This could be because each mutex is only active for a short time, alongside the overall benefits of parallelism. Since mutexes add a noticeable amount of runtime, they should only be used to ensure a program's accuracy.

	Sequential:								0.02760
	Global Counter Array Protected by Single Mutex:				0.028
	Global Counter Array Protected by Array of Mutexes:			0.03983
	Local Counter Array, with Final Update Protected by Single Mutex:	0.0281
	Local Counter Array, with Final Update Protected by Array of Mutexes:	0.02607
	Global Counter Array of Arrays, Grouped by Thread, no Mutex:		0.02906
	Global Counter Array of Arrays, Grouped by Digit, no Mutex:		0.02815

	Output:
	There are 312705 1's
	There are 177336 2's
	There are 121034 3's
	There are 92637 4's
	There are 75909 5's
	There are 65134 6's
	There are 57202 7's
	There are 51298 8's
	There are 46745 9's
	It took _ seconds for the whole thing to run

Compile:
gcc -o colorize_old -I. colorize_old.c ppmIO.c -lm
OR
gcc -o colorize -I. colorize.c ppmIO.c -lm
	
Run:
./colorize_old IMG_4203.ppm
OR
./colorize IMG_4203.ppm

	Task 2:
	The two colorize files apply a filter to each pixel of a .ppm image. The first file is the default sequential version. The second version uses parallelism, and was ran with 1, 2, and 4 threads. As an extension, this file supports N threads by changing the #define NUM_THREADS line. The mean runtimes for these files are displayed below. Surprisingly, the threads did not improve the runtime of this task. In fact, adding more threads only added to the overall time. To me, this says one of a few different things. One explanation is that my Mac does not have the processing power to efficiently run multiple threads in the way they are implemented here. Another option is that the start-up/clean-up involved with this file adds too much time to be worth it.

	Sequential:								0.14997
	1 Thread:								0.16452
	2 Threads:								0.19472
	4 Threads:								0.24842


Java detailed on Google Site.