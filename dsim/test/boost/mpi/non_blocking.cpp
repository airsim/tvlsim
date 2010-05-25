/*
 * testing non-blocking communication function
 * */
#include<boost/mpi.hpp>
#include<iostream>
#include <unistd.h>

using namespace std;
namespace mpi=boost::mpi;

int main(int argc, char ** argv){
    mpi::environment env(argc, argv);
    mpi::communicator world;
    int rank = world.rank();
    string zero, one;
    int tag = 10;

    mpi::request req[2];
    if (rank == 0){
        int dest = 1;
        zero = "zero";
        req[0] = world.isend(dest, tag, zero);
        req[1] = world.irecv (dest, tag,one );
        mpi::wait_all(req, req+2);
        cout << rank << ":" << zero << one << endl;
    }else if (rank == 1){
        int src = 0;
        one = "one";
        req[0] = world.isend(src, tag, one);
        req[1] = world.irecv (src, tag,zero );
        mpi::wait_all(req, req+2);
        cout << rank << ":" << zero << one << endl;
    }else{
       /** Do nothing  */;
    };
    sleep(2);
} 

