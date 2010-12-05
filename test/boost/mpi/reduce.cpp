/** testing reduce funtion of boost::mpi  */
#include<iostream>
#include<boost/mpi.hpp>
#include<cstdlib>
#include<string>

namespace mpi=boost::mpi;
using namespace std;

int petit(int a, int b){
     if (a<b)
         return a;
     else
         return b;
};
int main(int argc, char ** argv){
    mpi::environment env(argc, argv);
    mpi::communicator world;
    int rank = world.rank();
    srand(time(NULL) + rank);
    int val = rand();
    cout << "processus #" << rank<< "   val=" << val << endl;
    if (rank==0){
        int min ;
        reduce(world, val, min, mpi::minimum<int>(),0);
        //reduce(world, val, min, petit(),0);
        cout << "min=" << min << endl;
        world.barrier();
    }
    else{
        reduce(world, val, mpi::minimum<int>(), 0);
    }

}

