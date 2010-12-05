/*
 * C prototype for dSim - slave stuff
 *
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "mpi.h"

#include <mysql/mysql.h>


int init_guillotines (int * gduration, 
		      int * to_bi,
		      int * nb_guillotines, 
		      int ** translator, 
		      unsigned short ** guillotines, 
		      unsigned short ** forecasts)
{     
  MPI_Status status;
  int capsule[3]; 
  MPI_Recv(capsule, 3, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  
  *nb_guillotines = capsule[0]; 
  *gduration = capsule[1]; 
  *to_bi = capsule[2];
  
  printf("Guillotine data received %d %d %d\n", capsule[0], capsule[1], capsule[2]);
  int duration = capsule[1]; 

  int size_guillotines = capsule[0] * 365 * duration ; 

  *guillotines = (unsigned short *) malloc (size_guillotines * sizeof(short));
  *forecasts   = (unsigned short *) malloc (size_guillotines * sizeof(short));

  *translator  = (int *) malloc ((*nb_guillotines) * 5 * sizeof(int)) ;
  
  if (*guillotines == NULL || *forecasts == NULL || *translator == NULL) return -1;  // malloc failed
  
  MPI_Barrier( MPI_COMM_WORLD ) ;
 
  int guillotine_id = 0; 

  while (guillotine_id < *nb_guillotines)
    {
      int flight[5]; 
      MPI_Status status;
      MPI_Recv(flight, 5, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
      printf("Translation data recieved: %d %d %d %d %d\n", flight[0], flight[1], flight[2], flight[3], flight[4]);  
      int class_code; 
      for (class_code = 0; class_code < flight[3]; class_code++)
	{
	  (*translator)[4 * (flight[4] + class_code)    ] = flight[0] ; // Airline code
	  (*translator)[4 * (flight[4] + class_code) + 1] = flight[1] ; 
	  
	  (*translator)[4 * (flight[4] + class_code) + 2] = flight[2] ; // Flight number
	  (*translator)[4 * (flight[4] + class_code) + 3] = class_code ; // Class_code
	  
	  guillotine_id = guillotine_id++; 
	}
    }

  int it; 
  for (it = 0; it < size_guillotines; it++)
    {
      (*guillotines)[it] = 0 ;
      (*forecasts)[it] = 0 ;
    }

  printf ("Memory allocated: %d bytes\n", size_guillotines * 2 * sizeof(short)); 
  MPI_Barrier( MPI_COMM_WORLD ) ;
  return 0; 
}

int init_dbconnection (MYSQL **dbconnection, char * server, char * user, char * password, int port)
{
  
  *dbconnection = mysql_init(NULL);
  
  if (!mysql_real_connect(*dbconnection, server, user, password, "simbi", port, NULL, 0)) 
    printf("Mysql error: %s (%d)\n", mysql_error(*dbconnection), mysql_errno(*dbconnection));
  
  return 0;
}
