//. Charles McEachern, Fall 2014

//. This routine makes an array full of numbers, then fires those numbers off to each different process. 


#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main() {

  const int N(10);
  int arr[N];
  int buff(0);
  MPI_Status status;

// Initialize the MPI environment
  MPI_Init(NULL, NULL);
// Find out rank, size
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

 if(world_rank == 0){
//. if this is the master thread
//. printf is used instead of cout because printf sends a single string to output, while cout sends a series of individual outputs. If output is coming from multiple places at once, cout commands can interlace with one another, while printf commands never 
will. At least, this is the case for omp.
//. Also, printf lets you do fortran-style formatting of the numbers that get printed out, which is potentially convenient.
    printf("master thread assigning numbers: \n");
    for(int i=0;i<N;i++){
      arr[i] = i+1;
      printf("%i\n",arr[i]);
    }
  }

  for(int i=0;i<N;i++){
    for(int j=1;j<world_size;j++){

      if(world_rank == 0){
        MPI_Send(&arr[i], 1, MPI_INT, j, 123, MPI_COMM_WORLD);
      }

      if(world_rank == j){
        MPI_Recv(&buff, 1, MPI_INT, 0, 123, MPI_COMM_WORLD, &status);
        printf("thread %i just received %i.\n",j,buff);
      }
    }
  }
  MPI_Finalize();
}
