// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/factory/FacBomManager.hpp>
#include <stdair/service/Logger.hpp>
// AirInv
#include <airinv/bom/DCPEventStruct.hpp>
#include <airinv/command/DCPEventGenerator.hpp>

namespace AIRINV {

  // //////////////////////////////////////////////////////////////////////
  void DCPEventGenerator::
  createDCPEvent (stdair::BomRoot& ioBomRoot,
                  DCPEventStruct& iDCPEventStruct) {

    // Set the airport-pair primary key.
    /*
    const stdair::AirportCode_T& lBoardPoint = iDCPEventStruct._origin;
    const stdair::AirportCode_T& lOffPoint = iDCPEventStruct._destination;
    */
    
    // Set the DCP date-period primary key.
    const stdair::Date_T& lDateRangeStart = iDCPEventStruct._dateRangeStart;
    const stdair::Date_T& lDateRangeEnd = iDCPEventStruct._dateRangeEnd;
    const stdair::DatePeriod_T lDatePeriod (lDateRangeStart, lDateRangeEnd); 

    // Set the DCP time-period primary key.
    /*
    const stdair::Time_T& lTimeRangeStart = iDCPEventStruct._timeRangeStart;
    const stdair::Time_T& lTimeRangeEnd = iDCPEventStruct._timeRangeEnd;
    */
    
    // Generate the DCPEvent
    const stdair::DayDuration_T& lAdvancePurchase = 
      iDCPEventStruct._advancePurchase;
    const stdair::SaturdayStay_T& lSaturdayStay = iDCPEventStruct._saturdayStay;
    const stdair::ChangeFees_T& lChangeFees = iDCPEventStruct._changeFees;
    const stdair::NonRefundable_T& lNonRefundable = 
      iDCPEventStruct._nonRefundable;
    const stdair::DayDuration_T& lMinimumStay = iDCPEventStruct._minimumStay;
    const stdair::Fare_T& lDCP = iDCPEventStruct._DCP; 

    // Generate Segment Features and link them to their DCPEvent
    stdair::ClassList_StringList_T::const_iterator lItCurrentClassCodeList =
      iDCPEventStruct._classCodeList.begin();

    const unsigned int lAirlineListSize = iDCPEventStruct.getAirlineListSize();
    const unsigned int lClassCodeListSize =
      iDCPEventStruct.getClassCodeListSize();
    assert (lAirlineListSize == lClassCodeListSize);

    iDCPEventStruct.beginClassCode();
    for (iDCPEventStruct.beginAirline();
         iDCPEventStruct.hasNotReachedEndAirline();
         iDCPEventStruct.iterateAirline()) {
      /*
      const stdair::AirlineCode_T& lAirlineCode =
        iDCPEventStruct.getCurrentAirlineCode();
      const std::string& lClassCodeList = iDCPEventStruct.getCurrentClassCode();
      iDCPEventStruct.iterateClassCode();
      */
    }
  }
        
}

