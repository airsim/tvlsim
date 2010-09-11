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
/** Main.
    <br>Run with the following command:
    <tt>make check && ./bom && echo "Success"</tt>
    <br>To run the program with Valgrind, type:
    <tt>libtool --mode=execute valgrind --leak-check=full ./bom</tt>
*/
int main (int argc, char* argv[]) {

  //
  typedef std::vector<stdair::FlightDate*> FlightDateVector_T;

  // Standard STL container
  FlightDateVector_T lFlightDateVector;
  
  // Create several FlightDate objects, each one with a different value
  for (int idx = 0; idx < 100; ++idx) {
    stdair::FlightDate* lFlightDate_ptr = new stdair::FlightDate (idx);
    assert (lFlightDate_ptr != NULL);
    
    lFlightDateVector.push_back (lFlightDate_ptr);
  }

  // (Boost) Intrusive container
  stdair::FlightDateChildren lFlightDateChildren;

  // Now insert them in the same order as in vector in the member hook list
  for (FlightDateVector_T::iterator itFlight (lFlightDateVector.begin()),
         itend (lFlightDateVector.end()); itFlight != itend; ++itFlight) {
    stdair::FlightDate* lFlightDate_ptr = *itFlight;
    assert (lFlightDate_ptr != NULL);

    lFlightDateChildren.push_back (*lFlightDate_ptr);
  }

  // Now test lists
  {
    stdair::FlightDateChildren::iterator mit (lFlightDateChildren.begin()),
      mitend (lFlightDateChildren.end());
    FlightDateVector_T::iterator itFlight (lFlightDateVector.begin()),
      itend (lFlightDateVector.end());

    // Test the objects inserted in the member hook list
    for (itFlight = lFlightDateVector.begin();
         itFlight != itend; ++itFlight, ++mit) {
      stdair::FlightDate* lFlightDate_ptr = *itFlight;
      assert (lFlightDate_ptr != NULL);

      if (&*mit != lFlightDate_ptr) {
        return 1;
      }
    }
  }

  // Now, test iterator_to()
  stdair::FlightDateChildren::iterator itChild (lFlightDateChildren.begin());
  for (int idx = 0; idx < 100; ++idx, ++itChild) {
    stdair::FlightDate* lFlightDate_ptr = lFlightDateVector.at(idx);
    assert (lFlightDate_ptr != NULL);

    if (lFlightDateChildren.iterator_to (*lFlightDate_ptr) != itChild ||
        stdair::FlightDateChildren::s_iterator_to(*lFlightDate_ptr) != itChild){
      return 1;
    }
  }

  /** Some memory cleaning.
      <br>Note: the FlightDate objects cannot be simply deleted (with the
      delete opearator).
      <br>See also, for more details:
      - http://www.boost.org/doc/libs/1_44_0/doc/html/intrusive/usage.html#intrusive.usage.usage_lifetime
      - http://www.boost.org/doc/libs/1_44_0/doc/html/intrusive/erasing_and_disposing.html
  */
  lFlightDateChildren.clear_and_dispose (delete_disposer<stdair::FlightDate>());
  
  return 0;
}
