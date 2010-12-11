// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// Airrac
#include <airrac/basic/BasConst_AIRRAC_Service.hpp>
#include <airrac/service/AIRRAC_ServiceContext.hpp>

namespace AIRRAC {

  // //////////////////////////////////////////////////////////////////////
  AIRRAC_ServiceContext::
  AIRRAC_ServiceContext (const stdair::AirlineCode_T& iAirlineCode,
                         stdair::YieldStore& ioYieldStore)
    : _airlineCode (iAirlineCode), _yieldStore (ioYieldStore) {
  }

  // //////////////////////////////////////////////////////////////////////
  AIRRAC_ServiceContext::~AIRRAC_ServiceContext() {
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string AIRRAC_ServiceContext::shortDisplay() const {
    std::ostringstream oStr;
    oStr << "AIRRAC_ServiceContext: " << std::endl
         << "Airline code: " << _airlineCode
         << std::endl;
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string AIRRAC_ServiceContext::display() const {
    std::ostringstream oStr;
    oStr << shortDisplay();
    return oStr.str();
  }

}
