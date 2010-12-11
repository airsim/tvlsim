// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// STDAIR
#include <stdair/bom/SegmentDate.hpp>
// AIRINV
#include <airinv/bom/SegmentStruct.hpp>

namespace AIRINV {

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
    // Set the Boarding Date
    ioSegmentDate.setBoardingDate (_offDate);
    // Set the Boarding Time
    ioSegmentDate.setBoardingTime (_boardingTime);
    // Set the Off Date
    ioSegmentDate.setOffDate (_offDate);
    // Set the Off Time
    ioSegmentDate.setOffTime (_offTime);
    // Set the Elapsed Time
    ioSegmentDate.setElapsedTime (_elapsed);
  }

}
