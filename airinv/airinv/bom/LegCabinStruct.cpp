// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// StdAir
#include <stdair/bom/LegCabin.hpp>
// AirInv
#include <airinv/bom/LegCabinStruct.hpp>

namespace AIRINV {

  // //////////////////////////////////////////////////////////////////////
  const std::string LegCabinStruct::describe() const {
    std::ostringstream ostr;
    ostr << "        " << _cabinCode << ", " << _saleableCapacity
         << ", " << _adjustment  << ", " << _dcsRegrade
         << ", " << _au << ", " << _avPool
         << ", " << _upr << ", " << _nbOfBookings << ", " << _nav
         << ", " << _gav << ", " << _acp << ", " << _etb
         << ", " << _staffNbOfBookings << ", " << _wlNbOfBookings
         << ", " << _groupNbOfBookings
         << std::endl;

    for (BucketStructList_T::const_iterator itBucket = _bucketList.begin();
         itBucket != _bucketList.end(); ++itBucket) {
      const BucketStruct& lBucket = *itBucket;
      ostr << lBucket.describe();
    }
    if (_bucketList.empty() == false) {
      ostr << std::endl;
    }
    return ostr.str();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void LegCabinStruct::fill (stdair::LegCabin& ioLegCabin) const {
    // Set the Capacity
    ioLegCabin.setCapacities (_saleableCapacity);
  }

}
