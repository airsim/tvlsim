// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// Airinv
#include <airinv/basic/BasConst_AIRINV_Service.hpp>
#include <airinv/service/AIRINV_Master_ServiceContext.hpp>

namespace AIRINV {

  // //////////////////////////////////////////////////////////////////////
  AIRINV_Master_ServiceContext::AIRINV_Master_ServiceContext () {
  }

  // //////////////////////////////////////////////////////////////////////
  AIRINV_Master_ServiceContext::~AIRINV_Master_ServiceContext() {
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string AIRINV_Master_ServiceContext::shortDisplay() const {
    std::ostringstream oStr;
    oStr << "AIRINV_Master_ServiceContext: " << std::endl;
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string AIRINV_Master_ServiceContext::display() const {
    std::ostringstream oStr;
    oStr << shortDisplay();
    return oStr.str();
  }

}
