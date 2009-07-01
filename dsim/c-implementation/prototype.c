/*
 * C prototype for dSim
 *
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "mpi.h"

#include "proxy.h"
#include "slave.h"


int main (int argc, char **argv)
{

  int rank, size ; 
  int rc ; 

  MPI_Init(&argc, &argv); 
  MPI_Comm_rank(MPI_COMM_WORLD, &rank) ; 
  MPI_Comm_size(MPI_COMM_WORLD, &size) ; 
 
 
      
  if (!rank) 
    {
      printf("hi dev/ori, I'm the master process out of %d\n", size) ;
      rc = pending_master(size) ;
    }
  else
    {
      printf("hi dev/ori, I'm the slave process %d out of %d\n", rank, size) ;
      rc = pending_slave () ;
    }
  
  
  MPI_Finalize(); 
  return 0; 
}
