/*
 * dSim prototype
 *
 */

// O-D -> int translator; its a map

#include <stdlib.h>
#include <stdio.h>


#include "types.h"

struct flight
{
  char *airline_code;
  char *destination;
  
  int flight_number; 
  
  int agent ; 
  int id; 
  
  int first_flight_date; 
  int last_flight_date; 

  int board_time; 
  int arrival_time;
 
  int nb_classes; 
  char *capacities; 

  struct flight *next;
  
};

struct flight_container 
{
  struct flight *flight; 
  char *destination;
  struct flight_container *next;
};

struct airport_map 
{
  char *id; 
  struct flight *flight; 
  struct airport *left;
  struct airport *right;
}; 

struct schedule
{
  char *origin; 
  char *destination; 
  char *airline_code;
  
  int flight_number; 
  
  int first_flight_date; 
  int last_flight_date; 

  int board_time; 
  int arrival_time;
 
  int nb_classes; 
  char *capacities; 
  
  struct schedule *next;  
};


struct ond_map
{
  char *origin; 
  char *destination; 
  int id; 
  
  int balanced;
 
  struct travel_solution *solution; 
  
  struct ond_map *left; 
  struct ond_map *right;
};

int insert_flight_container(struct airport_map **airport_map, struct flight *flight, char *origin)
{
  if (! *airport_map)
    {
      struct airport *new_airport = (struct airport_map *) malloc (sizeof(struct airport)); 
      new_airport->id = schedule_elt->origin; 
      new_airport->flight = flight; 
      new_airport->left = NULL; 
      new_airport->right = NULL;
      *airport_map = new_airport;
    } 
  else 
    {
      switch (strcmp((*airport_map)->id, origin))
	{
	case 0:
	  {
	    flight->next = (*airport_map)->flight; 
	    (*airport_map)->flight; 
	  }
	case 1:
	  {
	    insert_flight_container( &((*airport_map)->right), flight_container, origin); 
	  }
	case -1:
	  {
	    insert_flight_container( &((*airport_map)->left), flight_container, origin); 
	  }
	}
    }
  return 0; 
}

int build_airport_map(struct airport_map **airport_map, 
		      struct schedule *schedule)
{
  if (!schedule) return 0; 
  
  struct flight *new_flight = (struct flight *) malloc (sizeof(struct flight)); 

  new_flight->airline_code = schedule->airline_code; 
  new_flight->flight_number = schedule->flight_number;

  new_flight->destination = schedule->destination; 

  new_flight->first_flight_date = schedule->first_flight_date; 
  new_flight->last_flight_date = schedule->last_flight_date; 
  
  new_flight->board_time = schedule->board_time; 
  new_flight->arrival_time = schedule->arrival_time;
  
  new_flight->nb_classes = schedule->nb_classes; 
  new_flight->capacities = schedule->capacities; 
  
  new_flight->agent = -1; // No agent for the moment
  new_flight->id = -1; 
  
  new_flight->next = NULL;

  int rc; 

  rc = insert_flight_container(airport_map, new_flight, schedule->origin); 

  return (build_airport_map(airport_map, schedule->next));
}


int build_travel_solution(airport_map


int build_world_schedule(struct ond_map **ond_map, 
			 struct airport_map *airport_map)
{
  if (!airport_map) return 0; 

  char *origin = airport_map->id; 
  
  
 

}


