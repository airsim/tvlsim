// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <iostream>
#include <sstream>
// Avlcal
#include <avlcal/basic/BasConst_AVLCAL_Service.hpp>
#include <avlcal/service/AVLCAL_ServiceContext.hpp>

namespace AVLCAL {

  // //////////////////////////////////////////////////////////////////////
  AVLCAL_ServiceContext::AVLCAL_ServiceContext ()
    : _airlineCode (DEFAULT_AIRLINE_CODE) {
  }

  // //////////////////////////////////////////////////////////////////////
  AVLCAL_ServiceContext::
  AVLCAL_ServiceContext (const AirlineCode_T& iAirlineCode)
    : _airlineCode (iAirlineCode) {
  }

  // //////////////////////////////////////////////////////////////////////
  AVLCAL_ServiceContext::~AVLCAL_ServiceContext() {
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string AVLCAL_ServiceContext::shortDisplay() const {
    std::ostringstream oStr;
    oStr << "AVLCAL_ServiceContext: " << std::endl
         << "Airline code: " << _airlineCode
         << std::endl;
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string AVLCAL_ServiceContext::display() const {
    std::ostringstream oStr;
    oStr << shortDisplay();
    return oStr.str();
  }

}
