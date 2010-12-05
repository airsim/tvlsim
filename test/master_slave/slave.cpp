#include <boost/mpi.hpp>
#include <iostream>
#include <string>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include<stdio.h>

using namespace std;
namespace mpi=boost::mpi;

//inactive waiting: wake up every 1s for verifying if there is a message
void inactive_wait(mpi::communicator& world, int source=mpi::any_source){
	boost::optional<mpi::status> stat = boost::none;//stat is not initialised
	while(1){
		stat = world.iprobe(source, mpi::any_tag); //inactive waiting

		//usleep(100);//we have the faster  result but it takes more CPU
		sleep(1); //better choice

		if (stat){
			stat = boost::none; //stat goes back to waiting state
			return;//ah, someone calls me !
		}

	}
	//world.probe(root,tag);//active waiting: take much more resources
}



int main(int argc, char ** argv ){
    mpi::environment env(argc, argv);
	mpi::communicator world;

	int root = 0; //server's rank
	int log_server=1;
	int tag = 0; // the tag (or the port) used for communication
	int dest;

	int rank = world.rank(); //process's rank
	string message; // message received from master
	bool tmp;

	if (rank > 1){
		cout << "Slave rank:" << rank << endl;
		while (1){
			//waits for demand from master, which is not frequent
// 			inactive_wait(world, root);
			world.recv(root, tag, message);

			//Exit if receive "finish" from master
			if (message.find ("finish") != string::npos){
				cout << "slave:" << rank << " is quitting..." << endl;
				
				world.send(log_server, tag, -1);
				break;
			}
			
			cout << "slave:" << rank << " received:" << message << endl;

			world.send(log_server, tag, rank); //try to get a place

			//waits for a response, might takes a while as server takes care of someone else
// 			inactive_wait(world, log_server);
			world.recv(log_server, tag, tmp );
			
			// Now I can send my message
			cout << "slave " << rank << " is sending to log server:" << message << endl;
			world.send(log_server,tag, message);
		}
	}
    return 0;
};

