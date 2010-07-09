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

int main(int argc, char ** argv ){
    mpi::environment env(argc, argv);
	mpi::communicator world;

	int root = 0; //server's rank
	int tag = 0; // the tag (or the port) used for communication
	int dest;

	int rank = world.rank(); //process's rank
	string message; // message received from client
	bool tmp;

	if (rank > 0){
		ostringstream oss;
		oss << rank << " says alo";
		message = oss.str();

		while (1){
			world.send(root, tag, rank); //try to get a place

			//waits for a response, might takes a while as server takes care of someone else
			inactive_wait(world);
			world.recv(root, tag, tmp );

			// Now I can send my message
			cout << "sending :" << message << endl;
			world.send(root,tag, message);

			sleep (1); //not good to be too actif ...
		}
	}
    return 0;
};

