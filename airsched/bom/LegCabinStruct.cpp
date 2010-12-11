// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// STDAIR
#include <stdair/bom/LegCabin.hpp>
// AIRSCHED
#include <airsched/bom/LegCabinStruct.hpp>

namespace AIRSCHED {

  // //////////////////////////////////////////////////////////////////////
  const std::string LegCabinStruct::describe() const {
    std::ostringstream ostr;
    ostr << "        " << _cabinCode << " " << _capacity << ", ";
    return ostr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  void LegCabinStruct::fill (stdair::LegCabin& ioLegCabin) const {
    // Set the Capacity
    ioLegCabin.setCapacities (_capacity);
  }

}
