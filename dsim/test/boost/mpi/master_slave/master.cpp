/*
 * Object: Make a simple master-slave program
 * Utilisation: User tape a number, master send it to a slave (he is chosen based on the number of slave ), that slave return the square of that number to master. 
 * 
 */

#include <boost/mpi.hpp>
#include <iostream>
#include <string>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include<stdio.h>

using namespace std;
namespace mpi=boost::mpi;

int main(int argc, char ** argv){
    mpi::environment env(argc, argv);
    mpi::communicator world;

    int root = 0; //master's rank
    int tag = 0; // the tag (or the port) used for communication
    int num ; //number taken in by user
    int res;  // result sent by slave
    int dest; //index of slave to which command is sent

    int rank = world.rank(); //process's rank 
    int nbslave = world.size() -1 ; //number of slave(total number of process - master)
    string message; //for receiving news from slave

    if (rank==0){
        cout << "We have " << nbslave << " slaves who work" << endl;
        //cout << rank << "; pid = " << getpid() << endl;
        cout << "Be patient & type -1 for quitting the program\n";
        cout << "=============================================================" << endl;

        while(1){

            cout << "A number, plz..\n" ;
            cin >> num  ;
            if (num == -1){
                cerr << "Bye bye\n";
                mpi::environment::abort(0);//Abort all the process   
            } else{
                dest = num % nbslave  + 1;
                world.send(dest, tag, num);
            };
            world.recv(dest, tag, message);
            cout << message ;
            world.recv(dest , tag , res);
            cout << num << "* "<< num << "=" << res << endl;
            cout << "=============================================================" << endl;
        };
    }
    return 0;
};
