ALL: master slave

CC = mpicxx  -lmpi_cxx -lboost_mpi 

master: master.cpp
	  $(CC) master.cpp -o master
slave: slave.cpp
	  $(CC) slave.cpp -o slave

clean:
	  rm -fv master slave 
