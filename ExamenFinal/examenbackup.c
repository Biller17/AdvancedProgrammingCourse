#include "pgmManipulator.h"

//*************************************************

//code written By Adrian Biller Alcantara A01018940
//Advanced programming course
// Final exam conways game of life

//*************************************************

void checkFile(FILE *file){
  //checking if the file was correctly opened or read
  if(!file) {
      printf("There was an error while reading the file, exiting now.");
      exit(0);
  }
}

void readFile(char * imageName, int iterations){
  int height;
  int width;
  int bit;
  FILE *lifeOrigin;

   //opening original image  with read binary
  lifeOrigin = fopen(imageName, "rb");
  //checking if file opened correctly
  checkFile(lifeOrigin);

  //reading width
  fscanf(lifeOrigin, "%d", &width);
  //reading height
  fscanf(lifeOrigin, "%d", &height);

  printf("width: %d height: %d   iterations: %d\n",width, height, iterations);
  int canvas[width][height];

  //scanning file to fill canvas
  for(int i = 0; i < width; i++){
    for(int j = 0; j < height; j++){
      fscanf(lifeOrigin, "%d", &bit);
      canvas[i][j] = bit;
    }
  }
  //printinf canvas
  for(int i = 0; i < width; i++){
    for(int j = 0; j < height; j++){
      printf("%d ",canvas[i][j]);
    }
    printf("\n");
  }
  //applying game of life to each cell
  for(int i = 0; i < width; i++){
    for(int j = 0; j < height; j++){
      canvas[i][j] = lifeGame(i, j,width, height, canvas);
    }
  }
  printf("\n");


}


int lifeGame(int posx, int posy, int width, int height,  int canvas[][height]){


  for(int i = 0; i < width; i++){
    for(int j = 0; j < height; j++){
      printf("%d ",canvas[i][j]);
    }
    printf("\n");
  }
  int lives = 0;

  int current = canvas[posx][posy];

  for(int i = posx-1; i <= posx+1; i++){
    for(int j  = posy-1; j <= posy+1; j++){
      if(i >= 0 && j >= 0 && i < width && j < height){
        if(canvas[i][j] == 1){
          lives ++;
        }
      }
    }
  }
  if(current == 1){
    lives --;
  }
  printf("%d\n", lives );
  if(lives < 2){
    return 0;
  }
  else if(lives == 2 && current == 1){
    return 1;
  }
  else if(lives == 2 && current == 0){
    return 0;
  }
  else if(lives == 3 && current == 1){
    return 1;
  }
  else if(lives > 3){
    return 0;
  }
  else if(lives == 3 && current == 0){
    return 1;
  }
}

void createFile(int width, int height, int canvas[][height], int generation){
  char ptype[3] = "P2"; //file type
  FILE *lifeIteration;
  char* name = generation + ".pgm";
  lifeIteration = fopen(name, "wb");
  fprintf(lifeIteration, "%s\n", ptype); //printing file type
  fprintf(lifeIteration, "%d %d\n", width, height); //printting the obtained info to the new file
  fprintf(lifeIteration, "%d\n", 1);
  for(int i = 0; i < width; i++){
    for(int j = 0; j < height; j++){
      fprintf(lifeIteration, "%d ",canvas[i][j]);
    }
    fprintf(lifeIteration, "\n");
  }
}


int main(int argc, char **argv){

  int iterations;
  if (sscanf (argv[2], "%i", &iterations) != 1) {
    fprintf(stderr, "error - you must enter the number of iterations\n");
  }
  readFile(argv[1], iterations);

}
