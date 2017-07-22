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

  int worldSize = 0;
  MPI_Comm_size( MPI_COMM_WORLD, &worldSize);

  int n = 1 + myRank;
  
  int* myArray = new int [n];
  for( int i = 0; i < n; i++)
  {
    myArray[i] = myRank;
  }

  // Determine the global number of array elements
  int N = 0;
  MPI_Allreduce( &n, &N, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  // Determine the size of each sub-buffer
  int* buffSpacer = new int [worldSize];
  for( int i = 0; i < worldSize; i++)
  {
    buffSpacer[i] = 0;
  }
  MPI_Allgather( &n, 1, MPI_INT, buffSpacer, 1, MPI_INT, MPI_COMM_WORLD);
  if( myRank == 0 )
  {
    print_array( buffSpacer, worldSize);
  }

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

  // Since we want the data to be tightly packed 
  // (no empty spaces between one sub array and 
  // the next) we set the displacement array to 
  // be equivalent to the receive-count array
  int* displ = new int [worldSize];
  for( size_t i = 0; i < worldSize; i++)
  {
    if ( i == 0)
    { 
      displ[i] = 0;
    }
    else
    {
      displ[i] = displ[i-1] + buffSpacer[i] - 1;
    }
  }

  // Finally, collect all data into ourArray
  int* ourArray = new int [N];
  MPI_Allgatherv( myArray, n, MPI_INT, ourArray, buffSpacer, displ, 
      MPI_INT, MPI_COMM_WORLD);
/*
  MPI_Allgatherv(
      void* send_data,
      int send_count,
      MPI_Datatype send_type,
      void* recvbuf,
      int* recvcounts,
      int* displs,
      MPI_Datatype recv_type,
      MPI_Comm comm)
*/
  if( myRank == 0)
  {
    print_array( ourArray, (size_t) N);
  }
  
  delete myArray;
  delete buffSpacer;
  delete displ;
  delete ourArray;

  MPI_Finalize();

  return 0;
}
