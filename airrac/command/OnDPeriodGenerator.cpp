// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// BOOST
#include <boost/date_time/date_iterator.hpp>
// StdAir
#include <stdair/stdair_date_time_types.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/factory/FacBomManager.hpp>
#include <stdair/service/Logger.hpp>
// AIRSCHED
#include <airsched/bom/OnDPeriodStruct.hpp>
#include <airsched/command/OnDPeriodGenerator.hpp>

namespace AIRSCHED {
  // ////////////////////////////////////////////////////////////////////
  void OnDPeriodGenerator::
  createOnDPeriod (stdair::BomRoot& ioBomRoot,
                   const OnDPeriodStruct& iOnDPeriodStruct) {
  }
}
