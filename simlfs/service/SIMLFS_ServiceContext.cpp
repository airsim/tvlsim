// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <istream>
#include <sstream>
// Standard Airline Object Model
#include <stdair/STDAIR_Service.hpp>
#include <stdair/service/Logger.hpp>
// SimLFS
#include <simlfs/basic/BasConst_SIMLFS_Service.hpp>
#include <simlfs/service/SIMLFS_ServiceContext.hpp>

namespace SIMLFS {
  
  // //////////////////////////////////////////////////////////////////////
  SIMLFS_ServiceContext::SIMLFS_ServiceContext (const SIMLFS_ServiceContext&) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  SIMLFS_ServiceContext::SIMLFS_ServiceContext () {
  }

  // //////////////////////////////////////////////////////////////////////
  SIMLFS_ServiceContext::~SIMLFS_ServiceContext() {
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string SIMLFS_ServiceContext::shortDisplay() const {
    std::ostringstream oStr;
    oStr << "SIMLFS_ServiceContext: no specific key";
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string SIMLFS_ServiceContext::display() const {
    std::ostringstream oStr;
    oStr << shortDisplay();
    return oStr.str();
  }

}
