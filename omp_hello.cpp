//. Charles mcEachern, Fall 2014

//. Just getting OpenMp up and running -- this should be pretty straightforward

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main (){
  int rank, size;

//. private() means that OpenMP will fork that variable, giving each thread its own copy.
  #pragma omp parallel private(rank)
  {
//. How many threads are there?
    size = omp_get_num_threads();
//. which thread am I currently in?
    rank = omp_get_thread_num();
//. Hello!
    printf("Hello World from thread %d of %d\n",rank,size);
  } //. all threads combine here

//. We can also use OpenMP to do a FOR loop faster...
  #pragma omp parallel for
  for(int i=0;i<10;i++){
    printf("Hello %d\n",i);
  }
//. Note that the iterations will execute in arbitrary order. Don't put things in here that need to happen in any particular order.

  return 0;
}
