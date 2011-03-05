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
  AIRINV_Master_ServiceContext::AIRINV_Master_ServiceContext()
   : _ownStdairService (false) {
  }

  // //////////////////////////////////////////////////////////////////////
  AIRINV_Master_ServiceContext::~AIRINV_Master_ServiceContext() {
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string AIRINV_Master_ServiceContext::shortDisplay() const {
    std::ostringstream oStr;
    oStr << "AIRINV_Master_ServiceContext -- Owns StdAir service: "
         << _ownStdairService;
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string AIRINV_Master_ServiceContext::display() const {
    std::ostringstream oStr;
    oStr << shortDisplay();
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string AIRINV_Master_ServiceContext::describe() const {
    return shortDisplay();
  }

  // //////////////////////////////////////////////////////////////////////
  void AIRINV_Master_ServiceContext::reset() {
    if (_ownStdairService == true) {
      _stdairService.reset();
    }
  }

}
