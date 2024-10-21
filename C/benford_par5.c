/* benford_par5.c
 * Timothy Pratt
 * Global Counter Array of Arrays, Grouped by Thread, no Mutex
 */
 
// Sequential implementation.
// output for medium.bin on nscc-n3
// There are 3217 1's
// There are 1779 2's
// There are 1121 3's
// There are 907 4's
// There are 745 5's
// There are 668 6's
// There are 591 7's
// There are 495 8's
// There are 477 9's
// It took 0.000567 seconds for the whole thing to run
//
// output for longer.bin on nscc-n3
// There are 312705 1's
// There are 177336 2's
// There are 121034 3's
// There are 92637 4's
// There are 75909 5's
// There are 65134 6's
// There are 57202 7's
// There are 51298 8's
// There are 46745 9's
// It took 0.055768 seconds for the whole thing to run

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <math.h>
#include "my_timing.h"
#include <pthread.h>
#include <unistd.h>
#define NUM_THREADS 8

// Global variables
int global_counts[NUM_THREADS][10] = {};
int N = 0;
double *data;

typedef struct
{
    int id;
} threadInfo;

// Load data from a binary file that has an int and then
// a sequence of doubles. The value of the int should indicate
// the number of doubles in the sequence.
// Load the data into global variables N and data.
int loadData(char *filename) {
  FILE *fp;

  if(filename != NULL && strlen(filename))
    fp = fopen(filename, "r");
  else
    return -1;

  if (!fp)
    return -1;

  fread( &N, sizeof(int), 1, fp );
  data = (double*)malloc( sizeof(double)*N );
  fread( data, sizeof(double), N, fp );
  fclose( fp );
  // Uncomment this to verify the right data are being read in.
  // For super_short.bin, it should print out
  // data[0] = 97.137926
  // data[1] = 24.639612
  // data[2] = 55.692572
//   int i;
//   for (i = 0; i < N; i++) {
//         printf( "data[%d] = %f\n", i, data[i] );
//   }
  return 1; // success
}

// Return the leading Digit of n.
int leadingDigit( double n ) {
    // This is not a particularly efficient approach.
    if (fabs(n) == 1.0)
        return 1;
    else if (fabs(n) == 0.0)
        return 0;
    else if (fabs(n) < 1.0) {
        // multiply it by 10 until you get a number that is at least 1.
        // Then chop off the fractional part. All that remains is the first digit.
        double tmp = fabs(n);
        while (tmp < 1.0) {
            tmp *= 10.0;
        }
        return (int)floor( tmp );
    }
    else {
        // Divide it by 10 until you get a number smaller than 10.
        // That number will be the first digit of the original number.
        long long unsigned in = (long long unsigned) floor(fabs(n));
        while (in > 9) {
            in /= 10;
        }
        return in;
    }
} // end leadingDigit

void *benford_thread(void *threadinfo)
{
    threadInfo *ti = (threadInfo *) threadinfo;
    
    for (int i = (ti->id / NUM_THREADS) * N; i < ((ti->id + 1) / NUM_THREADS) * N; i++)
    {
        int d = leadingDigit(data[i]);
        global_counts[ti->id][d]++;
    }
    
    pthread_exit(NULL);
}

/* Main routine. */
int main(int argc, char* argv[]) 
{
    double t1, t2;
    int i;
    
    // Load the data
    int succ;
    // succ = loadData( "super_short.bin" );
    // succ = loadData( "medium.bin" );
    succ = loadData( "longer.bin" );
    if (!succ) { return -1; }
    
    // Start the timer after we have loaded the data.
    t1 = get_time_sec();
    
    threadInfo ti[NUM_THREADS];
    pthread_t thread[NUM_THREADS];
    
    for (i = 0; i < NUM_THREADS; i++)
    {
        ti[i].id = i;
    }
    
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&(thread[i]), NULL, benford_thread, &(ti[i]));
    }
    
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(thread[i], NULL);
    }
    
    // End the timer
    t2 = get_time_sec();
    
    int total_counts[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    for (i = 1; i < 10; i++)
    {
        for (int j = 1; j < NUM_THREADS; j++)
        {
            total_counts[i] = total_counts[i] + global_counts[j][i];
        }
    }
    
    for (i = 1; i < 10; i++) {
        printf( "There are %d %d's\n", total_counts[i], i );
    }
    
    printf("It took %f seconds for the whole thing to run\n",t2-t1);
    
    // We are responsible for calling loadData, so we are responsible
    // for freeing the data array.
    free( data );
    return 0;
} // end main

