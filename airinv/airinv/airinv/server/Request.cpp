// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// AirInv
#include <airinv/server/Request.hpp>

namespace AIRINV {

  // //////////////////////////////////////////////////////////////////////
  bool Request::parseFlightDate () {
    bool hasBeenSuccessfull = false;

    //
    _airlineCode = "BA";
    _flightNumber = 341;
    _departureDate = stdair::Date_T (2010, 04, 20);
    
    //
    hasBeenSuccessfull = true;
    
    return hasBeenSuccessfull;
  }
  
}
