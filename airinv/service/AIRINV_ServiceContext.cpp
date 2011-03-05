// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// AirInv
#include <airinv/basic/BasConst_AIRINV_Service.hpp>
#include <airinv/service/AIRINV_ServiceContext.hpp>

namespace AIRINV {

  // //////////////////////////////////////////////////////////////////////
  AIRINV_ServiceContext::AIRINV_ServiceContext() : _ownStdairService (false) {
  }

  // //////////////////////////////////////////////////////////////////////
  AIRINV_ServiceContext::~AIRINV_ServiceContext() {
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string AIRINV_ServiceContext::shortDisplay() const {
    std::ostringstream oStr;
    oStr << "AIRINV_ServiceContext -- Owns StdAir service: "
         << _ownStdairService;
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string AIRINV_ServiceContext::display() const {
    std::ostringstream oStr;
    oStr << shortDisplay();
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string AIRINV_ServiceContext::describe() const {
    return shortDisplay();
  }

  // //////////////////////////////////////////////////////////////////////
  void AIRINV_ServiceContext::reset() {
    if (_ownStdairService == true) {
      _stdairService.reset();
    }
  }

}
