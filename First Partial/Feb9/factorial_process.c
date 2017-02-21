#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // Library to access POSIX functions
#include <sys/wait.h>   // Library for the wait functions

void loop();
int factorial(int num);

int main()
{
    printf("PROGRAM TO GET FACTORIALS\n");
    loop();
    return 0;
}

void loop()
{
    int num = 0;

    while (num >= 0)
    {
        printf("Enter a number (negative number to exit): ");
        scanf("%d", &num);

        pid_t new_id;

        // Create a new process
        new_id = fork();

        // The child process
        if (new_id == 0)
        {
            // Transform the child into another program
            exit( factorial(num) );
        }
        // The parent process
        else if (new_id > 0)
        {
            int status;
            waitpid(new_id, &status, 0);
            // Print a message if the child exited normally
            if (WIFEXITED(status))
            {
                printf("Child finished with status: %d\n", WEXITSTATUS(status));
            }
        }
        // If the new_id is negative
        else
        {
            printf("COULD NOT CREATE A NEW PROCESS\n");
        }
    }
}

int factorial(int num)
{
    int result = 1;
    for (; num>1; num--)
    {
        result *= num;
    }
    return result;
}
