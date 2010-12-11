// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// STDAIR
#include <stdair/bom/SegmentCabin.hpp>
// AIRSCHED
#include <airsched/bom/SegmentCabinStruct.hpp>

namespace AIRSCHED {

  // //////////////////////////////////////////////////////////////////////
  const std::string SegmentCabinStruct::describe() const {
    std::ostringstream ostr;
    ostr << "        " << _cabinCode << " " << _classes;
    return ostr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  void SegmentCabinStruct::fill (stdair::SegmentCabin& ioSegmentCabin) const {  
  }

}
