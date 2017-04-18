/*Write a program that simulates a slot machine, like those seen in casinos.

You should create at least 3 threads, each of which will be in charge or changing the figures  in a reel.
by looping around a list of characters. The reels will be simulated
using an array of chars, and the spinning should be done by shifting the chars around the array.
Each reel will have different speeds, meaning that each will shift the characters in
the array a different amount of spaces (2, 3 or 4) each time. For this you will need
 to use a temporary array, and all reels must use the same temporary array, so you
 must control access to it to avoid conflicts.

Another thread will handle the user interface, waiting for the moment when a user
pulls the lever and then taking the characters from the reels controlled by the other
 threads. It will have to lock the three reels, before it can read the values.

Be sure to handle critical sections correctly, so that you can get the correct results.

You should allow the player to play more than one line for each pull. Here is a list of
online slots games that you can use as a reference of how your program should work.
*/


//slot machine with threads and mutex code written in c by Adrian Biller Alcantara A01018940 Advanced programming course
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define NUMTHREADS 4
#define LETTERS 7



//global variables
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
char tempWheel[7];
char firstWheel[7] = {'A', 'E' , 'J' , 'D', 'R', 'M', 'P'};
char secondWheel[7] = {'A', 'P' , 'D' , 'E', 'R', 'M', 'J'};
char thirdWheel[7] = {'J', 'E' , 'M' , 'R', 'A', 'P', 'D'};
int endgame = 0;




//function declaration
void * runWheel(void * args);
void * stop(void *args);



int main(){
  pthread_t tid[NUMTHREADS];
  int i;
  int status;
  //creating the three wheel threads

  printf("--------------------Welcome to the slot machine game!--------------------- \n");
  for (i=0; i<NUMTHREADS-1; i++){
       status = pthread_create(&tid[i], NULL, &runWheel, &i);
       if (status)
       {
           fprintf(stderr, "ERROR: pthread_create %d\n", status);
           exit(EXIT_FAILURE);
       }
       //printf("Thread created: %d\n", i);
   }

   status = pthread_create(&tid[3], NULL, &stop, NULL);
   if (status)
   {
       fprintf(stderr, "ERROR: pthread_create %d\n", status);
       exit(EXIT_FAILURE);
   }
   //printf("Thread created: 3\n");




   // waiting for threads to finish their work
   for (i=0; i<NUMTHREADS; i++){
    status = pthread_join(tid[i], NULL);
    if (status)
    {
        fprintf(stderr, "ERROR: pthread_join %d\n", status);
        exit(EXIT_FAILURE);
    }
  }

}

void * runWheel(void * args)
{
    //getting number of thread which is also number of speed
    int speed = *((int *)args);
    speed++;
    //printf("Receiving speed constant: %d\n", speed );

    //checking if its the first wheel
    if(speed == 1){
      //while until the game ends
      while(endgame == 0){
        //locking other threads
        pthread_mutex_lock(&mutex);

        //assigning the characters depending on the speed given

        for(int i = 0; i < LETTERS; i++){
          if(i+speed < LETTERS){
            tempWheel[i + speed] = firstWheel[i];
          }
          else if(i+speed >= LETTERS){
            tempWheel[i + speed - LETTERS] = firstWheel[i];
          }
        }
        //copying temporary variable to the original

        memcpy(firstWheel, tempWheel, sizeof(firstWheel));
        memset(&tempWheel, 0, sizeof(tempWheel));
        //unlocking mutex
        pthread_mutex_unlock(&mutex);

        /*printf("First wheel \t");
        for(int i = 0; i < LETTERS; i++){
          printf(" %c  |", firstWheel[i] );
        }
        printf("\n");*/

      }
    }

    //checking if its the second thread in the fucntion
    else if(speed == 2){
      while(endgame == 0){
        pthread_mutex_lock(&mutex);

        for(int i = 0; i < LETTERS; i++){
          if(i+speed < LETTERS){
            tempWheel[i + speed] = secondWheel[i];
          }
          else if(i+speed >= LETTERS){
            tempWheel[i + speed - LETTERS] = secondWheel[i];
          }
        }
        //copying temporary variable to the original
        memcpy(secondWheel, tempWheel, sizeof(secondWheel));
        memset(&tempWheel, 0, sizeof(tempWheel));
        pthread_mutex_unlock(&mutex);
        /*printf("\nSecond wheel \t");
        for(int i = 0; i < LETTERS; i++){
          printf("%c   |", secondWheel[i] );
        }
        printf("\n");*/

      }
    }

//checking if its the third thread in the function
    else if(speed == 3){
      while(endgame == 0){
        //locking thread
        pthread_mutex_lock(&mutex);

        for(int i = 0; i < LETTERS; i++){
          if(i+speed < LETTERS){
            tempWheel[i + speed] = thirdWheel[i];
          }
          else if(i+speed >= LETTERS){
            tempWheel[i + speed - LETTERS] = thirdWheel[i];
          }
        }
        //copying temporary variable to the original
        memcpy(thirdWheel, tempWheel, sizeof(thirdWheel));
        memset(&tempWheel, 0, sizeof(tempWheel));
        pthread_mutex_unlock(&mutex);
        /*printf("\nThird wheel: \t");
        for(int i = 0; i < LETTERS; i++){
          printf("%c   |", thirdWheel[i] );
        }
        printf("\n");*/
      }
    }
    return NULL;
}

void * stop(void *arg){
  int win = 0;
  char input = 'n';
  int credit = 200;
  int bet = 0;
  printf("Spinning wheels\n");
  printf("Your credit is %d \n      How much do you want to bet?\n", credit);
  scanf("%d", &bet);
  printf("Placing bet\n");
  credit-=bet;
  while(endgame == 0){
    scanf("%c", &input);
    if(input == '\n'){
      //locking all threads
      pthread_mutex_lock(&mutex);
      printf("Your result is....\n");
      for(int i = 0; i < LETTERS; i++){
        printf("\t \t |  %c  |   %c  |  %c  |  ",firstWheel[i], secondWheel[i], thirdWheel[i]);
        if(firstWheel[i] == secondWheel[i] && secondWheel[i] == thirdWheel[i]){
          //printf("Congratulations! you won in the line %d\n", i);
          printf("*<*<*<*<*<*<*<*<*<*<*<*<*<*<*<*<*<*<*<*<*<*<*<*\n");
          win = i;
          credit+=(bet*1.5);
        }
        else{
          printf("\n");
        }
      }
      //unlocking all wheel threads
    }
    if(win > 0){
      printf("Congratulations you won in line %d\n press enter to play again 'q' to quit\n", win);
    }
    else{
      printf("Sorry.. press enter to try again! 'q' to quit\n");
    }
    while (1) {
      scanf("%c", &input);
      if(input == '\n'){
        win = 0;
        printf("     Spinning wheels\n");
        printf("Your credit is %d \n      How much do you want to bet?\n", credit);
        scanf("%d", &bet);
        printf("Placing bet\n");
        credit-=bet;
        pthread_mutex_unlock(&mutex);
        break;
      }
      else{
        printf("See you next time!\n");
        endgame = 1;
        pthread_mutex_unlock(&mutex);
        return NULL;
      }
    }
  }
return NULL;
}
