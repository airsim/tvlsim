// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// STDAIR
#include <stdair/bom/SegmentCabin.hpp>
// AIRINV
#include <airinv/bom/SegmentCabinStruct.hpp>

namespace AIRINV {
    
  // //////////////////////////////////////////////////////////////////////
  const std::string SegmentCabinStruct::describe() const {
    std::ostringstream ostr;
    ostr << "        " << _cabinCode;

    if (_classes.empty() == false) {
      ostr << " " << _classes;
    }
    
    ostr << ", " << _nbOfBookings
         << std::endl;

    for (FareFamilyStructList_T::const_iterator itFF = _fareFamilies.begin();
         itFF != _fareFamilies.end(); ++itFF) {
      const FareFamilyStruct& lFF = *itFF;
      ostr << lFF.describe();
    }
    if (_fareFamilies.empty() == false) {
      ostr << std::endl;
    }

    for (BookingClassStructList_T::const_iterator itBkgClass= _classList.begin();
         itBkgClass != _classList.end(); ++itBkgClass) {
      const BookingClassStruct& lBkgClass = *itBkgClass;
      ostr << lBkgClass.describe();
    }
    if (_classList.empty() == false) {
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
