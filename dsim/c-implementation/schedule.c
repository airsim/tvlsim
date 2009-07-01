/*
 * dSim prototype
 *
 */

// O-D -> int translator; its a map

#include <stdlib.h>
#include <stdio.h>


#include "types.h"


int balance (struct ond **cur_ond_map)
{
  printf("Sorry, not implemented ;(\n");
  return 0;
}

int iter (struct ond *cur_ond_map)
{
  int rc ; 
  if (cur_ond_map != NULL)
    {
      printf ("Leaf: %s -> %s; %d -- balanced ? %d\n", cur_ond_map->origin, cur_ond_map->destination, cur_ond_map->id, cur_ond_map->balanced);
      rc = iter (cur_ond_map->rond); 
      rc = iter (cur_ond_map->lond); 
    }
  return 0;

}


int insert(struct ond **cur_ond_map, char *origin, char *destination, int *counter)
{
  if (*cur_ond_map == NULL) 
    {
      struct ond *new_leaf = (struct ond *) malloc(sizeof(struct ond)); 
      new_leaf->origin = origin;  // origin est bien alloue de maniere dure, non ?
      new_leaf->destination = destination; 
      new_leaf->id = (*counter); 
      new_leaf->balanced = 0;
      new_leaf->rond = NULL; 
      new_leaf->lond = NULL; 
      (*counter)++; 
      *cur_ond_map = new_leaf; 
    }
  else 
    {

      switch (strcmp((*cur_ond_map)->origin, origin))
	{
	case 0:
	  {
	    switch (strcmp((*cur_ond_map)->destination, destination))
	      {
	      case 0:
		{
		  return 1;
		}
	      case 1: 
		{
		  (*cur_ond_map)->balanced++; 
		  insert(&((*cur_ond_map)->rond), origin, destination, counter); 
		  if ((*cur_ond_map)->balanced == 2)
		    {
		      balance(cur_ond_map);
		    }
		}
	      case -1: 
		{
		  (*cur_ond_map)->balanced--; 
		  insert(&((*cur_ond_map)->lond), origin, destination, counter);
		  if ((*cur_ond_map)->balanced == -2)
		    {
		      balance(cur_ond_map);
		    }
		}
		
	      }
	  }
	case 1: 
	  {
	    (*cur_ond_map)->balanced++; 
	    insert(&((*cur_ond_map)->rond), origin, destination, counter); 
	    if ((*cur_ond_map)->balanced == 2)
	      {
		balance(cur_ond_map);
	      }
	  }
	case -1: 
	  {
	    (*cur_ond_map)->balanced--; 
	    insert(&((*cur_ond_map)->lond), origin, destination, counter);
	    if ((*cur_ond_map)->balanced == -2)
	      {
		balance(cur_ond_map);
	      }
	  }
	  
	}
    } 

  return 0;
}

int find(struct ond *cur_ond_map, char *origin, char *destination, int *id)
{
  if (cur_ond_map == NULL) return -1 ;
  
  switch (strcmp(cur_ond_map->origin, origin))
    {
    case 0:
      {
	switch (strcmp(cur_ond_map->destination, destination))
	  {
	  case 0:
	    {
	      *id = cur_ond_map->id;
	      return 0;
	    }
	  case 1: 
	    {
	      return (find(cur_ond_map->rond, origin, destination, id)); 
	    }
	  case -1: 
	    {
	      return (find(cur_ond_map->lond, origin, destination, id)); 
	    }
	    
	  }
      }
    case 1: 
      {
	return(find(cur_ond_map->rond, origin, destination, id)); 
      }
    case -1: 
      {
	return(find(cur_ond_map->lond, origin, destination, id)); 
      }
      
    }
  
}

int find_by_id(struct ond *cur_ond_map, char **origin, char **destination, int id)
{
  // fonction sordide
  if (cur_ond_map == NULL) return -1 ;
  int rc; 
  if (cur_ond_map->id == id) 
    {
      *origin = cur_ond_map->origin; 
      *destination = cur_ond_map->destination; 
      return 0; 
    }
  else 
    {
      int rc1, rc2; 
      rc1 = find_by_id(cur_ond_map->lond, origin, destination, id); 
      rc2 = find_by_id(cur_ond_map->rond, origin, destination, id); 
      if (rc1 == 0 || rc2 == 0) 
	return 0 ; 
      else 
	return -1 ;
    }
 
} 

/*
int main()
{
  struct ond *map; 
  int rc; 
  int counter; 
  
  map = NULL;
  counter = 0;  
  
  rc = insert(&map, "LHR", "NCE", &counter);
  rc = insert(&map, "PAR", "NYC", &counter);
 
  rc = insert(&map, "RNC", "MAR", &counter);
  
  int id; 
  rc = find(map, "PAR", "NY", &id); 
  printf("Find returned %d %d\n", rc, id); 

  printf ("Insert done: %d\n", rc); 
  return 0; 

}
*/

int build_ond_map(struct ond **map, struct schedule *sched, int *counter)
{
  if (sched == NULL) return 0; 
  int rc; 
  printf("Origin: %s -> %s\n", sched->origin,  sched->destination); 
  rc = insert(map, sched->origin, sched->destination, counter); 
  rc = build_ond_map(map,sched->next, counter); 
  return 0;
}


