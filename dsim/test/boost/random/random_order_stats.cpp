#include <cassert>
#include <iostream>
#include <sstream>
// Boost.Random
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/generator_iterator.hpp>

// Type definition for a random number generator (mt19937).
typedef boost::minstd_rand base_generator_type;

// ////////////////////////////////////////////////////
void gen_save (std::string& ioState, const base_generator_type& iGenerator) {
  std::ostringstream oStr;
  oStr << iGenerator;
  ioState = oStr.str();
}

// ////////////////////////////////////////////////////
void gen_restore (const std::string& iState, base_generator_type& ioGenerator) {
  std::istringstream iStr (iState);
  iStr >> ioGenerator;
}

// /////////////// M A I N /////////////////
int main() {
  // Define a random number generator and initialize it with a reproducible
  // seed.
  // <br>The seed is unsigned, otherwise the wrong overload may be selected
  // when using mt19937 as the base_generator_type.
  base_generator_type generator (42u);

  std::cout << "10 samples of a uniform distribution in [0..1):" << std::endl;

  // Store current formatting flags of std::cout
  std::ios::fmtflags oldFlags = std::cout.flags();

  std::cout.setf (std::ios::fixed);

  // Draw randomly
  double lDrawnNumber = 0.0;
  std::string lGeneratorState;
  double average = 0.0;
  const unsigned int lNbOfDraws = 100;
  for (unsigned int i = 1; i <= lNbOfDraws; ++i) {
    // Change the lower bound of the range of the uniform distribution
    boost::uniform_real<> uni_dist (lDrawnNumber, 1);
    boost::variate_generator<base_generator_type&, 
                             boost::uniform_real<> > uni_gen (generator,
                                                              uni_dist);

    // Restore the generator state (seed)
    if (i != 1) {
      // Restore the generator state
      gen_restore (lGeneratorState, generator);
    }
    
    // Draw a random number corresponding to the uniform [x,1) range
    lDrawnNumber = uni_gen();
    average += lDrawnNumber / lNbOfDraws;

    // Save the generator state (seed)
    gen_save (lGeneratorState, generator);
    
    // DEBUG
    std::cout << "[" << i << "]: " << lDrawnNumber << std::endl;
  }

  // DEBUG
  std::cout << "Average on " << lNbOfDraws << " draws: " << average
            << std::endl;
  
  // Reset formatting flags of std::cout
  std::cout.flags (oldFlags);

  return 0;
}
