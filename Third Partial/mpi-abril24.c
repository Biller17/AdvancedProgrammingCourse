#include <stdio.h>
#include <mpi.h>




int main(int argc, char* argv[]){

  int processors;
  int rank;
  //start mpi environment
  MPI_Init(&argc, &argv);
  //get info about the processes
  MPI_Comm_size(MPI_COMM_WORLD, &processors);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if(rank == 0 ){ //only the first thread can do this
    printf("Using %d processors\n", processors);
  }
  printf("hello world from process %d\n", rank);
  MPI_Finalize();

  return 0;
}
