//
// Simple test of MPI, just to check that a MPI library is installed correctly
//Source: http://www.boost.org/doc/libs/1_41_0/doc/html/mpi/getting_started.html
#include <mpi.h>
#include <iostream>

// ///////////////////// M A I N //////////////////////
int main(int argc, char* argv[]) {
  MPI_Init (&argc, &argv);

  int rank = -1;

  MPI_Comm_rank (MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    int value = 17;
    int result = MPI_Send(&value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

    if (result == MPI_SUCCESS) {
      std::cout << "Rank 0 OK!" << std::endl;
    }

  } else if (rank == 1) {
    int value;
    int result = MPI_Recv(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
			  MPI_STATUS_IGNORE);
    
    if (result == MPI_SUCCESS && value == 17)
      std::cout << "Rank 1 OK!" << std::endl;
  }
  
  MPI_Finalize();
  
  return 0;
} 
