// STL
#include <iostream>
#include <algorithm>
// Boost Date-Time
#include <boost/date_time/gregorian/gregorian.hpp>

// using namespace boost;
namespace bd = boost::gregorian;

///////////////////////////////////////////////////////////////////////////////
// main
int main (int argc, char* argv[]) {

  //
  bd::date d1a (2002, bd::Feb, 24);
  bd::date d1b (bd::from_string ("2002-Feb-24"));

  std::cout << "Date 1 == 24-Feb-2002: " << std::endl
            << "[1a] " << d1a << "; [1b] " << d1b
            << std::endl;
  
  return 0;
}
