#ifndef __AIRINV_BOM_BOOKINGCLASSSTRUCT_HPP
#define __AIRINV_BOM_BOOKINGCLASSSTRUCT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <vector>
// STDAIR
#include <stdair/stdair_inventory_types.hpp>
#include <stdair/basic/StructAbstract.hpp>
// AIRINV
#include <airinv/AIRINV_Types.hpp>

// Forward declarations
namespace stdair {
  class BookingClass;
}

namespace AIRINV {

  /** Utility Structure for the parsing of BookingClass structures. */
  struct BookingClassStruct : public stdair::StructAbstract {
    // Attributes
    stdair::ClassCode_T _classCode;
    stdair::SubclassCode_T _subclassCode;
    stdair::ClassCode_T _parentClassCode;
    stdair::SubclassCode_T _parentSubclassCode;
    stdair::AuthorizationLevel_T _cumulatedProtection;
    stdair::AuthorizationLevel_T _protection;
    stdair::NbOfSeats_T _nego;
    stdair::OverbookingRate_T _noShowPercentage;
    stdair::OverbookingRate_T _overbookingPercentage;
    stdair::NbOfBookings_T _nbOfBookings;
    stdair::NbOfBookings_T _nbOfGroupBookings;
    stdair::NbOfBookings_T _nbOfPendingGroupBookings;
    stdair::NbOfBookings_T _nbOfStaffBookings;
    stdair::NbOfBookings_T _nbOfWLBookings;
    stdair::NbOfBookings_T _etb;
    stdair::Availability_T _netClassAvailability;
    stdair::Availability_T _segmentAvailability;
    stdair::Availability_T _netRevenueAvailability;

    /** Returns the concatenation of the class and subclass codes. */
    stdair::ClassCode_T getFullSubclassCode() const;

    /** Fill the BookingClass objects with the attributes of the
        BookingClassStruct. */
    void fill (stdair::BookingClass&) const;
      
    /** Give a description of the structure (for display purposes). */
    const std::string describe() const;

    /** Default Constructor. */
    BookingClassStruct();
  };

  /** List of BookingClass structures. */
  typedef std::vector<BookingClassStruct> BookingClassStructList_T;

}
#endif // __AIRINV_BOM_BOOKINGCLASSSTRUCT_HPP
