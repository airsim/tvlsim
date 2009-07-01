/*
 * C prototype for dSim - slave stuff
 *
 * We want sth more modular
 *
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "mpi.h"

#include <mysql/mysql.h>

#ifndef min
	#define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif

#ifndef max
	#define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif

int live(int duration, 
	 int nb_guillotines, 
	 int * translator,
	 unsigned short * guillotines, 
	 unsigned short * forecasts, 
	 void * forecaster,
	 MYSQL *dbconnection)
{
  MPI_Status status;
  int date = 0; 

  while (date < duration) 
    {
      int sales_request[2]; 
      MPI_Recv(sales_request, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, &status); 
      
      if (sales_request[0] > -1) 
	{
	  // Standard sale request
	  //  printf("Standard sale request date: %d : %d %d\n", date,  sales_request[1],  sales_request[0] * duration * 365 + (sales_request[1]) * 365 +  (sales_request[1] - date)); 
	  
	  guillotines[ sales_request[0] * duration * 365 + sales_request[1] * 365 +  (sales_request[1] - date) ]++ ; 	  
	   
	}
      else 
	{

	  // Forecast 
	  
	  int guillotine_id ; 
	  // printf("--- Midnight: %d %d\n", date, nb_guillotines); 
	  date++; 
	  for (guillotine_id = 0; guillotine_id < nb_guillotines ; guillotine_id++)
	    {
	      float average ;
	      int flight_date ; 
	      int dtd; 
	      
	      for (dtd = 0; dtd < 366; dtd++) 
		{
		  average = 0 ;  
		  
		  for (flight_date = 0; flight_date < (min(date + dtd, duration)); flight_date++)
		    {
		      average = average +  
			guillotines[ guillotine_id * duration * 365 + flight_date * 365  + dtd ] ; 	  
		    }
	
		  average = average / date * 100; 
		  
		  for (flight_date = date + dtd ; flight_date < duration; flight_date++)
		    {

		      forecasts[ guillotine_id * duration * 365  + flight_date * 365 +  dtd ] = (unsigned short) average ;

		    } 
		}
	    }
 
	  

	  
	  // printf("--- Midnight done: %d %d\n", date, nb_guillotines); 
	  MPI_Barrier( MPI_COMM_WORLD ) ;
	  
	}; 
 
    

    }  


}
