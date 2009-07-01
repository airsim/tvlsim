/*
 * C prototype for dSim - master stuff
 *
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "mpi.h"

int run_master (int size, int nb_flights, int nb_classes, int duration, int requests_per_day); 
