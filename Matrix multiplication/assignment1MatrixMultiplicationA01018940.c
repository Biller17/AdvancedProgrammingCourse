#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 //Adrian Biller A01018940
 //Matrix Multiplication assignment
 //Programacion avanzada

int main(){
    int columnA = 3;
    int rowA = columnA;//depending on the number of rows and columns of each matrix these implementation should change
    int rowB = columnA;
    int columnB = rowB;
    double matrixA [rowA][columnA];
    float matrixB [rowB][columnB];
    float answer [rowA][columnB];
    float num;
    FILE *file; //FILE  variable where both .txt will be assigned


    file = fopen("matrixA.txt", "r"); //opening and using the first matrix file
    for(int i = 0; i < rowA; i++){
      for(int j = 0; j < columnA; j++){
        fscanf(file, "%f", &num);//creating a 2d array for the matrix and scanning each number into a float
        matrixA[i][j] = num;
      }
    }
    fclose(file);//closing file for further use with another file


    //same process as before only with another file
      file = fopen("matrixB.txt", "r");
      for(int i = 0; i < rowB; i++){
        for(int j = 0; j < columnB; j++){
          fscanf(file, "%f", &num);
          matrixB[i][j] = num;
        }
      }
      fclose(file);


      num = 0; //resetting num variable for use in the Multiplication
      //three fors which multiply depending on the dimensions of the matrices
      for(int i = 0; i < rowA; i++){
        for(int j = 0; j < columnB; j++){
          for(int k = 0; k < rowB; k++){
              num = num + matrixA[i][k] * matrixB[k][j];
          }
          answer[i][j] = num;
          num = 0;
        }
      }

//printing the result matrix
      printf("Answer:\n");
      for(int i = 0; i < rowA; i++){
        for(int j = 0; j < columnB; j++){
          printf("%f  ", answer[i][j]);
        }
        printf("\n");
      }
    return 0;
}
