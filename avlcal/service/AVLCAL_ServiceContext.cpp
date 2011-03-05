// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <istream>
#include <sstream>
// StdAir
#include <stdair/STDAIR_Service.hpp>
// Avlcal
#include <avlcal/basic/BasConst_AVLCAL_Service.hpp>
#include <avlcal/service/AVLCAL_ServiceContext.hpp>

namespace AVLCAL {

  // //////////////////////////////////////////////////////////////////////
  AVLCAL_ServiceContext::AVLCAL_ServiceContext ()
    : _ownStdairService (false), _airlineCode (DEFAULT_AIRLINE_CODE) {
  }

  // //////////////////////////////////////////////////////////////////////
  AVLCAL_ServiceContext::AVLCAL_ServiceContext (const AVLCAL_ServiceContext&) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  AVLCAL_ServiceContext::
  AVLCAL_ServiceContext (const stdair::AirlineCode_T& iAirlineCode)
    : _ownStdairService (false), _airlineCode (iAirlineCode) {
  }

  // //////////////////////////////////////////////////////////////////////
  AVLCAL_ServiceContext::~AVLCAL_ServiceContext() {
  }
  
  // ////////////////////////////////////////////////////////////////////
  stdair::STDAIR_Service& AVLCAL_ServiceContext::getSTDAIR_Service() const {
    assert (_stdairService != NULL);
    return *_stdairService;
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string AVLCAL_ServiceContext::shortDisplay() const {
    std::ostringstream oStr;
    oStr << "AVLCAL_ServiceContext -- Owns StdAir service: " << _ownStdairService;
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string AVLCAL_ServiceContext::display() const {
    std::ostringstream oStr;
    oStr << shortDisplay();
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string AVLCAL_ServiceContext::describe() const {
    return shortDisplay();
  }

  // ////////////////////////////////////////////////////////////////////
  void AVLCAL_ServiceContext::reset() {
    if (_ownStdairService == true) {
      _stdairService.reset();
    }
  }

}
