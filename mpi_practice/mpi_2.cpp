#include <stdlib.h>
#include <iostream>
#include <mpi.h>

void print_array( int* array, size_t size)
{
  std::cout << "Array = " ;
  for( int i = 0; i < size; i++)
  {
    std::cout << array[i] << ", " ;
  }
  std::cout << std::endl;
  return;
}

int main( int argc, char** argv)
{

  MPI_Init( &argc, &argv);

  int myRank = 0;
  MPI_Comm_rank( MPI_COMM_WORLD, &myRank);

  std::cout << "My rank is : " << myRank << std::endl;

  int worldSize = 0;
  MPI_Comm_size( MPI_COMM_WORLD, &worldSize);
  
  int array[ worldSize] = {0};

  MPI_Allgather( &myRank, 1, MPI_INT, &array, 1, MPI_INT, MPI_COMM_WORLD);
/*
  MPI_Allgather( 
            void* send_data,
            int   send_count,
            MPI_Datatype send_datatype,
            void* recv_data,
            int   recv_count,
            MPI_Datatype recv_datatype,
            MPI_Comm Communicator)
*/

  if( myRank == 0)
  {
    print_array( array, (size_t) worldSize);
  }

  MPI_Finalize();

  return 0;
}
