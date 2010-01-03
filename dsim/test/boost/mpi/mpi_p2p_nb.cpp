// STL
#include <iostream>
#include <string>
// Boost.MPI
#include <boost/mpi.hpp>
#include <boost/serialization/string.hpp>


// ///////////////////////// M A I N ///////////////////////////
int main (int argc, char* argv[]) {
  boost::mpi::environment env (argc, argv);
  boost::mpi::communicator world;

  if (world.rank() == 0) {
    boost::mpi::request reqs[2];
    std::string msg;
    const std::string outMsg = "Hello";
    reqs[0] = world.isend (1, 0, outMsg);
    reqs[1] = world.irecv (1, 1, msg);
    boost::mpi::wait_all (reqs, reqs+2);

    std::cout << msg << "!" << std::endl;
    
  } else {
    boost::mpi::request reqs[2];
    std::string msg;
    const std::string outMsg = "world";
    reqs[0] = world.irecv (0, 0, msg);
    reqs[1] = world.isend (0, 1, outMsg);
    boost::mpi::wait_all (reqs, reqs+2);
    
    std::cout << msg << ", ";
    // std::cout.flush();
  }

  return 0;
}
