/** 
 * Send "alo alo " from process 0 to process 1
 * Just for testing if boost::mpi works
 * */
#include <boost/mpi.hpp>
#include <iostream>
#include <string>
using namespace std;
namespace mpi=boost::mpi;

int main(int argc, char ** argv){
    mpi::environment env(argc, argv);
    mpi::communicator world;
	
    int rank = world.rank();
    string s;
    int tag = 10;
    if (rank == 0){
        int dest = 1;
        s = "alo alo?";
        world.send(dest, tag,s );//blocking communication
    }else if (rank==1){
        int source = 0;
        world.recv(source, tag, s);
        cout << s << endl;
    }else{
         return 0; 
    };
    return 0;
};

