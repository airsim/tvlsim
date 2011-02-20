// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
#include <sstream>
// StdAir
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/service/Logger.hpp>
// AIRSCHED
#include <airsched/command/Simulator.hpp>

namespace AIRSCHED {

  // //////////////////////////////////////////////////////////////////////
  void Simulator::simulate (stdair::BomRoot& ioBomRoot) {

    // Delegate to the dedicated StdAir utility class
    // std::ostringstream oStream;
    // stdair::BomManager::display (oStream, ioBomRoot);

    // DEBUG
    // STDAIR_LOG_DEBUG ("BOM Tree: ");
    // STDAIR_LOG_DEBUG (oStream.str());

    // TODO: do not hardcode the booking request (get it from the
    // demand generation module instead).
    // stdair::BookingRequestStruct ("LHR", "JFK", stdair::Date_T (2009, 01, 16),
    //                               stdair::DEFAULT_DATETIME, "Y", 1);
  }

}
