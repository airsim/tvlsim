/* 
 * Dsim prototype - A simple interface
 *
 *
 */

// TODO: safety check during the add flight operation

/*
  commands:

  set 
      rid <>
      duration <>
      to_bi <>
  add 
      flight <nb_classes> <generator id>
     
  run 
  quit
  help


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

#include "parser.h"

#define MAX_RECEPT 100

#define PORT 6667

int get_configuration(int *rid, 
		      int *duration, 
		      int *to_bi, 
		      struct schedule **sched, 
		      struct demand **demand)
{ 
  int socket_server, socket_client, server_len, client_len;
  struct sockaddr_in sockaddr_server, sockaddr_client;
  
  if((socket_server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      printf("Socket creation error !\n" );
      return EXIT_FAILURE;
    }
  
  memset(&sockaddr_server, 0, sizeof(sockaddr_server));
  sockaddr_server.sin_family = AF_INET;
  sockaddr_server.sin_port = htons(PORT);
  sockaddr_server.sin_addr.s_addr = INADDR_ANY;
  
  server_len = sizeof(sockaddr_server);
  client_len = sizeof(sockaddr_client);
  
  int opt = 1;  
  
  if (setsockopt(socket_server,SOL_SOCKET,SO_REUSEADDR, (char *)&opt,sizeof(opt)) < 0)
    {
      printf("Couldn't set reuse option properly !\n" );
    } 
  
  if(bind(socket_server, (struct sockaddr*)&sockaddr_server, server_len) < 0)
    {
      printf("Socket bind error !\n" );
      return EXIT_FAILURE;
    }
  
  if(listen(socket_server, 10) < 0)
    {
      printf("Socket listen error !\n" );
      return EXIT_FAILURE;
    }
  
  printf("Waiting connection on port 6667... \n");
  socket_client = accept(socket_server, (struct sockaddr*)&sockaddr_client, &client_len);
  
  
  char * greetings = "Welcome to dSim v0.01 + mpich1.0\n"; 
  write(socket_client, greetings, strlen(greetings)); 
  int goon ; 
  goon = 1;
  
  while(goon)
    {
      char buffer[MAX_RECEPT];
      int i; 
      for (i = 0; i < MAX_RECEPT; i++) buffer[i] = '\0'; 
      
      if(read(socket_client, &buffer, MAX_RECEPT) < 0)
	{
          printf("Socket read error !" );
          return EXIT_FAILURE;
	}
      
      char *operation;
      char *command;
      char *request; 
      
      operation = strtok(buffer, " ");
      
      if (!strcmp("set", operation))
	{
	  command = strtok (NULL, " "); 
	  request = strtok (NULL, "\n");
	  
	  if (!strncmp("rid", command, 3) && request)
	    {
	      if (sscanf(request, "%d", rid) == 1)
		sprintf(buffer, "RID set : %d\n", *rid); 
	      else 
		sprintf(buffer, "RID error\n");
	      write(socket_client, buffer, strlen(buffer)); 
	    } 
	  else if (!strncmp("duration", command, 3) && request)
	    {
	      if (sscanf(request, "%d", duration) != 1)
		sprintf(buffer, "bad input\n");
	      else 
		sprintf(buffer, "done\n");
	      
	      write(socket_client, buffer, strlen(buffer)); 
	    } 
	  else if (!strncmp("to_bi", command, 5) && request)
	    {
	      if (sscanf(request, "%d", to_bi) != 1)
		sprintf(buffer, "bad input\n");
	      else 
		sprintf(buffer, "done\n");
	      write(socket_client, buffer, strlen(buffer)); 
	    } 
	  else
	    {
	      sprintf(buffer, "set what ?\n");
	      write(socket_client, buffer, strlen(buffer)); 
	
	    }
	  
	}
      else if (!strncmp("add", operation, 3))
	{
	  command = strtok (NULL, " "); 
	  request = strtok (NULL, "\n");
	  
	  if (!strncmp("flight", command, 3) && request)
	    {

	      char *origin = (char *) malloc (4 * sizeof(char)); 
	      char *destination = (char *) malloc (4 * sizeof(char)); 
	      char *airline_code = (char *) malloc (3 * sizeof(char)); 
	      
	      int flight_number;
	      
	      int first_flight_date; 
	      int last_flight_date; 
	      
	      int board_time; 
	      int arrival_time;
	      
	      int nb_classes; 
	      
	      struct schedule *nsch;
	      
	      if (sscanf(request, "%s %s %s %d %d %d %d %d %d", origin, destination, airline_code, &flight_number, &first_flight_date, &last_flight_date, &board_time, &arrival_time, &nb_classes) == 9)
		{
		 
		  nsch = (struct schedule *) malloc (sizeof(struct schedule)); 
		  nsch->origin = origin; 
		  nsch->destination = destination; 
		  nsch->airline_code = airline_code; 

		  nsch->flight_number = flight_number; 

		  nsch->first_flight_date = first_flight_date; 
		  nsch->last_flight_date = last_flight_date; 

		  nsch->board_time = board_time; 
		  nsch->arrival_time = arrival_time; 
		  
		  nsch->nb_classes = nb_classes; 
		  nsch->next = *sched;
		  *sched = nsch;
		  sprintf(buffer, "Flight_added; origin: %s, destination: %s, flight_number: %d, nb_classes: %d\n", origin, destination, flight_number, nb_classes); 
		}
	      else 
		sprintf(buffer, "error flight\n");
	      write(socket_client, buffer, strlen(buffer)); 
	    } 
	  else if (!strncmp("demand", command, 3) && request)
	    {
	      struct demand *ndemand; 
	     
	      if (demand_from_string(&ndemand, request) == 0)
		{
		  ndemand->next = *demand; 
		  *demand = ndemand;
		  sprintf(buffer, "demand added\n");
		}
	      else
		{
		  sprintf(buffer, "incorrect demand spec\n");
		}
	      write(socket_client, buffer, strlen(buffer)); 
	    }
	  
	}
      else if (!strncmp("run", operation, 3))
	{
	  goon = 0;
	  sprintf(buffer, "Quit message received (I'm launching your simulation !)\n");
	  write(socket_client, buffer, strlen(buffer)); 
	}
      else if (!strncmp("quit", operation, 4))
	{
	  goon = 0;
	  *rid = -1; 
	  sprintf(buffer, "Quit message received\n");
	  write(socket_client, buffer, strlen(buffer)); 
	}
      else if (strlen(operation) > 2)
	{
	  sprintf(buffer, "Welcome to dSim interface, here are the commands:\n");
	  write(socket_client, buffer, strlen(buffer)); 
	  sprintf(buffer, "      quit            : Quit\n");
	  write(socket_client, buffer, strlen(buffer)); 
	  sprintf(buffer, "      rid <0-..>      : Set the RID\n");
	  write(socket_client, buffer, strlen(buffer)); 
	  sprintf(buffer, "      nb_classes      : Set classes number\n");
	  write(socket_client, buffer, strlen(buffer)); 
	  sprintf(buffer, "      run             : Run the simulation\n");
	  write(socket_client, buffer, strlen(buffer)); 
	  sprintf(buffer, "      help            : Display this message\n");
	  write(socket_client, buffer, strlen(buffer)); 
	}
    }
  
  close(socket_client);
  close(socket_server);

  return 0;
}

