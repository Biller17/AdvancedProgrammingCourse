#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // Library to access POSIX functions
#include <sys/wait.h>

#define BUFFER_SIZE 255

void read_pipe(int file_descriptor[]){
  FILE* file_ptr = NULL;
  char buffer [BUFFER_SIZE];

  close(file_descriptor[1]);

  file_ptr = fdopen(file_descriptor[1], "r");
  if(file_ptr == NULL){
    printf("There was an error in opening the reading channel\n");
    exit(0);
  }

  while (fscanf(file_ptr, )) {
    /* code */
  }


}


void write_pipe(){

}

int main(){
  return 0;
}
