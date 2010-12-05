// STL
#include <cassert>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
// TraDemGen
#include <test/trademgen/EventStream.hpp>
#include <test/trademgen/CategoricalAttribute.hpp>

// ///////////// M A I N //////////////////
int main (int argc, char* const argv[]) {
  // input: seed, rate
  unsigned long int seed = 2;
  
  if (argc >= 2) {
    std::istringstream iStream (argv[1]);
    iStream >> seed;
  }
	
  // create event stream
  TRADEMGEN::EventStream e (seed);
  e.setKey("hello");
  e.setRate(2.0);
	
  // get rate
  // const double r = e.getRate();
  std::cout << "Seed: " << seed << std::endl << std::endl;
	
  // create instances
  for (int i=0; i<10; i++) {
    e.generateNext();
  }
	
  // display events
  e.displayAllEvents(std::cout);
	
	
  // /////////////////////////////////////////////////////
  // attributes
  std::map<int, float> M;
  M[1] = 0.1;
  M[17] = 0.7;
  M[77] = 0.2;
  TRADEMGEN::CategoricalAttribute C (M);
	
  return 0;
}
