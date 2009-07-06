/*
 * C prototype for dSim - demand generation
 *
 */ 

int get_next_travel_request(int **repartition, int *flight_date);

int prepare_demand(struct ond *cur_ond_map, int nb_ond, struct demand *demand, struct generators **gens); 
