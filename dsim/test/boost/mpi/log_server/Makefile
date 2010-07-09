ALL: log_server client 

CC = mpicxx  -lmpi_cxx -L/usr/lib/openmpi/lib/ -lboost_mpi

log_server: log_server.cpp
	  $(CC) log_server.cpp -o log_server
client: client.cpp
	  $(CC) client.cpp -o client

clean:
	  rm -fv log_server client
