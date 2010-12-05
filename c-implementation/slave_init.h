int init_guillotines (int * gduration, 
		      int * to_bi,
		      int * nb_guillotines, 
		      int ** translator, 
		      unsigned short ** guillotines, 
		      unsigned short ** forecasts) ;

int init_dbconnection (MYSQL **dbconnection, char * server, char * user, char * password, int port); 
