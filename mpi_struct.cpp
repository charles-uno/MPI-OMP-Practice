//. In this routine, I create an MPI type which allows me to send a user-defined struct between processes. Something like this will be necessary in any routine where you want to send data other than INT, DOUBLE, etc. 

//. Charles McEachern, Fall 2014

#include "mpi.h"
#include <stdio.h>

const int N(10);

//. first we define some random struct. It is full of stuff.
struct test_struct
{
  int sum;
  double arr[N];
};

int main(){
//. you are supposed to do this before anything else, I think
  MPI_Init(NULL,NULL);

//. declare a member of our test struct
  test_struct t;

//. standard mpi stuff -- what process am I currently in, and how many are there total
  int mpi_rank, mpi_size;

//. these arrays are needed for the custom mpi type. their size is the number of things in the struct
  MPI_Aint disp[2]; //. displacement from the struct address
  MPI_Datatype type[2]; //. mpi types of the struct components
  int elts[2]; //. how many of each type

  MPI_Status s; //. standard mpi thing

  MPI_Comm_rank(MPI_COMM_WORLD,&mpi_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&mpi_size);

//. figure out the displacements within the test struct. The typecasting is an ad-hoc solution, but it seems to work.
  disp[0] = (int*)&t.sum - (int*)&t;
  disp[1] = (int*)&t.arr - (int*)&t;

//. what is in the struct?
  type[0] = MPI_INT;
  type[1] = MPI_DOUBLE;

//. how much of that stuff is in there?
  elts[0] = 1;
  elts[1] = N;

//. what do we want to call this type?
  MPI_Datatype mpi_test_struct;

//. This is the moneymaker. For some reason it takes two commands -- one to make the type and one to commit it. I'm not sure why it's split.
  MPI_Type_create_struct(2,elts,disp,type,&mpi_test_struct);
  MPI_Type_commit(&mpi_test_struct);

//. Let's initialize t differently on the master node...
  if(mpi_rank == 0){
    t.sum = 0;
    for(int i=0;i<N;i++){
      t.arr[i] = i+1;
      t.sum += t.arr[i];
    }
  } else {
    t.sum = 0;
    for(int i=0;i<N;i++){
      t.arr[i] = 0;
    }
  }

//. then send it to the rest of the nodes and see if it gets there correctly.
  for(int i=1;i<mpi_size;i++){
    if(mpi_rank == 0){
      MPI_Send(&t,1,mpi_test_struct,i,123+i,MPI_COMM_WORLD);
       printf("just send to thread %i sum = %d\n",i,t.sum);
    }
    if(mpi_rank == i){
      MPI_Recv(&t,1,mpi_test_struct,0,123+i,MPI_COMM_WORLD,&s);
      printf("thread %i just got sum = %d\n",i,t.sum);
    }
  }

  MPI_Finalize();
  return 0;
}
