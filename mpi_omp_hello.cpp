//. Charles McEachern, Fall 2014

//. This is an attempt to use MPI and OpenMP together. 
//. Note that MPI creates processes which do not share memory with one another.
//. On the other hand, OpenMP runs multiple processes which DO share memory.
//. Since each machine on Itasca has 8 cores, the ideal way to run a big job is to have the 8 cores of each machine share memory with one another -- this avoids wasted time spent copying data between nodes that share the same physical memory space.
//. This may be easier said than done -- we'll see!

#include <mpi.h>
#include <stdio.h>
#include <omp.h>

main(){
//. MPI_init is supposed to be the first thing  you do!
  MPI_Init(NULL,NULL);

//. these are just for keeping track of where we are
  int mpi_rank, mpi_size, omp_rank, omp_size;

//. figure out our MPI location
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

  #pragma omp parallel private(omp_rank)
  {
//. now we figure out where we are in OpenMP land
    omp_rank = omp_get_thread_num();
    omp_size = omp_get_num_threads();

//. this should only happen once, on the master thread on the master node (though it may well not happen first)
    if( omp_rank == 0 && mpi_rank == 0 ){
      printf("rank then world size\n");
    }

//. report the situation to output
    printf("MPI: %d, %d; OMP: %d, %d\n",mpi_rank, mpi_size,omp_rank,omp_size);
  }

  MPI_Finalize();

  return 0;
}
