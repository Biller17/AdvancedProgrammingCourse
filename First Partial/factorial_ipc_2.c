/*
    Program to compute the factorial of different numbers
    The program will create another process, that will do the calculations
    The parent process will hear requests by the user
    Uses the 'pipe' function for communication
    Uses the functions 'read' and 'write' to work directly with the file descriptors (int)

    Gilberto Echeverria
    gilecheverria@yahoo.com
    12/02/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 20

///// FUNCTION DECLARATIONS /////
void createProcess();
void userMenu(int pipe_out[], int pipe_in[]);
void attendRequests(int pipe_out[], int pipe_in[]);
void openPipe(int pipe_channel[]);
void preparePipes(int pipe_out[], int pipe_in[]);
void closePipes(int pipe_out[], int pipe_in[]);
unsigned long int factorial(int number);

int main()
{
    printf("\nPROGRAM TO COMPUTE FACTORIALS\n");
    printf("Only one child process to find the factorials\n");

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
    // Variables for the file pointers
    //FILE * fp_in = NULL;
    //FILE * fp_out = NULL;

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
        // Start the loop to hear user requests and send them to child
        userMenu(pipe_parent_child, pipe_child_parent);
        // Close the pipes
        closePipes(pipe_parent_child, pipe_child_parent);
        printf("Parent finishing\n");
    }
    // Child process
    else if (pid == 0)
    {
        // Open the corresponding pipes
        preparePipes(pipe_child_parent, pipe_parent_child);
        // Start the loop to hear user requests and get the factorial
        attendRequests(pipe_child_parent, pipe_parent_child);
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

// Main loop for the parent process, listens for the user requests
// Receive: the arrays with the file descriptors for input and output
void userMenu(int pipe_out[], int pipe_in[])
{
    int number = 0;
    unsigned long int result = 0;
    char buffer[BUFFER_SIZE];

    while (number >= 0)
    {
        printf("\nEnter a number to compute its factorial (enter a negative number to finish): ");
        scanf("%d", &number);
        // Send the number to the child
        sprintf(buffer, "%d", number);
        // Write all the characters in the buffer, and also the null character at the end
        write(pipe_out[1], buffer, strlen(buffer) + 1);

        // Compute factorials for positive numbers and 0 only
        if (number >= 0)
        {
            // Get the reply from the child
            read(pipe_in[0], buffer, BUFFER_SIZE);
            sscanf(buffer, "%lu", &result);
            printf("The result sent by child is %lu\n", result);
        }
    }
}

// Loop for the child process to wait for requests from the parent
// Receive: the arrays with the file descriptors for input and output
void attendRequests(int pipe_out[], int pipe_in[])
{
    int number = 0;
    unsigned long int result = 0;
    char buffer[BUFFER_SIZE];

    while (1)
    {
        // Listen for requests from the parent
        read(pipe_in[0], buffer, BUFFER_SIZE);
        sscanf(buffer, "%d", &number);
        printf("Child got: %d\n", number);
        // Finish with a negative number
        if (number < 0)
        {
            break;
        }
        result = factorial(number);
        // Send the result back to the parent
        sprintf(buffer, "%lu", result);
        // Write all the characters in the buffer, and also the null character at the end
        write(pipe_out[1], buffer, strlen(buffer) + 1);
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

// Compute the factorial of a number
unsigned long int factorial(int number)
{
    unsigned long int result = 1;
    for (; number > 0; number--)
    {
        result *= number;
    }
    return result;
}
