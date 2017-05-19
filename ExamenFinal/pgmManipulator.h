#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>


//*************************************************

//code written By Adrian Biller Alcantara A01018940
//Advanced programming course
// Final exam conways game of life

//*************************************************

//used functions
void readFile(char * imageName, int iterations);
void createFile(int width, int height, int canvas[][height], int generation);
int lifeGame(int posx, int posy, int width, int height,  int canvas[][height]);
