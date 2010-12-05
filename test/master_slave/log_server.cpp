/*
* Object: Make a simple log server
* Utilisation: Run log_server_client.sh
* Problem: avoid competitive sending, i.d 2 slaves send in a same time
* Solution: Server takes care of slave one by one
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
	int log_server=1;
	int tag = 0; // the tag (or the port) used for communication
	int dest;

	int rank = world.rank(); //process's rank
	int nbSlave = world.size() -2; //number of slaves
	string message; // message received from client
	bool yes=true;

	int nb_of_quit_signal = 0;

	if (rank==log_server){
		cout << "Log server rank:" << rank << endl;
		cout << "=============================================================" << endl;

		while (1){
			//waits for a demand, might takes a while as client aren't always gossip
// 			inactive_wait(world);
			world.recv(mpi::any_source,tag,dest);

			// Exit if nb_of_quit_signal = nbSlave
			if (dest == -1){
				cout << "log server receive quit signal from 1 slave... " << endl;
				nb_of_quit_signal ++;
				cout << "nb of quit signal:" << nb_of_quit_signal << endl;
				if (nb_of_quit_signal == nbSlave)
					break;

				cout << "still " << nbSlave - nb_of_quit_signal << " quit signal to wait...." << endl;
				continue;
			}
			
			cout << "server received from client: " << dest << endl ;
			world.send(dest,tag,yes); //ok, you can do it

			//  waiting for message from client
// 			inactive_wait(world);
			world.recv(dest,tag,message);

			logToFile("server.log", message);
			cout << "server received: " << message << endl ;
		}
	}

	return 0;
};
