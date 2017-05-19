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
  int generation = 0;
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
  int temp[width][height];

  //scanning file to fill canvas
  for(int i = 0; i < width; i++){
    for(int j = 0; j < height; j++){
      fscanf(lifeOrigin, "%d", &bit);
      canvas[i][j] = bit;
    }
  }
  //printinf canvas
  /*for(int i = 0; i < width; i++){
    for(int j = 0; j < height; j++){
      printf("%d ",canvas[i][j]);
    }
    printf("\n");
  }*/


  //creating master thread private variables just generations, the other are shared
  #pragma omp parallel private(generation) shared(width,height, canvas, temp, iterations)
  {

    #pragma omp while
    //iteration depending on the generations wanted
    while(generation < iterations){
      //copying canvas to temp for further use
      for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
          temp[i][j] = canvas[i][j];
        }
        printf("\n");
      }

      //applying game of life to each cell
      for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
          canvas[i][j] = lifeGame(i, j,width, height, temp);
        }
      }
      //creating file with current canvas
      createFile(width, height, canvas, generation);
      generation++;
    }
  }
}

//where the actual lifegame algorithm takes place
int lifeGame(int posx, int posy,int width, int height,  int canvas[][height]){

//start current cell lives
  int lives = 0;
//get the lifestate of the cell
  int current = canvas[posx][posy];

//for that checks all the
  for(int i = posx-1; i <= posx+1; i++){
    for(int j  = posy-1; j <= posy+1; j++){
      if(i >= 0 && j >= 0 && i < width && j < height){
        if(canvas[i][j] == 1){
          lives ++;
        }
      }
    }
  }

  //because the previous for also counts the life of the cell itself
  //then one life is taken off so it has only the neighbours lives
  if(current == 1){
    lives --;
  }

  //printing lives for debbuging purposes
  printf("%d\n", lives );

  //including the ifs for any case the cell might be
  //depending on the neighbours and the current state of the cell
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
//creating the pgm file
void createFile(int width, int height, int canvas[][height], int generation){
  char ptype[3] = "P2"; //file type
  FILE *lifeIteration;
  char name[30];
  //creating the name of each pgm iteration of the game
  sprintf(name, "lifeFile_%d.pgm", generation);
  lifeIteration = fopen(name, "wb");
  fprintf(lifeIteration, "%s\n", ptype); //printing file type
  fprintf(lifeIteration, "%d %d\n", width, height); //printting the obtained info to the new file
  fprintf(lifeIteration, "%d\n", 1);
  //printing the actual canvas
  for(int i = 0; i < width; i++){
    for(int j = 0; j < height; j++){
      fprintf(lifeIteration, "%d ",canvas[i][j]);
    }
    fprintf(lifeIteration, "\n");
  }
}


int main(int argc, char **argv){

  int iterations;
  //receiving the char* argv and changing it to int for the function
  if (sscanf (argv[2], "%i", &iterations) != 1) {
    fprintf(stderr, "error - you must enter the number of iterations\n");
  }
  readFile(argv[1], iterations);

}
