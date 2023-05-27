/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	//open the filename
	FILE *fp = fopen(filename, "r");

	//allocate a memory address for img
	Image *img = (Image *) malloc(sizeof(Image));
	
	//read the head information
	uint32_t max_scale;
	fscanf(fp, "P3 %u %u %u", &(img->cols), &(img->rows), &max_scale);

	//read the remainder of image data
	int i;
	int total_pixels = (img->cols) * (img->rows);

	img->image = (Color **) malloc(sizeof(Color*) * total_pixels);
	for (i = 0; i < total_pixels; i++) {
		*(img->image + i) = (Color *)malloc(sizeof(Color));
		Color *pixel = *(img->image + i);
		fscanf(fp, "%hhu %hhu %hhu", &pixel->R, &pixel->G, &pixel->B);
	}

	// close the file descriptor.
	fclose(fp);
	return img;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	int i, j;
	Color **p;
	uint32_t rows, cols;

	rows = image->rows;
	cols = image->cols;
	p = image->image;	

	printf("P3\n%u %u\n255\n",cols, rows);

	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols-1; j++) {
			printf("%3hhu %3hhu %3hhu   ", (*p)->R, (*p)->G, (*p)->B);
			p++;
		}
		printf("%3hhu %3hhu %3hhu\n", (*p)->R, (*p)->G, (*p)->B);
		p++;
	}
}

//Frees an image
void freeImage(Image *image)
{
	int i;
	int total_pixels = (image->rows) * (image->cols);
	for (i = 0; i < total_pixels; i++) {
		free(*(image->image + i));
	}
	free(image->image);
	free(image);
}

