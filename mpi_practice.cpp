//Written for: practice using parallel processing. Will:
// 1. Output 'hello world' from only Rank Zero (line 26)
// 2. Prinout Current Rank (line 32)
// 3. Sum all ranks over all processes (line 36)
//Written by: Justin Clough
//Written on: 10/19/2016


#include <iostream>
#include <mpi.h>


int main(int argc, char** argv)
{

	//Initialize the MPI Enviroment
	MPI_Init(NULL, NULL);

	//Get number of processes
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	//Get rank of the process
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	//Do task 1: Print off 'Hello World' from rank zero
	if (world_rank == 0 ) //If this process is rank zero, then...
		{
		std::cout << "Hello World From Process with Rank Number " << world_rank << std::endl ;
		}

	//Do task 2: Print out current rank
	std::cout << "Current Rank Number is: " << world_rank << std::endl;

	//Do task 3: Sum ranks over all processes
	//Gather rank information 
	int summed_ranks;
	MPI_Reduce(&world_rank, &summed_ranks, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
		
	//Close MPI Enviroment
	MPI_Finalize();

	if ( world_rank == 0 )
	{
		std::cout << "The sum ranks are: " << summed_ranks << std::endl; 
	}

	return 0;


}


/* Notes: Must be compiled on terminal with "mpicxx 'filename.ext' -o executable_name'. Must be run on terminal with "mpirun -n '#ofProcessors' ./executable_name */ 
