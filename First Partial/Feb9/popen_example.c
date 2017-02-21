/*
    Open pipes into commands using the popen functions
*/
#include <stdio.h>

#define BUFFER_SIZE 255

void pipe_read();
void pipe_write();

int main()
{
    pipe_read();
    pipe_write();
    return 0;
}

void pipe_read()
{
    FILE * file_ptr = NULL;
    char * command = "./pids";
    char buffer[BUFFER_SIZE];

    // Open the pipe
    file_ptr = popen(command, "r");
    // Validate that the pipe could be opened
    if (file_ptr != NULL)
    {
        while ( fgets(buffer, BUFFER_SIZE, file_ptr) )
        {
            printf("\t%s", buffer);
        }

        // Close the pipe
        pclose(file_ptr);
    }
}

void pipe_write()
{
    FILE * file_ptr = NULL;
    char * command = "grep 5";
    int i;

    // Open the pipe
    file_ptr = popen(command, "w");
    // Validate that the pipe could be opened
    if (file_ptr != NULL)
    {
        for (i=0; i<500; i+=30)
        {
            fprintf(file_ptr, "%d\n", i);
        }

        // Close the pipe
        pclose(file_ptr);
    }
}
