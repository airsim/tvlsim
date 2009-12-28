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
    world.send (1, 0, std::string("Hello"));
    
    std::string msg;
    world.recv(1, 1, msg);
    
    std::cout << msg << "!" << std::endl;
    
  } else {
    std::string msg;
    world.recv (0, 0, msg);
    
    std::cout << msg << ", ";
    std::cout.flush();
    
    world.send (0, 1, std::string ("world"));
  }

  return 0;
}
