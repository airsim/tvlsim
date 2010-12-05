// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// STDAIR
#include <stdair/bom/SegmentDate.hpp>
// AIRSCHED
#include <airsched/bom/SegmentStruct.hpp>

namespace AIRSCHED {

  // //////////////////////////////////////////////////////////////////////
  const std::string SegmentStruct::describe() const {
    std::ostringstream ostr;
    ostr << "    " << _boardingPoint << " / "
         << boost::posix_time::to_simple_string(_boardingTime)
         << " -- " << _offPoint << " / "
         << boost::posix_time::to_simple_string(_offTime)
         << " --> "
         << boost::posix_time::to_simple_string(_elapsed)
         << std::endl;
    for (SegmentCabinStructList_T::const_iterator itCabin =
           _cabinList.begin(); itCabin != _cabinList.end(); itCabin++) {
      const SegmentCabinStruct& lCabin = *itCabin;
      ostr << lCabin.describe();
    }
    ostr << std::endl;
    
    return ostr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  void SegmentStruct::fill (stdair::SegmentDate& ioSegmentDate) const {
    // Note that some parameters (boarding date, boarding time, off
    // date, off time, elapsed time) are set by
    // SegmentDate::fillFromRouting() when the routing (with legs) is
    // built. So, it is useless to set those parameters here.

    // At that time, there are no other parameters.
  }

}
