// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// StdAir
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/bom/BookingClass.hpp>
// AirInv
#include <airinv/bom/BookingClassStruct.hpp>

namespace AIRINV {

  // //////////////////////////////////////////////////////////////////////
  BookingClassStruct::BookingClassStruct () {
  }
  
  // //////////////////////////////////////////////////////////////////////
  stdair::ClassCode_T BookingClassStruct::getFullSubclassCode() const {
    std::ostringstream oStr;
    oStr << _classCode << _subclassCode;
    return oStr.str();
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string BookingClassStruct::describe() const {
    std::ostringstream ostr;
    ostr << "            " << _classCode << _subclassCode
         << " (" << _parentClassCode << _parentSubclassCode << ")"
         << ", " << _cumulatedProtection << ":" << _protection
         << ", " << _nego
         << ", " << _noShowPercentage << ":" << _overbookingPercentage
         << ", " << _nbOfBookings << ":" << _nbOfGroupBookings
         << ":" << _nbOfPendingGroupBookings << ":" << _nbOfStaffBookings
         << ":" << _nbOfWLBookings << ":" << _etb
         << ", " << _netClassAvailability  << ":" << _segmentAvailability
         << ":" << _netRevenueAvailability
         << std::endl;
    return ostr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  void BookingClassStruct::fill (stdair::BookingClass& ioBookingClass) const {
    // Set the Yield Range Upper Value
    // ioBookingClass.setYieldRangeValue (_yieldRangeUpperValue);

    // Set the Availability
    // ioBookingClass.setAvailability (_availability);

    // Set the number of seats
    // ioBookingClass.setNbOfSeats (_nbOfSeats);
      
    // Set the Seat Index
    // ioBookingClass.setSeatIndex (_seatIndex);
  }

}
