/*
 * dSim C prototype - world schedule helper
 *
 */

#include <stdio.h>
#include <stdlib.h>

struct airport_map
{
  struct airport *airport; 
  struct airport_map *left; 
  struct airport_map *right; 
}; 

struct flight
{
  char *airline_code;
  
  int flight_number; 
  
  int agent ; 
  int id; 
  
  int first_flight_date; 
  int last_flight_date; 

  int board_time; 
  int arrival_time;
 
  int nb_classes; 
  char *capacities; 

};

struct flight_container 
{
  struct flight *flight; 
  struct airport *airport_destination; 
  struct flight_container *next;
};

struct airport
{
  char *airport_code; 
  struct flight_container *flights;
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

struct travel_solution
{
  int number_of_segments; 
  struct flight **segments;
  struct travel_solution *next;
};

struct ond {
  char *origin; 
  char *destination; 

  struct travel_solution *solution;

  struct ond *left;
  struct ond *right;
};


// recursive display of the flights

int display_flights(struct flight_container *fl)
{
  if (!fl) return 0; 
  printf(" --- Flight %s %d to %s\n", fl->flight->airline_code, fl->flight->flight_number, fl->airport_destination->airport_code); 
  return (display_flights(fl->next)); 
}
// recursive display of the map
int display_airport_map(struct airport_map *map)
{
  if (!map) return 0; 
  int rc ; 
  
  printf("AIRPORT %s\n", map->airport->airport_code);
  rc = display_flights(map->airport->flights);
  rc = display_airport_map(map->right); 
  rc = display_airport_map(map->left); 
  
  return 0;
}


int dispatch_flights_on_airports(struct airport_map **map,
				 struct schedule *schedule)
{
  if (!schedule) return 0; 
  printf("We are dispatching %s %s %s %d\n", schedule->origin, schedule->destination, schedule->airline_code, schedule->flight_number); 
  struct flight *new_flight = (struct flight *) malloc (sizeof(struct flight)); 
  
  new_flight->airline_code = schedule->airline_code; 
  new_flight->flight_number = schedule->flight_number;
  
  new_flight->first_flight_date = schedule->first_flight_date; 
  new_flight->last_flight_date = schedule->last_flight_date; 
  
  new_flight->board_time = schedule->board_time; 
  new_flight->arrival_time = schedule->arrival_time;
  
  new_flight->nb_classes = schedule->nb_classes; 
  new_flight->capacities = schedule->capacities; 
  
  new_flight->agent = -1; // No agent for the moment
  new_flight->id = -1; 
 
  
  struct flight_container *container = (struct flight_container *) malloc (sizeof(struct flight_container)); 
  container->flight = new_flight; 
  container->next = NULL;

  int rc; 
  rc = get_or_create(map, schedule->destination, &(container->airport_destination)); 

  // we now have to insert this container at the correct place

