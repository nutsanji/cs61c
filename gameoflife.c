/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	uint32_t rows, cols, center;
	rows = image->rows;
	cols = image->cols;
	center = row * cols + col;

	//read the eight neighbors of the cell
	int num = 8;
	int nx[num], ny[num];
	const int px[] = {-1, -1, -1, 0, 0, 1, 1, 1};
	const int py[] = {-1, 0, 1, -1, 1, -1, 0, 1};

	for (int i = 0; i < num; i++) {
		nx[i] = (row + px[i]) % rows; 
		ny[i] = (col + py[i]) % cols;
	}

	//determine the color of the given cell
	int aR, aG, aB;					//alive
	int aNR = 0, aNG = 0, aNB = 0;  //number of alive neighbours

	aR = (*(image->image + center))->R == 255;
	aG = (*(image->image + center))->G == 255;
	aB = (*(image->image + center))->B == 255;

	for (int i = 0; i < num; i++) {
		Color *neighbor = *(image->image + (nx[i] * cols + ny[i]));
		aNR += (neighbor->R == 255) ? 1 : 0;
		aNG += (neighbor->G == 255) ? 1 : 0;
		aNB += (neighbor->B == 255) ? 1 : 0;
	}

	//the key part;
	int shiftR = 9 * aR + aNR;
	int shiftG = 9 * aG + aNG;
	int shiftB = 9 * aB + aNB;
	Color *cell = (Color *)malloc(sizeof(Color));

	cell->R = (rule & (1<<shiftR)) ? 255 : 0; 
	cell->G = (rule & (1<<shiftG)) ? 255 : 0; 
	cell->B = (rule & (1<<shiftB)) ? 255 : 0; 

	return cell;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	Image *nextImg = (Image *)malloc(sizeof(Image));
	nextImg->rows = image->rows;
	nextImg->cols = image->cols;
	nextImg->image = (Color **)malloc(sizeof(Color *) * (nextImg->rows)*(nextImg->cols));

	Color **p = nextImg->image;
	for (int i = 0; i < nextImg->rows; i++) {
		for (int j = 0; j < nextImg->cols; j++) {
			*p = evaluateOneCell(image, i, j, rule);
			p++;
		}
	}

	return nextImg;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	if (argc != 3) {
		printf("usage: ./gameOfLife filename rule\n");
		printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
		printf("rule is a hex number beginning with 0x; Life is 0x1808.\n");
		return -1;
	}

	Image *img = readData(argv[1]);
	uint32_t rule = strtol(argv[2], NULL, 16);

	Image *lifeImg = life(img, rule);
	writeData(lifeImg);
	freeImage(img);
	freeImage(lifeImg);
	return 0;
}
