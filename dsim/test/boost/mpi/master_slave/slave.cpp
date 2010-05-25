#include <boost/mpi.hpp>
#include <iostream>
#include <string>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include<stdio.h>


using namespace std;
namespace mpi=boost::mpi;

int main(int argc, char ** argv ){
    mpi::environment env(argc, argv);
    mpi::communicator world;

    int root = 0, tag = 0, num, res;
    int rank = world.rank();//processus number 
    string message ; // for informing the master 

    //for veriying that master & slave use the same communicator
    //if (rank==1)
        //cout << "We have " << world.size()  -1 << " slaves who work" << endl;
   
    boost::optional<mpi::status> stat = boost::none;//stat is not initialised

    if (rank > 0){
        //cout << rank << "; pid = " << getpid() << endl;
        int r; //number sent by master

        while (1){

            //inactive waiting: wake up every 1s for verifying if there is a message
            while(1){
                stat = world.iprobe(root, tag); //inactive waiting

                //usleep(100);//we have the result faster but it takes more CPU 
                sleep(1); //better choice

                if (stat)
                    break;//ah, I have message !
            }
            //world.probe(root,tag);//active waiting: take much more resources

            world.recv(root,tag, r );
            
            std::stringstream out;
            out << "Slave " << rank <<  " has received :" << r << endl; 
            message = out.str();
            world.send(root,tag, message);

            res = r * r;
            world.send(root, tag ,res );//return the result to master
            
            stat = boost::none; //stat goes back to waiting state  
        }
    }
    return 0;
};

