/*
 * dSim prototype - Types
 *
 */

#include <sys/types.h>

#include <netdb.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct ond
{
  char *origin; 
  char *destination; 
  int id; 
  
  int balanced;
  struct ond *rond;
  struct ond *lond;
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
  
  struct flight *next; 
  
};

struct travel_solution {
  struct flight *first; 
  struct travel_solution *next;

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

struct share
{
  int percentage; 
  int bdate;
  int edate;
  struct share *next;
};

struct demand {
  char *origin; 
  char *destination; 
  
  int bookings_per_day; 
  struct share *repartition; 
  struct demand *next; 
};

struct generators {
  int ***repartition; 
  int *bookings_per_day; 
};

