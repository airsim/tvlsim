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

#include "slave_init.h"
#include "slave_live.h"
 


int pending_slave (void)
{
  int rc; 
  
  MYSQL *dbconnection;
    
  int nb_guillotines ;
  int * translator; 
  unsigned short * guillotines; 
  unsigned short * forecasts; 
  
  int duration; 
  int to_bi; 

  rc = init_guillotines(&duration, &to_bi, &nb_guillotines, &translator, &guillotines, &forecasts);
  
  // Display translator
  

  if (rc == -1) printf ("Malloc failed .. \n"); 
   
  if (to_bi)
    {
      rc = init_dbconnection(&dbconnection, "nceorilnx02", "root", "pods3030", 3306); 
      
      rc = live(duration, 
		nb_guillotines, 
		translator, 
		guillotines, forecasts, dbconnection);
    }
  else 
    {
      rc = live(duration, 
		nb_guillotines, 
		translator, 
		guillotines, forecasts, NULL);
    }
     
  MPI_Barrier( MPI_COMM_WORLD ) ;
 
  return 0;
  

}
