/*
* Object: Make a simple log server
* Utilisation: Run log_server_client.sh
* Problem: avoid competitive sending, i.d 2 clients send in a same time
* Solution: Server takes care of client one by one
* Note: Usage of inactive_wait funtion, which reduces CPU charge evidently
*/

#include <boost/mpi.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include<stdio.h>
// Some log macros
#include"log_service.hpp"


using namespace std;
namespace mpi=boost::mpi;

/* Write message to filename */
void logToFile(string filename, string message){
	ofstream out;
	out.open(filename.c_str(), ios_base::app);
	out << message << "\n" ;
	out.close();
}

//inactive waiting: wake up every 1s for verifying if there is a message
void inactive_wait(mpi::communicator& world){
	boost::optional<mpi::status> stat = boost::none;//stat is not initialised
	while(1){
		stat = world.iprobe(mpi::any_source, mpi::any_tag); //inactive waiting

		//usleep(100);//we have the faster  result but it takes more CPU
		sleep(1); //better choice

		if (stat){
			stat = boost::none; //stat goes back to waiting state
			return;//ah, someone calls me !
		}
			
	}
	//world.probe(root,tag);//active waiting: take much more resources
}


int main(int argc, char ** argv){
	mpi::environment env(argc, argv);
	mpi::communicator world;

	int root = 0; //server's rank
	int tag = 0; // the tag (or the port) used for communication
	int dest;
	
	int rank = world.rank(); //process's rank
	string message; // message received from client
	bool yes=true;
	
	if (rank==0){

		
		
		cout << "We have " << world.size() -1 << " clients " << endl;
		cout << "=============================================================" << endl;
	
		while (1){
			//waits for a demand, might takes a while as client aren't always gossip
			inactive_wait(world);
			world.recv(mpi::any_source,tag,dest);
			
			cout << "server received from client: " << dest << endl ;
			world.send(dest,tag,yes); //ok, you can do it

			//  waiting for message from client
			inactive_wait(world);
			world.recv(dest,tag,message);
			
			LOG_DEBUG("ex.log", message);
			
			cout << "server received: " << message << endl ;
		}
	}

	return 0;
};
