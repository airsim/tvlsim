/*
 * C prototype for dSim - proxy
 *
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "mpi.h"

int run_proxy (const char * path, int size, int nb_flights, int nb_classes, int duration, int requests_per_day); 
