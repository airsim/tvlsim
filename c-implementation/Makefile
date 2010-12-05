## C dSim implementation - Requires MPI libs and usual tools
 
all: dSim

%.o: %.c 
	mpicc $^ -c

dSim: parser.o schedule.o demand_generation.o interface.o master.o slave.o slave_init.o slave_live.o prototype.o 
	mpicc -inline $^ -o $@ -L/usr/lib/mysql -lmysqlclient

connector: connector.c
	gcc $^ -o $@

parser: parser.c
	gcc $^ -o $@

schedule: schedule.c
	gcc $^ -o $@

graph: airport_graph.c 
	gcc $^ -o $@

clean: 
	rm -rf *.o 
	rm -rf dSim
