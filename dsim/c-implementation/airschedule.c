/*
 * dSim c prototype - inventory stuff
 *
 */

// Here we deal with O&D


// looking for travel solution must be fast (and distributed)

struct route {
  struct hop *destination; 
  int flight_code ; 
  int board_time; 
  int arrival_time; 
};

struct hop {
  char *airport_code; 
  struct hop *next; 
}; 


struct world_schedule{
  char *map;
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





int init_world(struct world **wd)
{
  *wd = (struct world *) malloc (sizeof(struct world)); 
  (*wd)->map = (char *) malloc (3 * 25 * 25 * 25 * sizeof(int));

  return 0;
}

// We need a structure that allow easy splitting among the clients


int build_client_side(int nb_sched_elts)
{
  

}

int insert(char *map, char *origin, char *destination, int flight_id)
{
  int pos_origin = ((int) origin[0] - 65 ) * 25 * 25 + ((int) origin[1] - 65 ) * 25 + ((int) origin[2] - 65 ); 
  int pos_destination = ((int) destination[0] - 65 ) * 25 * 25 + ((int) destination[1] - 65 ) * 25 + ((int) destination[2] - 65 ); 

  struct hop *hp = (struct hop *) malloc (sizeof(struc hop)); 
  
  hp->airport_code = pos_destination; 
  hp->next = map[pos_origin]; 
  
  map[pos_origin] = hp; 
 
}


int ask_availability(char *origin, char *destination, struct inventory *inv)
{
  
  

}

