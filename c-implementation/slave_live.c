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
	 MYSQL *dbconnection)
{
  MPI_Status status;
  int date = 0; 
  printf("Starting sim; %d\n", duration); 
  while (date < duration) 
    {
      printf("Expecting sales request ...\n"); 
      int sales_request[2]; 
      MPI_Recv(sales_request, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, &status); 
      printf("Sale request received: %d %d\n", sales_request[0], sales_request[1]); 
      if (sales_request[0] > -1) 
	{
	  guillotines[ sales_request[0] * duration * 365 + sales_request[1] * 365 +  (sales_request[1] - date) ]++ ; 	  
	}
      else if (sales_request[0] == -1)
	{
	  // This is midnight 
	  // Dump to mysql
	  if (dbconnection != NULL && date > 0 && date < duration) 
	    {
	      int guillotine_id ; 
	      for (guillotine_id = 0; guillotine_id < nb_guillotines; guillotine_id++)
		{ // BROKEN function
		  int it;
		  for (it = 0; it < (min(366, duration - date)); it ++)
		    {
		      //      printf("Flushing to mysql : %d\n",  guillotine_id * duration * 365 + (366 + date + it) * 365 +  it); 
		      char q [1000];
		      char airline_code[3]; 
		      airline_code[0] =  translator[ 4 * guillotine_id ]; 
		      airline_code[1] =  translator[ 4 * guillotine_id + 1]; 
		      airline_code[2] =  '\0'; 

		      sprintf(q,"INSERT INTO tvlsim_dump (RID, airline_code, flight_number, class_code, day_to_departure, flight_date, insertion_date, booking_counter, forecast) VALUES (\"%d\", \"%s\", \"%d\", \"%d\", \"%d\", \"%d\", \"%d\",  \"%d\", \"%d\")",
			      1, 
			      airline_code, 
			      translator[ 4 * guillotine_id + 2],  
			      translator[ 4 * guillotine_id + 3], 
			      it,
			      date + it, 
			      date,
			      guillotines[ guillotine_id * duration * 365 + (date + it) * 365 +  it ], 
			      forecasts[ guillotine_id * duration * 365 + (date + it) * 365 +  it ]) ;
		      
		      mysql_send_query(dbconnection, q, strlen(q));
		      
		    }
		  
		  
		}
	    }
	  
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
	
		  average = (average * 100) / date; 
		  
		  for (flight_date = date + dtd ; flight_date < duration; flight_date++)
		    {

		      forecasts[ guillotine_id * duration * 365  + flight_date * 365 +  dtd ] = (unsigned short) average ;

		    } 
		}
	    }
 	  
	  printf("--- Midnight done: %d %d\n", date, nb_guillotines); 
	  MPI_Barrier( MPI_COMM_WORLD ) ;
	  
	}
      else 
	{
	  // End of simulation 
	  printf("End signal !\n"); 
	
	  return 0;

	}
; 
 
    

    }  


}