  rc = insert_or_create(map, schedule->origin, container); 
  rc = dispatch_flights_on_airports(map, schedule->next);
  return 0;
}

// recursive stuff
int get_or_create(struct airport_map **map, 
		  char *destination, 
		  struct airport **airport)
{
  if (! *map) 
    {
      // The airport doesn't exist
      struct airport_map *new_airport_container = (struct airport_map *) malloc (sizeof(struct airport_map)); 
      struct airport *new_airport = (struct airport *) malloc (sizeof(struct airport)); 

      new_airport_container->airport = new_airport; 
      new_airport_container->left = NULL; 
      new_airport_container->right = NULL; 

      new_airport->airport_code = destination; 
      new_airport->flights = NULL; 

      *map = new_airport_container; 
      
      *airport = new_airport ;
      
      return 0;
      
    }
  else 
    {
       switch (strcmp((*map)->airport->airport_code, destination))
	{
	case 0:
	  {
	    // We found the right node
	    *airport = (*map)->airport;
	    return 0;
	  }
	case 1:
	  {
	    return 
	      (get_or_create(&((*map)->right), destination, airport)); 
	  }
	case -1:
	  {
	    return 
	      (get_or_create(&((*map)->left), destination, airport));
	  }  
	}
    }
}


// recursive stuff; inserting stuff in the map
int insert_or_create(struct airport_map **map, 
		     char *origin, 
		     struct flight_container *container)
{
  if (! *map) 
    {
      // The airport doesn't exist
      struct airport_map *new_airport_container = (struct airport_map *) malloc (sizeof(struct airport_map)); 
      struct airport *new_airport = (struct airport *) malloc (sizeof(struct airport)); 

      new_airport_container->airport = new_airport; 
      new_airport_container->left = NULL; 
      new_airport_container->right = NULL; 

      new_airport->airport_code = origin; 
      new_airport->flights = container; 

      *map = new_airport_container; 
            
      return 0;
      
    }
  else 
    {
       switch (strcmp((*map)->airport->airport_code, origin))
	{
	case 0:
	  {
	    // We found the right node
	    container->next = ((*map)->airport)->flights;
	    (*map)->airport->flights = container;
	    return 0;
	  }
	case 1:
	  {
	    return 
	      (insert_or_create(&((*map)->right), origin, container)); 
	  }
	case -1:
	  {
	    return 
	      (insert_or_create(&((*map)->left), origin, container));
	  }  
	}
    }
}



////////////////////////// OnD functions + balancing over the agents

int display_ond_map(struct ond *ond_map)
{
  if (!ond_map) return 0; 
  
  printf ("O: %s D: %s\n", ond_map->origin, ond_map->destination);
  int rc; 
  rc = display_ond_map(ond_map->right); 
  rc = display_ond_map(ond_map->left); 
  return 0; 

}

int build_simple_ond_map (struct ond **ond_map, struct airport_map *map)
{
  if (!map) return 0; 
  int rc ;

  rc = build_simple_ond_map_single(ond_map, map); 
  rc = build_simple_ond_map(ond_map, map->right); 
  rc = build_simple_ond_map(ond_map, map->left); 
  
  return 0;   
  
}

int build_simple_ond_map_single (struct ond **ond_map, struct airport_map *map)
{
  struct flight_container *container ;

  container = map->airport->flights; 
  while (container != NULL)
    {
      struct travel_solution *fts = (struct travel_solution *) malloc (sizeof(struct travel_solution)); 
      fts->number_of_segments = 1; 
      fts->segments = (struct flight **) malloc (sizeof(struct flight *)); 
      (fts->segments)[0] = container->flight;
      fts->next = NULL;
      int r; 
      r = inject_ts (ond_map, map->airport->airport_code, container->airport_destination->airport_code,  fts); 
      
      container = container->next; 
    }
}

int inject_ts (struct ond **ond_map, char *origin, char *destination, struct travel_solution *fts)
{
  printf ("Inject an OnD %s %s\n", origin, destination); 
  if ( *ond_map == NULL )
    {
      // This OnD does not exists 
      struct ond *new_ond = (struct ond *) malloc (sizeof(struct ond)); 
      new_ond->origin = origin; 
      new_ond->destination = destination; 
      new_ond->solution = fts; 
      new_ond->left = NULL; 
      new_ond->right = NULL; 
      *ond_map = new_ond; 
      return 0; 
    }
  else 
    {
      switch(strcmp((*ond_map)->origin, origin))
	{
	case 0: 
	  {
	    switch(strcmp((*ond_map)->destination, destination))
	      {
	      case 0: 
		{
		  fts->next = (*ond_map)->solution; 
		  (*ond_map)->solution = fts;
		  return 0;
		}
	      case 1:
		{
		  return (inject_ts( &((*ond_map)->right), origin, destination, fts)); 
		}
	      case -1: 
		{
		  return (inject_ts( &((*ond_map)->left), origin, destination, fts)); 
		}
	      }
	  }
	case 1:
	  {
	    return (inject_ts( &((*ond_map)->right), origin, destination, fts)); 
	  }
	case -1: 
	  {
	    return (inject_ts( &((*ond_map)->left), origin, destination, fts)); 
	  }
	}

    }
}



//////////////////////// OnD gets complicated - we allow 2 segments 


int extend_ts_over_ts(struct ond **ond_map_root, struct travel_solution *cur_sol, struct travel_solution *ts, char*origin, char *destination)
{
  // chaque solution par 
  printf("Considering the OnD %s %s\n", origin, destination); 
  if (!cur_sol) return 0; 
  
  int nnseg = ts->number_of_segments + cur_sol->number_of_segments; 
  if (nnseg < 4)
    {
      struct travel_solution *extended = (struct travel_solution *) malloc (sizeof(struct travel_solution)); 
      
      extended->number_of_segments = nnseg; 
      extended->next = NULL; 
      extended->segments = (struct flight **) malloc (nnseg * sizeof(struct flight *)); 
      int it; 
      for (it = 0; it< ts->number_of_segments; it++)
	(extended->segments)[it] = (ts->segments)[it]; 
      
      int offset = it; 
      
      for (it = 0 ; it< cur_sol->number_of_segments; it++)
	(extended->segments)[it + offset] = (cur_sol->segments)[it]; 
      
      int rc; 
      rc = inject_ts(ond_map_root, origin, destination, extended); 
    }
  return(extend_ts_over_ts(ond_map_root, cur_sol->next, ts, origin, destination)); 
}

int extend_ts(struct ond **ond_map_root, struct ond *ond_map, char *origin, char* destination,  struct travel_solution *ts)
{
  if (!ond_map) return 0; 

  int rc; 
  printf("ond :: %s %s\n",ond_map->origin , destination);
  if (strcmp(ond_map->destination, origin) == 0)
    {
      rc = extend_ts_over_ts(ond_map_root, ond_map->solution, ts, ond_map->origin, destination);  // Toutes les solutions partent de destination 
      rc = extend_ts(ond_map_root, ond_map->right, origin, destination, ts); 
      rc = extend_ts(ond_map_root, ond_map->left, origin, destination, ts); 
    }
  else
    {
      rc = extend_ts(ond_map_root, ond_map->right, origin, destination, ts); 
      rc = extend_ts(ond_map_root, ond_map->left, origin, destination, ts); 
    }
  return 0; 

}


int one_ts_iteration(struct ond **ond_map, struct ond *ptr)
{
  if (!ptr) return 0; 
  
  int rc; 

  rc = extend_ts(ond_map, (*ond_map), ptr->origin, ptr->destination, ptr->solution); 

  rc = one_ts_iteration(ond_map, ptr->right);
  rc = one_ts_iteration(ond_map, ptr->left);
  
  return 0; 
}


int main()
{
  int rc; 
  struct airport_map *map; 
  map = NULL ;

  struct schedule *sh1 = (struct schedule *) malloc (sizeof(struct schedule)); 
  sh1->origin = "LHR"; 
  sh1->destination = "NCE"; 
  sh1->airline_code = "AF"; 
  sh1->flight_number = 100; 
  sh1->next = NULL; 
  struct schedule *sh2 = (struct schedule *) malloc (sizeof(struct schedule));
  sh2->origin = "JFK"; 
  sh2->destination = "LHR"; 
  sh2->airline_code = "BA"; 
  sh2->flight_number = 12; 
  sh2->next = sh1; 
  struct schedule *sh3 = (struct schedule *) malloc (sizeof(struct schedule));
  sh3->origin = "JFK"; 
  sh3->destination = "CDG"; 
  sh3->airline_code = "LU"; 
  sh3->flight_number = 7; 
  sh3->next = sh2; 
  
  rc = dispatch_flights_on_airports(&map, sh3);  

  rc = display_airport_map(map);

  struct ond *ond_map; 
  ond_map = NULL; 

  rc = build_simple_ond_map(&ond_map, map);
  
  rc = display_ond_map(ond_map); 

  rc = one_ts_iteration (&ond_map, ond_map); 
  
  rc = display_ond_map(ond_map); 
  
  return 0; 
}
