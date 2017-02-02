#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void readPPM(char** imageName);

void checkFile(FILE* file);

void modify(FILE *original, FILE *modified, int height, int width);
