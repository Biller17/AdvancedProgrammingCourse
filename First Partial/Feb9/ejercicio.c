#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // Library to access POSIX functions
#include <sys/wait.h>

#define BUFFER_SIZE 255


void read_pipe(int file_descriptor[]){
  FILE * file_ptr = NULL;
  char buffer[BUFFER_SIZE];
  close(file_descriptor[1]);

  file_ptr = fdopen(file_descriptor[0], "r");

  if (file_ptr ==NULL)
    EXIT(EXIT_FAILURE);

  while (fgets(buffer, BUFFER_SIZE, file_ptr)) {
    
  }
}
int main()
{

  return 0;

}
