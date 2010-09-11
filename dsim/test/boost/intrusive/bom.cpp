// STL
#include <cassert>
#include <sstream>
#include <string>
#include <vector>
// Boost
#include <boost/intrusive/list.hpp>
// Local
#include <test/boost/intrusive/FlightDate.hpp>

/** Alias for the boost::intrusive namespace. */
namespace bi = boost::intrusive;



// /////////////////////////// M A I N /////////////////////////
int main (int argc, char* argv[]) {

  // Create several FlightDate objects, each one with a different value
  std::vector<intrusive::FlightDate> flightDateList;
  for (int i = 0; i < 100; ++i) {
    flightDateList.push_back (intrusive::FlightDate (i));
  }

  intrusive::FlightDateChildren flightDateChildren;

  // Now insert them in the same order as in vector in the member hook list
  for (std::vector<intrusive::FlightDate>::iterator it (flightDateList.begin()),
         itend (flightDateList.end()); it != itend; ++it) {
    flightDateChildren.push_back (*it);
  }

  // Now test lists
  {
    intrusive::FlightDateChildren::iterator mit (flightDateChildren.begin()),
      mitend (flightDateChildren.end());
    std::vector<intrusive::FlightDate>::iterator it (flightDateList.begin()),
      itend (flightDateList.end());

    // Test the objects inserted in the member hook list
    for (it = flightDateList.begin(); it != itend; ++it, ++mit) {
      if (&*mit != &*it) {
        return 1;
      }
    }
  }

  return 0;
}
