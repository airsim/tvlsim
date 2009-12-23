/*
 * C prototype for dSim - proxy
 *
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "mpi.h"

#ifndef min
	#define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif



int run_proxy (const char *path, int size, int nb_flights, int nb_classes, int requests_per_day, int duration)
{
  FILE *pipe; 
  
  // Dispatch the flights among MPI clients
  
  int * balancer = (int *) malloc (nb_flights * nb_classes * sizeof(int) * 2); 
  int * translator = (int *) malloc (nb_flights * nb_classes * sizeof(int) * 2); 
    
  int counters[size - 1]; 

  int it ; 

  for (it = 1; it < size; it++)
    {
      counters[it - 1] = 0; 
    }
  
  int guillotine_id = 0 ; 
  int agent = 1 ;
  
  int flight_number ; 
  int class_code ; 
  
  for (flight_number = 0; flight_number < nb_flights; flight_number++)
    {
      int class_code ; 
      for (class_code = 0; class_code < nb_classes; class_code++)
	{
	  balancer[flight_number * nb_classes + class_code] = agent ; 
	  translator[flight_number * nb_classes + class_code] = counters[agent - 1]; 
	  counters[agent - 1]++;
	}
      agent++ ; 	  	  
      if (agent == size) agent = 1; 
    }
  
  for (agent = 1; agent < size; agent++)
    {
      int capsule[1]; 
      capsule[0] = counters[agent - 1]; 
      MPI_Send(capsule, 1, MPI_INT, agent, 0, MPI_COMM_WORLD) ;       
    }

  MPI_Barrier( MPI_COMM_WORLD ) ;

  for (flight_number = 0; flight_number < nb_flights; flight_number++)
    {
      int flight[2]; 
      flight[0] = flight_number ; 
      flight[1] = nb_classes; 
  
      MPI_Send(flight, 2, MPI_INT, balancer[flight_number * nb_classes], 0, MPI_COMM_WORLD) ;  
     
    }

  // Wait for all the agents 
  MPI_Barrier( MPI_COMM_WORLD ) ;

  // Open the pipe 

  
  pipe = fopen(path, "r+"); 

  if (pipe == NULL) {printf("Couldn't open pipe (check /tmp/simpipe please)\n"); return -1; } 

  while (1) 
    {
      int rc ;
      int sale_request[2]; 
      int guillotine_id; 
      rc = fscanf (pipe, "%d %d", &guillotine_id, &(sale_request[1]));
      printf ("-- Got a message: %d %d\n", guillotine_id, sale_request[1]); 
      if (rc == EOF) { printf("EOF\n"); MPI_Barrier( MPI_COMM_WORLD ) ; return 0;  }
      if (rc != 2) { printf("Read %d elts\n", rc); return -1 ; }
      
      if (guillotine_id == -2) 
	{
	  for (agent = 1; agent < size; agent++)
	    {
	      sale_request[0] = -2; 
	      MPI_Send(sale_request, 2, MPI_INT, agent, 0, MPI_COMM_WORLD) ;       
	    }
	  return 0;
	} else if (guillotine_id == -1) 
	  {
	    for (agent = 1; agent < size; agent++)
	    {
	      sale_request[0] = -1 ;
	      MPI_Send(sale_request, 2, MPI_INT, agent, 0, MPI_COMM_WORLD) ;       
	    }
	  MPI_Barrier( MPI_COMM_WORLD ) ;
	}
      else 
	{
	  sale_request[0] = translator[guillotine_id]; 	  
	  MPI_Send(sale_request, 2, MPI_INT, balancer[guillotine_id], 0, MPI_COMM_WORLD) ;   
	}
      
     
    }
    
}
