/**
 * Editing image with parallelism
 *
 * Timothy Pratt
 * 11/30/2023
 */

#include <stdio.h>
#include <stdlib.h>
#include "ppmIO.h"
#include <pthread.h>
#include <unistd.h>
#define NUM_THREADS 4 //extention: can be any number of threads

Pixel *src;
int rows, cols, colors;

typedef struct
{
    int id;
} threadInfo;

void *img_thread(void *threadinfo)
{
    threadInfo *ti = (threadInfo *) threadinfo;
    
    for (int j = 0; j < 10; j++)
    {
        for (int i = ti->id * rows * cols / NUM_THREADS; i < (ti->id + 1) * rows * cols / NUM_THREADS; i++)
        {
            src[i].r = src[i].r > 128 ? (220+src[i].r)/2 : (30+src[i].r)/2;
            src[i].g = src[i].g > 128 ? (220+src[i].g)/2 : (30+src[i].g)/2;
            src[i].b = src[i].b > 128 ? (220+src[i].b)/2 : (30+src[i].b)/2;
        }
    }
    
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    clock_t begin1 = clock();
    int i;

    // check usage
    if( argc < 2 )
    {
        printf("Usage: %s <image filename>\n", argv[0]);
        exit(-1);
    }

    // read image and check for errors
    src = ppm_read(&rows, &cols, &colors, argv[1]);
    
    if(!src)
    {
        printf("Unable to read file %s\n", argv[1]);
        exit(-1);
    }

    threadInfo ti[NUM_THREADS];
    pthread_t thread[NUM_THREADS];
        
    for (i = 0; i < NUM_THREADS; i++)
    {
        ti[i].id = i;
    }
    
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&(thread[i]), NULL, img_thread, &(ti[i]));
    }
    
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(thread[i], NULL);
    }

	// write out the image
	ppm_write(src, rows, cols, colors, "bold.ppm");
	free(src);
    clock_t end1 = clock();
    double time1 = (double)(end1 - begin1) / CLOCKS_PER_SEC;
    printf("Overall Time: %f\n", time1);
	return(0);
}
