//. Charles McEachern, Fall 2014

//. MPI Hello World.
//. This is basically the simplest MPI routine possible. It figures out how many processes there are, then has each of thos eprocesses introduce itself.
//. Note that printf is used, not cout, since multiple processes using cout at the same time can become jumbled. cout sends multiple short strings to output. printf, on the other hand, sends just one string to output, so there are no bits to get interlaced.
//
#include <mpi.h>
#include <stdio.h>
main(){
  MPI_Init(NULL,NULL);
  int node;
  MPI_Comm_rank(MPI_COMM_WORLD, &node);
  printf("Hello World from Node %d\n",node);
  MPI_Finalize();
}
