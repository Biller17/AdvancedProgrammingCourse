/*
    Program to show the use of pipe to open a pipe between processes
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // Library to access POSIX functions
#include <sys/wait.h>   // Library for the wait functions

#define BUFFER_SIZE 255

void read_pipe(int file_descriptor[]);
void write_pipe(int file_descriptor[]);

int main()
{
    printf("CALL TO FORK\n");
    pid_t new_id;
    int file_descriptor[2];

    if (pipe(file_descriptor) == -1)
    {
        perror("Could not open the pipe\n");
        exit(EXIT_FAILURE);
    }

    // Create a new process
    new_id = fork();

    printf("My pid: [%d], my parent id: [%d], new_id: [%d]\n", getpid(), getppid(), new_id);

    // The child process
    if (new_id == 0)
    {
        write_pipe(file_descriptor);
    }
    // The parent process
    else if (new_id > 0)
    {
        read_pipe(file_descriptor);
    }
    // If the new_id is negative
    else
    {
        printf("COULD NOT CREATE A NEW PROCESS\n");
    }

    printf("BACK TO THE MAIN PROGRAM\n");

    return 0;
}

void read_pipe(int file_descriptor[])
{
    FILE * file_ptr = NULL;
    char buffer[BUFFER_SIZE];

    // Close the pipe for writing
    close(file_descriptor[1]);

    file_ptr = fdopen(file_descriptor[0], "r");
    if (file_ptr == NULL)
        exit(EXIT_FAILURE);

    while ( fgets(buffer, BUFFER_SIZE, file_ptr) )
    {
        printf("Read from file: %s", buffer);
    }

    // Close the open pipe
    fclose(file_ptr);
    close(file_descriptor[0]);
}

void write_pipe(int file_descriptor[])
{
    FILE * file_ptr = NULL;
    int i;

    // Close the pipe for writing
    close(file_descriptor[0]);

    file_ptr = fdopen(file_descriptor[1], "w");
    if (file_ptr == NULL)
        exit(EXIT_FAILURE);

    for (i=0; i<100; i+=10)
    {
        fprintf(file_ptr, "Writing to pipe: %d\n", i);
    }

    // Close the open pipe
    fclose(file_ptr);
    close(file_descriptor[1]);
}
