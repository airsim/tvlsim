// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <iostream>
#include <sstream>
// Simlfs
#include <simlfs/basic/BasConst_SIMLFS_Service.hpp>
#include <simlfs/service/SIMLFS_ServiceContext.hpp>

namespace SIMLFS {
  
  // //////////////////////////////////////////////////////////////////////
  SIMLFS_ServiceContext::
  SIMLFS_ServiceContext (const SIMLFS_ServiceContext&) {
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
    oStr << "SIMLFS_ServiceContext: " << std::endl;
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string SIMLFS_ServiceContext::display() const {
    std::ostringstream oStr;
    oStr << shortDisplay();
    return oStr.str();
  }

}
