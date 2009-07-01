/*
 * C prototype for dSim - demand generation
 *
 */ 

#include "types.h"

int get_travel_request(int **repartition, int *flight_date)
{
  printf("get_travel_request\n"); 
  int uniform ; 
  uniform = rand () % 100; // 1000 = time resolution

  if (repartition[uniform] == NULL) return -1; 

  int range ;
  range = repartition[uniform][1] - repartition[uniform][0] ; 
  
  *flight_date = repartition[uniform][1] +  (rand () % range); 
 
  return 0;
}


int prepare_demand(struct ond *cur_ond_map, int nb_ond, struct demand *demand, struct generators **gens)
{

  (*gens) = (struct generators *) malloc (sizeof(struct generators)); 
  (*gens)->repartition = (int ***) malloc (sizeof(int **)  * nb_ond); 
  (*gens)->bookings_per_day = (int *) malloc (sizeof(int) * nb_ond); 
  
  struct demand *tdemand = demand; 
  
  int it; 
  for (it = 0; it < nb_ond; it++)
    {
      ((*gens)->repartition)[it] = NULL;
    }
  
  while(tdemand != NULL)
    {
      int ond_id ;
      int rc ; 
      printf("We have demand for %s %s\n",  tdemand->origin,  tdemand->destination ); 
      rc = find(cur_ond_map, tdemand->origin, tdemand->destination, &ond_id); 
      printf("This ond translates to %d\n", ond_id);
      if (!rc) 
	{
	  ((*gens)->repartition)[ond_id] = (int **) malloc (100 * sizeof(int *)); 
	  
	  struct share * trepartition = tdemand->repartition; 
	  int pos;			       
	  pos = 0; 
	  
	  while (trepartition != NULL)
	    {
	      int it;
	      int *range = (int *) malloc (2 * sizeof(int)); 
	      range[0] = trepartition->bdate; 
	      range[1] = trepartition->edate; 
	      for (it = 0; it < trepartition->percentage; it++)
		{
		  (((*gens)->repartition)[ond_id])[pos] = range; 
		  pos++;  
		} 
	      trepartition = trepartition->next;
	    }
	  	  
	  ((*gens)->bookings_per_day)[ond_id] = tdemand->bookings_per_day; 
	}
      else printf("Data is not coherent!!\n"); 
      tdemand = tdemand->next; 
    }
  
  return 0;
}
