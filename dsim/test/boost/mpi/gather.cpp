/** Testing gather function in boost::mpi  */
#include <boost/mpi.hpp>
#include <iostream>
#include <string>
#include <unistd.h>
#include <time.h>

namespace mpi=boost::mpi;
using namespace std;


int main(int argc, char ** argv ){
    /*initialisation of mpi*/
    mpi::environment env(argc, argv);

    mpi::communicator world;
    int rank = world.rank();//processus number 

    srand(time(NULL)+ rank);//initialisation of random machine
    int val = rand();

    cout << "processus #" << rank << "   val=" << val <<endl;
    if (rank==0){
        vector<int>  v ;
        gather(world, val, v,0); //gather all values of val in v in processus 0

        /*for testing*/
        for (int i = 0; i< v.size(); i++){
            cout << "val[" << i << "]=" << v[i] << endl;
        }
    }
    else{
        gather(world, val, 0);//send val to processus 0
    }
    sleep(3);//pause a little for viewing in another machine
} 




