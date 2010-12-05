// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <ostream>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
// TraDemGen
#include <trademgen/bom/BomManager.hpp>

namespace TRADEMGEN {

  // ////////////////////////////////////////////////////////////////////
  void BomManager::
  csvDisplay (std::ostream& oStream,
              const stdair::BookingRequestStruct& iBookingRequest) {
    // Store current formatting flags of the given output stream
    std::ios::fmtflags oldFlags = oStream.flags();

    /**
      #id, request_date (YYMMDD), request_time (HHMMSS), POS (three-letter code),
       origin (three-letter code), destination (three-letter code),
       preferred departure date (YYMMDD), preferred departure time (HHMM),
       min departure time (HHMM), max departure time (HHMM),
       preferred cabin (F/C/M), trip type (OW/RO/RI), duration of stay (days),
       frequent flyer tier (G/S/K/N), willingness-to-pay (SGD),
       disutility per stop (SGD), preferred arrival date (YYMMDD),
       preferred arrival time (HHMM), value of time (SGD per hour)

      #Preferred cabin: F=First, C=Business M=Economy
      #Trip type: OW=One-way, RO=Round-trip outbound portion,
        RI=Round-trip inbound portion
      #Duration of stay: irrelevant in case of one-way, but set to 0
      #Frequent-flyer tier: G=Gold, S=Silver, K=Basic (Krisflyer), N=None
    */

    // Request date&time
    const stdair::DateTime_T& lRequestDateTime =
      iBookingRequest.getRequestDateTime();
    intDisplay (oStream, lRequestDateTime.date().year());
    intDisplay (oStream, lRequestDateTime.date().month());
    intDisplay (oStream, lRequestDateTime.date().day());
    oStream << ", ";
    intDisplay (oStream, lRequestDateTime.time_of_day().hours());
    intDisplay (oStream, lRequestDateTime.time_of_day().minutes());
    intDisplay (oStream, lRequestDateTime.time_of_day().seconds());
    oStream << ", ";
    // POS
    oStream << iBookingRequest.getPOS() << ", ";
    // Origin
    oStream << iBookingRequest.getOrigin() << ", ";
    // Destination
    oStream << iBookingRequest.getDestination() << ", ";
    // Preferred departure date
    const stdair::Date_T& lPreferredDepartureDate =
      iBookingRequest.getPreferedDepartureDate();
    intDisplay (oStream, lPreferredDepartureDate.year());
    intDisplay (oStream, lPreferredDepartureDate.month());
    intDisplay (oStream, lPreferredDepartureDate.day());
    oStream << ", ";
    // Preferred departure time
    const stdair::Duration_T& lPreferredDepartureTime =
      iBookingRequest.getPreferredDepartureTime();
    intDisplay (oStream, lPreferredDepartureTime.hours());
    intDisplay (oStream, lPreferredDepartureTime.minutes());
    oStream << ", ";
    // MIN & MAX preferred departure time (hardcode)
    oStream << "0000, 2359, ";
    // Preferred cabin
    oStream << iBookingRequest.getPreferredCabin() << ", ";
    // Trip type
    oStream << iBookingRequest.getTripType() << ", ";
    // Duration of stay
    if (iBookingRequest.getTripType() == "OW") {
      oStream << "0, ";
    } else {
      oStream << iBookingRequest.getStayDuration() << ", ";
    }
    // Frequent flyer tier
    oStream << iBookingRequest.getFrequentFlyerType() << ", ";
    // Willingness-to-pay
    oStream << iBookingRequest.getWTP() << ", ";
    // Disutility per stop (hardcode to 100)
    oStream << "100, ";
    // Preferred arrival date (hardcode to the preferred departure date)
    intDisplay (oStream, lPreferredDepartureDate.year());
    intDisplay (oStream, lPreferredDepartureDate.month());
    intDisplay (oStream, lPreferredDepartureDate.day());
    oStream << ", ";
    // Preferred arrival time (hardcode to 23:55
    oStream << "2355, ";
    // Value of time
    oStream <<iBookingRequest.getValueOfTime() << std::endl;
    
    // Reset formatting flags of the given output stream
    oStream.flags (oldFlags);
  }

  // ////////////////////////////////////////////////////////////////////
  void BomManager::intDisplay (std::ostream& oStream, const int& iInt) {
    const int dInt = iInt - static_cast<int>(iInt/100)*100;
    if (dInt < 10) {
      oStream << "0" << dInt;
    } else {
      oStream << dInt;
    }
  }
  
}
