// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <iostream>
#include <sstream>
// Simfqt
#include <simfqt/basic/BasConst_SIMFQT_Service.hpp>
#include <simfqt/service/SIMFQT_ServiceContext.hpp>

namespace SIMFQT {
  
  // //////////////////////////////////////////////////////////////////////
  SIMFQT_ServiceContext::
  SIMFQT_ServiceContext (const SIMFQT_ServiceContext&) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  SIMFQT_ServiceContext::SIMFQT_ServiceContext () {
  }

  // //////////////////////////////////////////////////////////////////////
  SIMFQT_ServiceContext::~SIMFQT_ServiceContext() {
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string SIMFQT_ServiceContext::shortDisplay() const {
    std::ostringstream oStr;
    oStr << "SIMFQT_ServiceContext: " << std::endl;
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string SIMFQT_ServiceContext::display() const {
    std::ostringstream oStr;
    oStr << shortDisplay();
    return oStr.str();
  }

}
