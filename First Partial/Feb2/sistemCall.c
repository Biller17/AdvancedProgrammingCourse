#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //library to access POSIX functinos


int main(){

//creating a process
printf("call to fork");
pid_t new_id;

//create new process
new_id = fork();

printf("My pid: [%d], my parent id: [%d], new_id: [%d]\n",getpid(), getppid(), new_id );

//the child process
if(new_id == 0){
  printf("I am the child process\n");
  execl("/bin/ls", "ls", (char*) NULL);
}
//the parent process
else if(new_id > 0){
    printf("I am the parent of %d\n", new_id );
}

else{
  printf("The process could not be created :(\n");
}



  //system call
  printf("call to a system program\n");
  system("/bin/ls ");
  printf("Back to my program\n");

  return 0;
}
