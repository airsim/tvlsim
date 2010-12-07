// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// StdAir
#include <stdair/basic/BasConst_General.hpp>
//#include <stdair/bom/Bucket.hpp>
// AirInv
#include <airinv/bom/BucketStruct.hpp>

namespace AIRINV {

  // //////////////////////////////////////////////////////////////////////
  BucketStruct::BucketStruct () {
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string BucketStruct::describe() const {
    std::ostringstream ostr;
    ostr << "            " << _yieldRangeUpperValue << ":" << _availability
         << ":" << _nbOfSeats  << ":" << _seatIndex
         << std::endl;
    return ostr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  void BucketStruct::fill (stdair::Bucket& ioBucket) const {
    // Set the Yield Range Upper Value
    // ioBucket.setYieldRangeValue (_yieldRangeUpperValue);

    // Set the Availability
    // ioBucket.setAvailability (_availability);

    // Set the number of seats
    // ioBucket.setNbOfSeats (_nbOfSeats);
      
    // Set the Seat Index
    // ioBucket.setSeatIndex (_seatIndex);
  }

}
