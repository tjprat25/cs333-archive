#include <stdio.h>
#include <stdlib.h>
#include "ppmIO.h"
#include <pthread.h>

int main(int argc, char *argv[]){
    clock_t begin1 = clock();
    Pixel *src;
	int rows, cols, colors;
	int i;

	// check usage
	if( argc < 2 ) {
		printf("Usage: %s <image filename>\n", argv[0]);
		exit(-1);
	}

	// read image and check for errors
	src = ppm_read( &rows, &cols, &colors, argv[1] );
	if( !src ) {
		printf("Unable to read file %s\n", argv[1]);
		exit(-1);
	}

	// process image
    for (int j = 0; j < 10; j++)
    {
        for(i=0;i<rows*cols;i++) {
            src[i].r = src[i].r > 128 ? (220+src[i].r)/2 : (30+src[i].r)/2;
            src[i].g = src[i].g > 128 ? (220+src[i].g)/2 : (30+src[i].g)/2;
            src[i].b = src[i].b > 128 ? (220+src[i].b)/2 : (30+src[i].b)/2;
        }
    }

	// write out the image
	ppm_write( src, rows, cols, colors, "bold.ppm" );

	free(src);
    clock_t end1 = clock();
    double time1 = (double)(end1 - begin1) / CLOCKS_PER_SEC;
    printf("Overall Time: %f\n", time1);
	return(0);
}

	
