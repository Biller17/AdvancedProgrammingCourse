/*
	Name: Adrian Biller
	ID number: A01018940

    TC2025 Advanced Programming
    First Partial Evaluation
    Evaluates:
        - Pointers
        - Dynamic memory
        - Process management
        - Inter Process Communication

    Program to test concentration counting exercise

    The problem:
    There is an exercise to develop concentration, where two numbers are chosen,
    the first will count up from 0, and the second counts down from 100.
    The player must keep counting in his head, adding and substracting each time
    from both sides. Example, in the range 0-100, adding 3 and substracting 4:
     3 - 96
     6 - 92
     9 - 88
    12 - 84
    15 - 80
    etc.

    You must write a program that can do this calculations, but the increment
    must be done in one process and the decrement in a different process.
    The parent will be the one who decides when to stop, so the two processes
    must keep constant communication. The child will send the updated value
    to the parent, and the parent will reply with a boolean value indicating
    whether the child should continue or it has finished already.
    The program will ask the user for the size of the range, and the amounts
    to add and substract

    You must show visually the progress of these counts, by using a dynamically
    sized string, initially filled with spaces. As the counts progress, the
    string will be filled with the '\' character from left to right, and with
    the '/' character from right to left. If the two counts intersect, you
    should write the 'X' character instead.
    Example, in the range 0-20, adding 4 and substracting 2

		   4 -   18	\\\\              //
		   8 -   16	\\\\\\\\        ////
		  12 -   14	\\\\\\\\\\\\  //////
		  16 -   12	\\\\\\\\\\\\XXXX////
		  20 -   10	\\\\\\\\\\XXXXXXXXXX
		  20 -    8	\\\\\\\\XXXXXXXXXXXX
		  20 -    6	\\\\\\XXXXXXXXXXXXXX
		  20 -    4	\\\\XXXXXXXXXXXXXXXX
		  20 -    2	\\XXXXXXXXXXXXXXXXXX
		  20 -    0	XXXXXXXXXXXXXXXXXXXX

	You must use pointer arithmetic to modify the string. Index notation is not
	allowed for this part of the test.

    QUESTIONS:
    Complete the following three functions to make the program work:
        - countUp
        - countDown
        - drawLine
    The description of what each does is at the end of this file.
    Write your functions there

    HINTS:
        - Write your name in this comment and in the "main" function
        - Read all of the existing code before answering anything. This will help you
            understand what needs to be done and how to do it
        - Keep calm. Think what you have to do before typing code
        - Write clean code and use correct indentation
        - Add comments to your code
        - You can add any other functions that you need to make your code more organized
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 20

///// FUNCTION DECLARATIONS /////
void createProcess();
void openPipe(int pipe_channel[]);
void preparePipes(int pipe_out[], int pipe_in[]);
void closePipes(int pipe_out[], int pipe_in[]);
void countUp(int pipe_out[], int pipe_in[]);
void countDown(int pipe_out[], int pipe_in[]);
void drawLine(char * line, int range, int count_up, int count_down);

int main()
{
    printf("\n=== PROGRAM FOR CONCENTRATION EXERCISES ===\n");
    printf("By: {Adrian Biller Alcantara}\n\n");

    createProcess();

    return 0;
}

///// FUNCTION DEFINITIONS /////

// Function to create the child process and declare the pipe variables
void createProcess()
{
    pid_t pid;
    // Variables for the pipes, indicating the direction of communication
    int pipe_parent_child[2];
    int pipe_child_parent[2];

    // Open the two channels
    openPipe(pipe_parent_child);
    openPipe(pipe_child_parent);

    // Create only one fork, the child will do the calculations
    pid = fork();

    // Parent process
    if (pid > 0)
    {
        // Open the corresponding pipes
        preparePipes(pipe_parent_child, pipe_child_parent);
        // Start the loop to count upwards from 0
        countUp(pipe_parent_child, pipe_child_parent);
        // Close the pipes
        closePipes(pipe_parent_child, pipe_child_parent);
        printf("Parent finishing\n");
    }
    // Child process
    else if (pid == 0)
    {
        // Open the corresponding pipes
        preparePipes(pipe_child_parent, pipe_parent_child);
        // Start the loop to count downwards from the range selected by the user
        countDown(pipe_child_parent, pipe_parent_child);
        // Close the pipes
        closePipes(pipe_child_parent, pipe_parent_child);
        printf("Child finishing\n");
    }
    // No process created
    else
    {
        perror("Unable to create a new process\n");
        exit(EXIT_FAILURE);
    }
}

// Verify the opening of a commuication pipe
void openPipe(int pipe_channel[])
{
    if (pipe(pipe_channel) == -1)
    {
        perror("Unable to create pipe\n");
        exit(EXIT_FAILURE);
    }
}

// Close the pipe directions that are not necessary
// Receive the file descriptors
void preparePipes(int pipe_out[], int pipe_in[])
{
    // Close the unwanted channels
    close(pipe_in[1]);
    close(pipe_out[0]);
}

// Close the remaining pipes
// Receive the file descriptors
void closePipes(int pipe_out[], int pipe_in[])
{
    // Close the file descriptors
    close(pipe_in[0]);
    close(pipe_out[1]);
}

// Main loop for the parent process, does the addition.
// It asks the user for the necessary inputs (range, increment, decrement)
// It passes initiates its variables and passes the variables range and
//  decrement to the child
// Then begins the main loop, where the two processes communicate the
//  updated values
// Receive: the arrays with the file descriptors for input and output
// Return: nothing
void countUp(int pipe_out[], int pipe_in[])
{

  int actualIncreasingNumber = 0;
  int actualDecreasingNumber;
  int range;
  int increment;
  int decrement;
  int childFinished = 0;
  char buffer[BUFFER_SIZE];


//writing range and passing to child
  printf("\nPlease write the range: ");
  scanf("%d", &range);
  sprintf(buffer, "%d", range);
  write(pipe_out[1], buffer, strlen(buffer) + 1);
//writing increment rate
  printf("\nPlease write the increment rate: ");
  scanf("%d", &increment);
//writing decrement rate and sending to child
  printf("\nPlease write the decrement rate: ");
  scanf("%d", &decrement);
  sprintf(buffer, "%d", decrement);
  write(pipe_out[1], buffer, strlen(buffer) + 1);

  printf("Range:%d  increment: %d   decrement: %d\n",range,increment, decrement);



  while(actualIncreasingNumber < range || childFinished == 0)
  {
    //check child number
    read(pipe_in[0], buffer, BUFFER_SIZE);
    sscanf(buffer, "%d", &actualDecreasingNumber);
    if(actualDecreasingNumber == 0){
      childFinished = 1;
    }

    //tell child to stop or not
    sprintf(buffer, "%d", childFinished);
    write(pipe_out[1], buffer, strlen(buffer) + 1);
    //increments own number
    if(actualIncreasingNumber < range){
      actualIncreasingNumber+=increment;
    }
    //check if any error in the increasing might happen and prevent it
    if(actualIncreasingNumber+increment > range){
      actualIncreasingNumber = range;
    }
    //create buffer for drawline string
    char* drawBuffer = (char*)malloc(range *sizeof(char));

    printf("%d - %d\t", actualIncreasingNumber, actualDecreasingNumber);
    //call to draw line
    drawLine(drawBuffer, range, actualIncreasingNumber, actualDecreasingNumber);
  }
}

// Loop for the child process to update the decrement
// It gets from the parent the values for the range and the decrement
// It then loops, updating the count and listening for a reply from
//  the parent, indicating whether it has finished or not.
// Receive: the arrays with the file descriptors for input and output
// Return: nothing
void countDown(int pipe_out[], int pipe_in[])
{
  int continuing = 1;
  int range;
  int actualDecreasingNumber;
  int decrement;
  int finished = 0;
  char buffer[BUFFER_SIZE];
  //receive range from parent
  read(pipe_in[0], buffer, BUFFER_SIZE);
  sscanf(buffer, "%d", &range);
  printf("Child received range: %d\n",range );
  actualDecreasingNumber = range;
//receive decrement rate from parent
  read(pipe_in[0], buffer, BUFFER_SIZE);
  sscanf(buffer, "%d", &decrement);
  printf("Child received decrement: %d\n", decrement);
  actualDecreasingNumber-=decrement;

    while(continuing)
    {
      //tell parent new number
      sprintf(buffer, "%d", actualDecreasingNumber);
      write(pipe_out[1], buffer, strlen(buffer) + 1);


      //read if parent wants to stop
      read(pipe_in[0], buffer, BUFFER_SIZE);
      sscanf(buffer, "%d", &finished);

      //checking to decrement or to change it to zero to prevent errors
      if(actualDecreasingNumber > 0){
        actualDecreasingNumber-=decrement;
      }
      if(actualDecreasingNumber-decrement < 0){
        actualDecreasingNumber = 0;

      }
    }
}

// Draw the line showing the progress of the counts
// Called at every loop
// Will modify the string, adding '\' characters from the left
//  and '/' characters fromt he right. 'X' characters are used
//  when the counts intersect.
// Use only pointer arithmetic to modify the string
// Receives: the string, the size, and the counters for left and right
// Return: nothing
void drawLine(char * line, int range, int count_up, int count_down)
{
  int blank = count_down - count_up;
  int up = count_up;
  int down = count_down;
//when the lines dont cross
  if(up-down <0){
    for(int i=0; i<up; i++){
      *(line+i) = '\\';
    }
    for(int i=0; i<blank; i++){
      *(line+up+i) = ' ';
    }
    for(int i=0; i<range-down; i++){
      int temp = up+blank;
      *(line+temp+i) = '/';
    }
  }
  //when the lines do cross
  else{
    for(int i = 0; i < down; i++){
      *(line+i) = '\\';
    }
    for(int i = 0; i<up-down; i++){
      *(line+down+i) = 'X';
    }
    for(int i = 0; i<range-up; i++){
      *(line+up+i) = '/';
    }
  }
  printf("%s \n", line );

}
