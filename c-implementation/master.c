/*
 * C prototype for dSim - master stuff
 *
 */ 

/*
  TODO: 

  implement the use of the demand.csv file
 
  


 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "mpi.h"

#include "types.h"
#include "schedule.h"
#include "demand_generation.h"

#ifndef min
	#define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif


void display_schedule(struct schedule *sched)
{
  if (sched != NULL)
    {
      printf ("Schedule entry: %s %s %d %d\n", sched->origin, sched->destination, sched->flight_number, sched->nb_classes); 
      display_schedule(sched -> next); 
    }
  else return;

}

int pending_master(int size)
{
  int rc; 
 
  while (1) 
    {
      printf ("Entering pending state\n");
      int rid;
      int duration; 
      int to_bi; 

      // DEFAULT VALUES 
      rid = 0;
      duration = 100; 
      to_bi = 0; 
      

      struct schedule *sched;
      struct demand *demand; 

      sched = NULL; 
      demand = NULL;

      struct timeval deb; 
      struct timeval fin; 
      
      rc = get_configuration(&rid, &duration, &to_bi, &sched, &demand); 
         
      struct flight ** balancer;
      struct generators *generators; 
      printf("Ready to prepare_inputs\n"); 
      int counter; 
      prepare_inputs(size, duration, to_bi, &counter, sched, demand, &balancer, &generators); 

      printf("Let's go\n"); 

      rc = run(size, duration, counter, balancer, generators); 
      
      return 0;
    }
  
  return 0;
}

//Basic passenger choice
int passenger_choice(int ond, int flight_date, struct flight **balancer, struct flight **chosen_flight)
{
  (*chosen_flight) = balancer[ond]; 
  return 0;
}


// Run function
int run(int size, 
	int duration, 
	int nb_ond, 
	struct flight **balancer,
	struct generators *gens)
{  
  int date; 
  date = 0;


  while (date < duration) 
    {
      printf("-- Day %d\n", date); 
      int ond; 
      for (ond = 0; ond < nb_ond; ond++)
	{
	  int request_id; 
	  for (request_id = 0; request_id < (gens->bookings_per_day)[ond]; request_id++)
	    {
	      printf("Generating a travel request\n"); 
	      int flight_date; 
	      int rc; 
	      rc = get_travel_request((gens->repartition)[ond], &flight_date); 
	      flight_date = flight_date + date; 
	      if (!rc)
		{
		  printf("Travel request; insertion_date: %d ond: %d flight_date: %d\n", date, ond, flight_date); 
		  
		  struct flight *fl; 
		  passenger_choice(ond, flight_date, balancer, &fl); // Very very basic !!
		  
		  // Update the forecast
		  
		  int sale_request[2]; 
		  
		  sale_request[0] = fl->id; 
		  sale_request[1] = flight_date;
		  
		  MPI_Send(sale_request, 2, MPI_INT, fl->agent, 0, MPI_COMM_WORLD) ;
		}
	      else 
		printf("I can't build a travel request !\n"); 
	    }

	}

      // Midnight !
      
      int sale_request[2]; 
      
      sale_request[0] = -1;  
      sale_request[1] = 0;
      int agent ; 
      for (agent = 1; agent < size; agent++)
	{
	  MPI_Send(sale_request, 2, MPI_INT, agent, 0, MPI_COMM_WORLD) ;
	}
      MPI_Barrier( MPI_COMM_WORLD ) ;

      date++;
      

    }
  MPI_Barrier( MPI_COMM_WORLD ) ;


  return 0;		     
}

int prepare_inputs(int size, 
		   int duration, 
		   int to_bi, 
		   int *gcounter,
		   struct schedule *sched, 
		   struct demand *demand,
		   struct flight *** balancer, 
		   struct generators ** gens)
		   
{
  printf ("Preparing inputs\n"); 
  struct schedule *tsched; 
  tsched = sched; 
  
  int counter; 
  counter = 0;
  
  struct ond *map; 
  map = NULL;

  build_ond_map(&map, sched, &counter);
  

  *gcounter = counter;
  printf ("Schedule size: %d\n", counter);  
  // -> now we have the map storing ond ids, counter is the size of the map.
  
  *balancer = (struct flight **) malloc (counter * sizeof(struct flight *)); 
  int counters[size - 1]; 
  
  int it ; 
  
  for (it = 0; it < counter; it++)
    {
      (*balancer)[it] = NULL;

    }
  for (it = 1; it < size; it++)
    {
      counters[it - 1] = 0; 
    }
  
  int agent = 1 ;
  
  tsched = sched; 
  // Only deals with direct flights, see schedule.c for an network computation function
  while(tsched != NULL)
    {
      int id; 
      int rc; 
      rc = find(map, tsched->origin, tsched->destination, &id); 
      if (rc == 0)
	{
	  struct flight *new_flight = (struct flight *) malloc (sizeof(struct flight)); 
  
	  new_flight->airline_code = tsched->airline_code;
	  new_flight->flight_number = tsched->flight_number; 
	  
	  new_flight->first_flight_date = tsched->first_flight_date; 
	  new_flight->last_flight_date = tsched->last_flight_date; 
	  
	  new_flight->board_time = tsched->board_time; 
	  new_flight->arrival_time = tsched->arrival_time;
	  
	  new_flight->nb_classes = tsched->nb_classes; 
	  new_flight->capacities = tsched->capacities; 
	  
	  new_flight->agent = agent; 
	  

	  new_flight->id = counters[agent - 1]; 

	  counters[agent - 1] = counters[agent - 1] + tsched->nb_classes;

	  new_flight->next = (*balancer)[id];
	  (*balancer)[id] = new_flight ;
	}
      else {printf("Inconsistent data, I go on but some flights might be forgotten.\n");}
      agent++;
      if (agent == size) agent = 1; 
      tsched = tsched->next;
    }
  
  // Free what has to be freed .. 
  printf("Send aggregated workload to the clients\n");
  // SEND the info to all the guillotine clients
  for (agent = 1; agent < size; agent++)
    {
      printf("Agent %d, counter %d %d %d\n", agent, counters[agent - 1], duration, to_bi);
      int capsule[3]; 
      capsule[0] = counters[agent - 1]; 
      capsule[1] = duration; 
      capsule[2] = to_bi; 
      
      MPI_Send(capsule, 3, MPI_INT, agent, 0, MPI_COMM_WORLD) ;       
    }
  
  MPI_Barrier( MPI_COMM_WORLD ) ;

  printf("Send translation data to clients\n");
  // SEND the info for translating to the BI to all the guillotine clients
  
  for (it = 0; it < counter; it++)
    {
      struct flight * tfl; 
      tfl = (*balancer)[it]; 
      while (tfl != NULL) 
	{
	  printf("Still going through .. !\n"); 
	  int flight[5]; 
	  flight[0] = (int)((tfl->airline_code)[0]);
	  flight[1] = (int)((tfl->airline_code)[1]); // Not that clean (lacks '\0')
	  flight[2] = tfl->flight_number; 
	  flight[3] = tfl->nb_classes; 
	  flight[4] = tfl->id;
	  
	  MPI_Send(flight, 5, MPI_INT, tfl->agent, 0, MPI_COMM_WORLD) ;    
	  tfl = tfl->next; 
	}
    
    }
  
  MPI_Barrier( MPI_COMM_WORLD ) ;
  printf("Send translation data SENT\n");
  
  
  // We have to prepare the demand_generators
  
  int rc; 
  
  rc = prepare_demand(map, counter, demand, gens); 
  
  // We are ready !!
  
  return 0;
  
 
  
}
