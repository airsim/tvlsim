// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// StdAir
#include <stdair/bom/SegmentCabin.hpp>
// AirInv
#include <airinv/bom/SegmentCabinStruct.hpp>

namespace AIRINV {
    
  // //////////////////////////////////////////////////////////////////////
  const std::string SegmentCabinStruct::describe() const {
    std::ostringstream ostr;

    ostr << "        " << _cabinCode << ", " << _nbOfBookings << std::endl;

    for (FareFamilyStructList_T::const_iterator itFF = _fareFamilies.begin();
         itFF != _fareFamilies.end(); ++itFF) {
      const FareFamilyStruct& lFF = *itFF;
      ostr << lFF.describe();
    }
    if (_fareFamilies.empty() == false) {
      ostr << std::endl;
    }

    return ostr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  void SegmentCabinStruct::fill (stdair::SegmentCabin& ioSegmentCabin) const {
    // Set the total number of bookings
    // ioSegmentCabin.setNbOfBookings (_nbOfBookings);
  }

}
