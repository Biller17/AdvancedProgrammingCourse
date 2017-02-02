#include "ppmManipulator.h"
/*struct pixel {
    char red;         this struct was meant to be used but it interfered with
    char green;           the size of the arrays and had to be discarded
    char blue;
};*/



//*************************************************

//code written By Adrian Biller Alcantara A01018940
//Advanced programming course
// simple image manipulation homework

//*************************************************


void readPPM(char** imageName)
{
    int height;
    int width;       //height and width of file plus the base number for the colors
    int maximumColor;
    FILE *original;
    FILE *modified;
    char ptype[3]; //file type

    original = fopen(imageName, "rb"); //opening original image  with read binary
    checkFile(original); //checking if file opened correctly

    fscanf(original, "%2s", ptype); //checking if the file is a type P6
    ptype[2] = '\0';
    if(strcmp(ptype, "P6") != 0) {
        printf("There was an error while reading the file\n File is not P6 type");
        exit(0);
    }
    modified = fopen("modifiedImage.ppm", "wb"); //Creating new "modifiedImage.ppm" file with write binary
    checkFile(modified);
    fprintf(modified, "%s\n", ptype); //printing file type
    fscanf(original, "%d", &width); //scanning width
    fscanf(original, "%d", &height);//scanning height
    fscanf(original, "%d", &maximumColor);// getting the base color number
    fprintf(modified, "%d %d\n", width, height); //printting the obtained info to the new file
    fprintf(modified, "%d\n", maximumColor);
    modify(original, modified, height, width); //function that will read, modify and write the pixels
    fclose(modified); //closing files
    fclose(original);
}

void checkFile(FILE *file){
  //checking if the file was correctly opened or read
  if(!file) {
      printf("There was an error while reading the file, exiting now.");
      exit(0);
  }
}


void modify(FILE *original, FILE *modified, int height, int width){
  unsigned char pixels[3]; ////replacement for struct with and unsigned char array
  for(int i = 0; i < height; i++){
    for(int j = 0; j< width; j++){
      fread(pixels, 3, 1, original); //reads three components of the pixel
      pixels[0] = 255 - pixels[0];//modify and asign the three new components
      pixels[1] = 255 - pixels[1];
      pixels[2] = 255 - pixels[2];
      fwrite(pixels, 1, 3, modified);//writing the 3 components to the new file
    }
  }
}


int main(int argc, char **argv){
  readPPM(argv[1]);
}
