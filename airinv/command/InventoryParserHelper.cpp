// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/service/Logger.hpp>
#include <stdair/stdair_exceptions.hpp>
// Airinv
#include <airinv/command/InventoryBuilder.hpp>
#include <airinv/command/InventoryParserHelper.hpp>

namespace AIRINV {

  namespace InventoryParserHelper {
      
    // //////////////////////////////////////////////////////////////////
    //  Semantic actions
    // //////////////////////////////////////////////////////////////////

    ParserSemanticAction::
    ParserSemanticAction (FlightDateStruct& ioFlightDate)
      : _flightDate (ioFlightDate) {
    }      

    // //////////////////////////////////////////////////////////////////
    storeSnapshotDate::
    storeSnapshotDate (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeSnapshotDate::operator() (iterator_t iStr,
                                        iterator_t iStrEnd) const {
      _flightDate._flightDate = _flightDate.getDate();
    }
      
    // //////////////////////////////////////////////////////////////////
    storeAirlineCode::
    storeAirlineCode (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeAirlineCode::operator() (iterator_t iStr,
                                       iterator_t iStrEnd) const {
      const stdair::AirlineCode_T lAirlineCode (iStr, iStrEnd);
      _flightDate._airlineCode = lAirlineCode;
                
      // As that's the beginning of a new flight, all the list must be reset
      // 1. Leg branch of the tree
      _flightDate._legList.clear();
      _flightDate._itLeg._cabinList.clear();
      _flightDate._itLegCabin._bucketList.clear();
      _flightDate._itBucket._yieldRangeUpperValue = 0.0;

      // 2. Segment branch of the tree
      _flightDate._segmentList.clear();
      _flightDate._itSegment._cabinList.clear();
      _flightDate._itSegmentCabin._classList.clear();
      _flightDate._itBookingClass._classCode = "";
    }
      
    // //////////////////////////////////////////////////////////////////
    storeFlightNumber::storeFlightNumber (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeFlightNumber::operator() (unsigned int iNumber) const { 
      _flightDate._flightNumber = iNumber;
    }

    // //////////////////////////////////////////////////////////////////
    storeFlightDate::storeFlightDate (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeFlightDate::operator() (iterator_t iStr,
                                      iterator_t iStrEnd) const {
      _flightDate._flightDate = _flightDate.getDate();
    }
      
    // //////////////////////////////////////////////////////////////////
    storeFlightTypeCode::storeFlightTypeCode (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeFlightTypeCode::operator() (iterator_t iStr,
                                          iterator_t iStrEnd) const {
      const std::string lFlightTypeCodeStr (iStr, iStrEnd);
      const FlightTypeCode lFlightTypeCode (lFlightTypeCodeStr);
      _flightDate._flightTypeCode = lFlightTypeCode.getCode();
      //STDAIR_LOG_DEBUG ("FlightType code: " << lFlightTypeCode);
    }

    // //////////////////////////////////////////////////////////////////
    storeFlightVisibilityCode::
    storeFlightVisibilityCode (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeFlightVisibilityCode::operator() (iterator_t iStr,
                                                iterator_t iStrEnd) const {
      const std::string lFlightVisibilityCodeStr (iStr, iStrEnd);
      const FlightVisibilityCode lFlightVisibilityCode(lFlightVisibilityCodeStr);
      _flightDate._flightVisibilityCode = lFlightVisibilityCode.getCode();
      //STDAIR_LOG_DEBUG ("FlightVisibility code: " << lFlightVisibilityCode);
    }

    // //////////////////////////////////////////////////////////////////
    storeLegBoardingPoint::
    storeLegBoardingPoint (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeLegBoardingPoint::operator() (iterator_t iStr,
                                            iterator_t iStrEnd) const {
      stdair::AirportCode_T lBoardingPoint (iStr, iStrEnd);

      // ///////////////////
      // If this is not the first leg-date of the flight-date,
      // the already parsed leg-date must be added to the flight-date.
      if (_flightDate._itLeg._cabinList.empty() == false) {
        _flightDate._itLegCabin._bucketList.push_back (_flightDate._itBucket);
        _flightDate._itLeg._cabinList.push_back (_flightDate._itLegCabin);
        _flightDate._legList.push_back (_flightDate._itLeg);
      }

      // As that's the beginning of a new leg-date,
      // (re-)initialise the leg-cabin branch of the tree
      _flightDate._itLeg._cabinList.clear();
      _flightDate._itLegCabin._cabinCode = "";
      _flightDate._itLegCabin._bucketList.clear();
      _flightDate._itBucket._yieldRangeUpperValue = 0.0;


      // ///////////////////
      // Set the (new) boarding point
      _flightDate._itLeg._boardingPoint = lBoardingPoint;
      
      // Add the airport code if it is not already stored in the airport lists
      _flightDate.addAirport (lBoardingPoint);
    }

    // //////////////////////////////////////////////////////////////////
    storeLegOffPoint::
    storeLegOffPoint (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeLegOffPoint::operator() (iterator_t iStr,
                                       iterator_t iStrEnd) const {
      stdair::AirportCode_T lOffPoint (iStr, iStrEnd);
      _flightDate._itLeg._offPoint = lOffPoint;

      // Add the airport code if it is not already stored in the airport lists
      _flightDate.addAirport (lOffPoint);
    }

    // //////////////////////////////////////////////////////////////////
    storeBoardingDate::storeBoardingDate (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeBoardingDate::operator() (iterator_t iStr,
                                        iterator_t iStrEnd) const {
      _flightDate._itLeg._boardingDate = _flightDate.getDate();
    }

    // //////////////////////////////////////////////////////////////////
    storeBoardingTime::storeBoardingTime (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeBoardingTime::operator() (iterator_t iStr,
                                        iterator_t iStrEnd) const {
      _flightDate._itLeg._boardingTime = _flightDate.getTime();
        
      // Reset the number of seconds
      _flightDate._itSeconds = 0;

      // Reset the date off-set
      _flightDate._dateOffSet = 0;
    }

    // //////////////////////////////////////////////////////////////////
    storeOffDate::storeOffDate (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeOffDate::operator() (iterator_t iStr, iterator_t iStrEnd) const {
      _flightDate._itLeg._offDate = _flightDate.getDate();
    }

    // //////////////////////////////////////////////////////////////////
    storeOffTime::storeOffTime (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeOffTime::operator() (iterator_t iStr, iterator_t iStrEnd) const {
      _flightDate._itLeg._offTime = _flightDate.getTime();
        
      // Reset the number of seconds
      _flightDate._itSeconds = 0;
    }

    // //////////////////////////////////////////////////////////////////
    storeLegCabinCode::storeLegCabinCode (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeLegCabinCode::operator() (char iChar) const { 

      // ///////////////////
      // If this is not the first leg-cabin of the leg-date,
      // the already parsed leg-cabin must be added to the leg-date.
      if (_flightDate._itLegCabin._cabinCode != "") {
        if (_flightDate._itLegCabin._bucketList.empty() == false) {
          _flightDate._itLegCabin._bucketList.push_back (_flightDate._itBucket);
        }
        _flightDate._itLeg._cabinList.push_back (_flightDate._itLegCabin);
      }

      // (Re-)initialise the leg-cabin branch of the tree
      _flightDate._itLegCabin._bucketList.clear();
      _flightDate._itBucket._yieldRangeUpperValue = 0.0;

      
      // ///////////////////
      _flightDate._itLegCabin._cabinCode = iChar; 
      //std::cout << "Cabin code: " << iChar << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeSaleableCapacity::
    storeSaleableCapacity (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeSaleableCapacity::operator() (double iReal) const { 
      _flightDate._itLegCabin._saleableCapacity = iReal; 
      //std::cout << "Saleable capacity: " << iReal << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeAU::storeAU (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeAU::operator() (double iReal) const {
      _flightDate._itLegCabin._au = iReal; 
      //std::cout << "AU: " << iReal << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeUPR::storeUPR (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeUPR::operator() (double iReal) const {
      _flightDate._itLegCabin._upr = iReal; 
      //std::cout << "UPR: " << iReal << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeBookingCounter::storeBookingCounter (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeBookingCounter::operator() (double iReal) const {
      _flightDate._itLegCabin._nbOfBookings = iReal; 
      //std::cout << "Nb of bookings: " << iReal << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeNAV::storeNAV (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeNAV::operator() (double iReal) const {
      _flightDate._itLegCabin._nav = iReal; 
      //std::cout << "NAV: " << iReal << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeGAV::storeGAV (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeGAV::operator() (double iReal) const {
      _flightDate._itLegCabin._gav = iReal; 
      //std::cout << "GAV: " << iReal << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeACP::storeACP (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeACP::operator() (double iReal) const {
      _flightDate._itLegCabin._acp = iReal; 
      //std::cout << "ACP: " << iReal << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeETB::storeETB (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeETB::operator() (double iReal) const { 
      _flightDate._itLegCabin._etb = iReal; 
      //std::cout << "ETB: " << iReal << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeYieldUpperRange::storeYieldUpperRange(FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeYieldUpperRange::operator() (double iReal) const {
      // If this is not the first bucket of the leg-cabin,
      // the already parsed bucket must be added to the leg-cabin.
      if (_flightDate._itBucket._yieldRangeUpperValue != 0.0) {
        _flightDate._itLegCabin._bucketList.push_back (_flightDate._itBucket);
      }


      // ///////////////////
      _flightDate._itBucket._yieldRangeUpperValue = iReal; 
      //std::cout << "Yield Upper Range Value: " << iReal << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeBucketAvaibality::
    storeBucketAvaibality (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeBucketAvaibality::operator() (double iReal) const {
      _flightDate._itBucket._availability = iReal; 
      //std::cout << "Availability: " << iReal << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeSeatIndex::storeSeatIndex (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeSeatIndex::operator() (double iReal) const {
      _flightDate._itBucket._seatIndex = iReal; 
      //std::cout << "Seat Index: " << iReal << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeSegmentBoardingPoint::
    storeSegmentBoardingPoint (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeSegmentBoardingPoint::operator() (iterator_t iStr,
                                                iterator_t iStrEnd) const {
      stdair::AirportCode_T lBoardingPoint (iStr, iStrEnd);

      // ///////////////////
      // When the first segment-date is read, it means that the leg section
      // is over. The parsed leg can therefore be added to the list.
      if (_flightDate._itLeg._cabinList.empty() == false) {
        _flightDate._itLegCabin._bucketList.push_back (_flightDate._itBucket);
        _flightDate._itLeg._cabinList.push_back (_flightDate._itLegCabin);
        _flightDate._legList.push_back (_flightDate._itLeg);

        // (Re-)initialise the leg-date branch of the tree
        _flightDate._itLeg._cabinList.clear();
        _flightDate._itLegCabin._cabinCode = "";
        _flightDate._itLeg._cabinList.clear();
        _flightDate._itLegCabin._bucketList.clear();
      }

      
      // ///////////////////
      // If this is not the first segment-date of the flight-date,
      // the already parsed segment-date must be added to the flight-date.
      if (_flightDate._itSegment._cabinList.empty() == false) {
        _flightDate._itSegmentCabin._classList.push_back (_flightDate._itBookingClass);
        _flightDate._itSegment._cabinList.push_back (_flightDate._itSegmentCabin);
        _flightDate._segmentList.push_back (_flightDate._itSegment);
      }

      // As that's the beginning of a new segment-date,
      // (re-)initialise the segment-cabin branch of the tree
      _flightDate._itSegment._cabinList.clear();
      _flightDate._itSegmentCabin._classList.clear();
      _flightDate._itBookingClass._classCode = "";


      // ///////////////////
      _flightDate._itSegment._boardingPoint = lBoardingPoint;
      //std::cout << "Board point: " << lBoardingPoint << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeSegmentOffPoint::storeSegmentOffPoint (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeSegmentOffPoint::operator() (iterator_t iStr,
                                           iterator_t iStrEnd) const {
      stdair::AirportCode_T lOffPoint (iStr, iStrEnd);
      _flightDate._itSegment._offPoint = lOffPoint;
      //std::cout << "Off point: " << lOffPoint << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeSegmentCabinCode::
    storeSegmentCabinCode (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeSegmentCabinCode::operator() (char iChar) const {
      // ///////////////////
      // If this is not the first segment-cabin of the segment-date,
      // the already parsed segment-cabin must be added to the segment-date.
      if (_flightDate._itSegmentCabin._classList.empty() == false) {
        _flightDate._itSegmentCabin._classList.push_back (_flightDate._itBookingClass);
        _flightDate._itSegment._cabinList.push_back (_flightDate._itSegmentCabin);
      }

      // (Re-)initialise the booking-class branch of the tree
      _flightDate._itSegmentCabin._classList.clear();
      _flightDate._itBookingClass._classCode = "";

      
      // ///////////////////
      _flightDate._itSegmentCabin._cabinCode = iChar; 
      //std::cout << "Segment-cabin code: " << iChar << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeSegmentCabinBookingCounter::
    storeSegmentCabinBookingCounter (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeSegmentCabinBookingCounter::operator() (double iReal) const {
      _flightDate._itSegmentCabin._nbOfBookings = iReal; 
      //std::cout << "Nb of bookings: " << iReal << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeClassCode::storeClassCode (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeClassCode::operator() (char iChar) const { 
      // If this is not the first booking-class of the segment-cabin,
      // the already parsed booking-class must be added to the segment-cabin.
      if (_flightDate._itBookingClass._classCode != "") {
        _flightDate._itSegmentCabin._classList.push_back (_flightDate._itBookingClass);
      }


      // ///////////////////
      _flightDate._itBookingClass._classCode = iChar; 
      //std::cout << "Booking class code: " << iChar << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeSubclassCode::storeSubclassCode (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeSubclassCode::operator() (unsigned int iNumber) const { 
      _flightDate._itBookingClass._subclassCode = iNumber; 
      //std::cout << "Sub-class code: " << iNumber << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeParentClassCode::
    storeParentClassCode (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeParentClassCode::operator() (char iChar) const { 
      _flightDate._itBookingClass._parentClassCode = iChar; 
      //std::cout << "Parent booking class code: " << iChar << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeParentSubclassCode::
    storeParentSubclassCode (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeParentSubclassCode::operator() (unsigned int iNumber) const { 
      _flightDate._itBookingClass._parentSubclassCode = iNumber; 
      //std::cout << "Parent sub-class code: " << iNumber << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeCumulatedProtection::
    storeCumulatedProtection (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeCumulatedProtection::operator() (double iReal) const {
      _flightDate._itBookingClass._cumulatedProtection = iReal; 
      //std::cout << "Cumulated protection: " << iReal << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeProtection::storeProtection (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeProtection::operator() (double iReal) const {
      _flightDate._itBookingClass._protection = iReal; 
      //std::cout << "Protection: " << iReal << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeNego::storeNego (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeNego::operator() (double iReal) const {
      _flightDate._itBookingClass._nego = iReal; 
      //std::cout << "Negotiated allotment: " << iReal << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeNoShow::storeNoShow (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeNoShow::operator() (double iReal) const {
      _flightDate._itBookingClass._noShowPercentage = iReal; 
      //std::cout << "No-Show percentage: " << iReal << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeOverbooking::storeOverbooking (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeOverbooking::operator() (double iReal) const {
      _flightDate._itBookingClass._overbookingPercentage = iReal; 
      //std::cout << "Overbooking percentage: " << iReal << std::endl;
    }

    // //////////////////////////////////////////////////////////////////
    storeNbOfBkgs::storeNbOfBkgs (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeNbOfBkgs::operator() (double iReal) const {
      _flightDate._itBookingClass._nbOfBookings = iReal; 
      //std::cout << "Nb of bookings: " << iReal << std::endl;
    }
    
    // //////////////////////////////////////////////////////////////////
    storeNbOfGroupBkgs::storeNbOfGroupBkgs (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeNbOfGroupBkgs::operator() (double iReal) const {
      _flightDate._itBookingClass._nbOfGroupBookings = iReal; 
      //std::cout << "Nb of group bookings: " << iReal << std::endl;
    }
    
    // //////////////////////////////////////////////////////////////////
    storeNbOfPendingGroupBkgs::storeNbOfPendingGroupBkgs(FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeNbOfPendingGroupBkgs::operator() (double iReal) const {
      _flightDate._itBookingClass._nbOfPendingGroupBookings = iReal; 
      //std::cout << "Nb of pending group bookings: " << iReal << std::endl;
    }
    
    // //////////////////////////////////////////////////////////////////
    storeNbOfStaffBkgs::storeNbOfStaffBkgs (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeNbOfStaffBkgs::operator() (double iReal) const {
      _flightDate._itBookingClass._nbOfStaffBookings = iReal; 
      //std::cout << "Nb of staff bookings: " << iReal << std::endl;
    }
    
    // //////////////////////////////////////////////////////////////////
    storeNbOfWLBkgs::storeNbOfWLBkgs (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeNbOfWLBkgs::operator() (double iReal) const {
      _flightDate._itBookingClass._nbOfWLBookings = iReal; 
      //std::cout << "Nb of wait-list bookings: " << iReal << std::endl;
    }
    
    // //////////////////////////////////////////////////////////////////
    storeClassETB::storeClassETB (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeClassETB::operator() (double iReal) const {
      _flightDate._itBookingClass._etb = iReal; 
      //std::cout << "Class-level ETB: " << iReal << std::endl;
    }
    
    // //////////////////////////////////////////////////////////////////
    storeClassAvailability::storeClassAvailability (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeClassAvailability::operator() (double iReal) const {
      _flightDate._itBookingClass._netClassAvailability = iReal; 
      //std::cout << "Net class availability: " << iReal << std::endl;
    }
    
    // //////////////////////////////////////////////////////////////////
    storeSegmentAvailability::storeSegmentAvailability (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeSegmentAvailability::operator() (double iReal) const {
      _flightDate._itBookingClass._segmentAvailability = iReal; 
      //std::cout << "Segment availability: " << iReal << std::endl;
    }
    
    // //////////////////////////////////////////////////////////////////
    storeRevenueAvailability::storeRevenueAvailability (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeRevenueAvailability::operator() (double iReal) const {
      _flightDate._itBookingClass._netRevenueAvailability = iReal; 
      //std::cout << "Net revenue availability: " << iReal << std::endl;
    }
    
    // //////////////////////////////////////////////////////////////////
    storeClasses::storeClasses (FlightDateStruct& ioFlightDate)
      : ParserSemanticAction (ioFlightDate) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeClasses::operator() (iterator_t iStr,
                                   iterator_t iStrEnd) const {
      std::string lClasses (iStr, iStrEnd);
      _flightDate._itSegmentCabin._classes = lClasses;

      // The list of classes is the last (according to the arrival order
      // within the schedule input file) detail of the segment cabin. Hence,
      // when a list of classes is parsed, it means that the full segment
      // cabin details have already been parsed as well: the segment cabin
      // can thus be added to the segment.
      if (_flightDate._areSegmentDefinitionsSpecific == true) {
        _flightDate.addSegmentCabin (_flightDate._itSegment,
                                     _flightDate._itSegmentCabin);
      } else {
        _flightDate.addSegmentCabin (_flightDate._itSegmentCabin);
      }
    }

    // //////////////////////////////////////////////////////////////////
    doEndFlightDate::doEndFlightDate (stdair::BomRoot& ioBomRoot,
                                      FlightDateStruct& ioFlightDate,
                                      unsigned int& ioNbOfFlights)
      : ParserSemanticAction (ioFlightDate), _bomRoot (ioBomRoot),
        _nbOfFlights (ioNbOfFlights) {
    }
    
    // //////////////////////////////////////////////////////////////////
    // void doEndFlightDate::operator() (char iChar) const {
    void doEndFlightDate::operator() (iterator_t iStr,
                                      iterator_t iStrEnd) const {

      // ///////////////////
      // The segment-date section is now over. It means that the
      // already parsed segment-date must be added to the flight-date.
      if (_flightDate._itSegment._cabinList.empty() == false) {
        _flightDate._itSegmentCabin._classList.push_back (_flightDate._itBookingClass);
        _flightDate._itSegment._cabinList.push_back (_flightDate._itSegmentCabin);
        _flightDate._segmentList.push_back (_flightDate._itSegment);
      }

      // As that's the beginning of a new segment-date,
      // (re-)initialise the segment-cabin branch of the tree
      _flightDate._itSegment._cabinList.clear();
      _flightDate._itSegmentCabin._classList.clear();
      _flightDate._itBookingClass._classCode = "";
      

      // ///////////////////
      //if (_nbOfFlights % 1000 == 0) {
        // DEBUG: Display the result
        STDAIR_LOG_DEBUG ("FlightDate #" << _nbOfFlights
                          << ": " << _flightDate.describe());
        //}
      
      // Build the FlightDate BOM objects
      InventoryBuilder::buildInventory (_bomRoot, _flightDate);

      ++_nbOfFlights;
    }

      
    // ///////////////////////////////////////////////////////////////////
    //
    //  Utility Parsers
    //
    // ///////////////////////////////////////////////////////////////////
    /** 1-digit-integer parser */
    int1_p_t int1_p;
    
    /** 2-digit-integer parser */
    uint2_p_t uint2_p;
    
    /** Up-to-2-digit-integer parser */
    uint1_2_p_t uint1_2_p;

    /** Up-to-3-digit-integer parser */
    uint1_3_p_t uint1_3_p;

    /** 4-digit-integer parser */
    uint4_p_t uint4_p;
    
    /** Up-to-4-digit-integer parser */
    uint1_4_p_t uint1_4_p;

    /** Airline Code Parser: repeat_p(2,3)[chset_p("0-9A-Z")] */
    repeat_p_t airline_code_p (chset_t("0-9A-Z").derived(), 2, 3);
      
    /** Flight Number Parser: limit_d(0u, 9999u)[uint1_4_p] */
    bounded1_4_p_t flight_number_p (uint1_4_p.derived(), 0u, 9999u);

    /** Year Parser: limit_d(00u, 99u)[uint4_p] */
    bounded2_p_t year_p (uint2_p.derived(), 0u, 99u);
      
    /** Month Parser: limit_d(1u, 12u)[uint2_p] */
    bounded2_p_t month_p (uint2_p.derived(), 1u, 12u);

    /** Day Parser: limit_d(1u, 31u)[uint2_p] */
    bounded2_p_t day_p (uint2_p.derived(), 1u, 31u);
     
    /** DOW (Day-Of-the-Week) Parser: repeat_p(7)[chset_p("0-1")] */
    repeat_p_t dow_p (chset_t("0-1").derived().derived(), 7, 7);

    /** Airport Parser: repeat_p(3)[chset_p("0-9A-Z")] */
    repeat_p_t airport_p (chset_t("0-9A-Z").derived(), 3, 3);
      
    /** Hour Parser: limit_d(0u, 24u)[uint2_p] */
    bounded1_2_p_t hours_p (uint1_2_p.derived(), 0u, 24u);

    /** Minute Parser: limit_d(0u, 59u)[uint2_p] */
    bounded2_p_t minutes_p (uint2_p.derived(), 0u, 59u);

    /** Second Parser: limit_d(0u, 59u)[uint2_p] */
    bounded2_p_t seconds_p (uint2_p.derived(), 0u, 59u);

    /** Cabin code parser: chset_p("A-Z") */
    chset_t cabin_code_p ("A-Z");

    /** Booking class code parser: chset_p("A-Z") */
    chset_t class_code_p ("A-Z");

    /** Passenger type parser: chset_p("A-Z") */
    chset_t passenger_type_p ("A-Z");

    /** Family code parser */
    int1_p_t family_code_p;
      
    /** Class Code List Parser: repeat_p(1,26)[chset_p("A-Z")] */
    repeat_p_t class_code_list_p (chset_t("A-Z").derived(), 1, 26);

    /** Stay duration Parser: limit_d(0u, 999u)[uint3_p] */
    bounded1_3_p_t stay_duration_p (uint1_3_p.derived(), 0u, 999u);


    // //////////////////////////////////////////////////////////////////
    //  (Boost Spirit) Grammar Definition
    // //////////////////////////////////////////////////////////////////

    // //////////////////////////////////////////////////////////////////
    InventoryParser::InventoryParser (stdair::BomRoot& ioBomRoot,
                                      FlightDateStruct& ioFlightDate,
                                      unsigned int& ioNbOfFlights) 
      : _bomRoot (ioBomRoot), _flightDate (ioFlightDate),
        _nbOfFlights (ioNbOfFlights) {
    }

    // //////////////////////////////////////////////////////////////////
    template<typename ScannerT>
    InventoryParser::definition<ScannerT>::
    definition (InventoryParser const& self) {

      flight_date_list = *( boost::spirit::classic::comment_p("//")
                            | boost::spirit::classic::comment_p("/*", "*/")
                            | flight_date )
        ;
      
      flight_date = flight_key
        >> leg_list
        >> segment_list
        >> flight_date_end[doEndFlightDate (self._bomRoot, self._flightDate,
                                            self._nbOfFlights)]
        ;

      flight_date_end =
        boost::spirit::classic::ch_p(';')
        ;
      
      flight_key = date[storeSnapshotDate(self._flightDate)]
        >> '/' >> airline_code
        >> '/' >> flight_number
        >> '/' >> date[storeFlightDate(self._flightDate)]
        >> '/' >> flight_type_code[storeFlightTypeCode(self._flightDate)]
        >> !( '/' >> flight_visibility_code[storeFlightVisibilityCode(self._flightDate)])
        ;

      airline_code =
        boost::spirit::classic::lexeme_d[
                                (airline_code_p)[storeAirlineCode(self._flightDate)]
                                ]
        ;
        
      flight_number =
        boost::spirit::classic::lexeme_d[
                                (flight_number_p)[storeFlightNumber(self._flightDate)]
                                ]
        ;

      date =
        boost::spirit::classic::lexeme_d[
                                (day_p)[boost::spirit::classic::assign_a(self._flightDate._itDay)]
                                >> (month_p)[boost::spirit::classic::assign_a(self._flightDate._itMonth)]
                                >> (year_p)[boost::spirit::classic::assign_a(self._flightDate._itYear)]
                                ]
        ;

      flight_type_code =
        ( boost::spirit::classic::chseq_p("INT")
          | boost::spirit::classic::chseq_p("DOM")
          | boost::spirit::classic::chseq_p("GRD") )
        ;

      flight_visibility_code =
        ( boost::spirit::classic::chseq_p("HID")
          | boost::spirit::classic::chseq_p("PSD") )
        ;

      leg_list = +( '/' >> leg )
        ;
      
      leg = leg_key >> ';' >> leg_details >> full_leg_cabin_details
        ;
	 
      leg_key = (airport_p)[storeLegBoardingPoint(self._flightDate)]
        >> ';' >> (airport_p)[storeLegOffPoint(self._flightDate)]
        ;
	 
      leg_details = date[storeBoardingDate(self._flightDate)]
        >> ';' >> time[storeBoardingTime(self._flightDate)]
        >> ';' >> date[storeOffDate(self._flightDate)]
        >> ';' >> time[storeOffTime(self._flightDate)]
        ;

      full_leg_cabin_details = +( ';' >> leg_cabin_details >> !bucket_list )
        ;
        
      leg_cabin_details = (cabin_code_p)[storeLegCabinCode(self._flightDate)]
        >> ',' >> (boost::spirit::classic::ureal_p)[storeSaleableCapacity(self._flightDate)]
        >> ',' >> (boost::spirit::classic::real_p)[storeAU(self._flightDate)]
        >> ',' >> (boost::spirit::classic::real_p)[storeUPR(self._flightDate)]
        >> ',' >> (boost::spirit::classic::real_p)[storeBookingCounter(self._flightDate)]
        >> ',' >> (boost::spirit::classic::real_p)[storeNAV(self._flightDate)]
        >> ',' >> (boost::spirit::classic::real_p)[storeGAV(self._flightDate)]
        >> ',' >> (boost::spirit::classic::ureal_p)[storeACP(self._flightDate)]
        >> ',' >> (boost::spirit::classic::real_p)[storeETB(self._flightDate)]
        ;
        
      time =
        boost::spirit::classic::lexeme_d[
            (hours_p)[boost::spirit::classic::assign_a(self._flightDate._itHours)]
         >> (minutes_p)[boost::spirit::classic::assign_a(self._flightDate._itMinutes)]
         >> !((seconds_p)[boost::spirit::classic::assign_a(self._flightDate._itSeconds)])
                                         ]
        ;
      
      bucket_list = +( ',' >> bucket_details )
        ;

      bucket_details =
        (boost::spirit::classic::ureal_p)[storeYieldUpperRange(self._flightDate)]
        >> ':' >> (boost::spirit::classic::real_p)[storeBucketAvaibality(self._flightDate)]
        >> ':' >> (uint1_3_p)[storeSeatIndex(self._flightDate)];
      
      segment_list = +( '/' >> segment )
        ;
      
      segment = segment_key >> segment_cabin_list
        ;

      segment_key = (airport_p)[storeSegmentBoardingPoint(self._flightDate)]
        >> ';' >> (airport_p)[storeSegmentOffPoint(self._flightDate)]
        ;

      segment_cabin_list =
        +( ';' >> segment_cabin_key >> ',' >> segment_cabin_details >> class_list )
        ;

      segment_cabin_key =
        (cabin_code_p)[storeSegmentCabinCode(self._flightDate)]
        ;
      
      segment_cabin_details =
        (boost::spirit::classic::ureal_p)[storeSegmentCabinBookingCounter(self._flightDate)]
        ;

      class_list = +( ',' >> class_key >> '|' >> class_details )
        ;

      class_key = (class_code_p)[storeClassCode(self._flightDate)]
        ;

      parent_subclass_code =
        (class_code_p)[storeParentClassCode(self._flightDate)]
        >> (uint1_2_p)[storeParentSubclassCode(self._flightDate)]
        ;

      class_protection =
        (boost::spirit::classic::ureal_p)[storeProtection(self._flightDate)]
        ;
        
      class_nego =
        (boost::spirit::classic::ureal_p)[storeNego(self._flightDate)]
        ;
        
      class_details = (uint1_2_p)[storeSubclassCode(self._flightDate)]
        >> ':' >> (boost::spirit::classic::ureal_p)[storeCumulatedProtection(self._flightDate)]
        >> ':' >> !( parent_subclass_code )
        >> ':' >> !( class_protection )
        >> ':' >> (boost::spirit::classic::ureal_p)[storeNoShow(self._flightDate)]
        >> ':' >> (boost::spirit::classic::ureal_p)[storeOverbooking(self._flightDate)]
        >> ':' >> (boost::spirit::classic::ureal_p)[storeNbOfBkgs(self._flightDate)]
        >> ':' >> (boost::spirit::classic::ureal_p)[storeNbOfGroupBkgs(self._flightDate)]
        >> ':' >> (boost::spirit::classic::ureal_p)[storeNbOfPendingGroupBkgs(self._flightDate)]
        >> ':' >> (boost::spirit::classic::ureal_p)[storeNbOfStaffBkgs(self._flightDate)]
        >> ':' >> (boost::spirit::classic::ureal_p)[storeNbOfWLBkgs(self._flightDate)]
        >> ':' >> (boost::spirit::classic::ureal_p)[storeClassETB(self._flightDate)]
        >> ':' >> !( class_nego )
        >> ':' >> (boost::spirit::classic::real_p)[storeClassAvailability(self._flightDate)]
        >> ':' >> (boost::spirit::classic::real_p)[storeSegmentAvailability(self._flightDate)]
        >> ':' >> (boost::spirit::classic::real_p)[storeRevenueAvailability(self._flightDate)]
        ;

      // BOOST_SPIRIT_DEBUG_NODE (InventoryParser);
      BOOST_SPIRIT_DEBUG_NODE (flight_date_list);
      BOOST_SPIRIT_DEBUG_NODE (flight_date);
      BOOST_SPIRIT_DEBUG_NODE (flight_date_end);
      BOOST_SPIRIT_DEBUG_NODE (flight_key);
      BOOST_SPIRIT_DEBUG_NODE (airline_code);
      BOOST_SPIRIT_DEBUG_NODE (flight_number);
      BOOST_SPIRIT_DEBUG_NODE (flight_type_code);
      BOOST_SPIRIT_DEBUG_NODE (flight_visibility_code);
      BOOST_SPIRIT_DEBUG_NODE (date);
      BOOST_SPIRIT_DEBUG_NODE (leg_list);
      BOOST_SPIRIT_DEBUG_NODE (leg);
      BOOST_SPIRIT_DEBUG_NODE (leg_key);
      BOOST_SPIRIT_DEBUG_NODE (leg_details);
      BOOST_SPIRIT_DEBUG_NODE (full_leg_cabin_details);
      BOOST_SPIRIT_DEBUG_NODE (leg_cabin_details);
      BOOST_SPIRIT_DEBUG_NODE (bucket_list);
      BOOST_SPIRIT_DEBUG_NODE (bucket_details);
      BOOST_SPIRIT_DEBUG_NODE (time);
      BOOST_SPIRIT_DEBUG_NODE (segment_list);
      BOOST_SPIRIT_DEBUG_NODE (segment);
      BOOST_SPIRIT_DEBUG_NODE (segment_key);
      BOOST_SPIRIT_DEBUG_NODE (full_segment_cabin_details);
      BOOST_SPIRIT_DEBUG_NODE (segment_cabin_list);
      BOOST_SPIRIT_DEBUG_NODE (segment_cabin_key);
      BOOST_SPIRIT_DEBUG_NODE (segment_cabin_details);
      BOOST_SPIRIT_DEBUG_NODE (class_list);
      BOOST_SPIRIT_DEBUG_NODE (class_key);
      BOOST_SPIRIT_DEBUG_NODE (parent_subclass_code);
      BOOST_SPIRIT_DEBUG_NODE (class_protection);
      BOOST_SPIRIT_DEBUG_NODE (class_nego);
      BOOST_SPIRIT_DEBUG_NODE (class_details);
    }

    // //////////////////////////////////////////////////////////////////
    template<typename ScannerT>
    boost::spirit::classic::rule<ScannerT> const&
    InventoryParser::definition<ScannerT>::start() const {
      return flight_date_list;
    }
    
  }


  /////////////////////////////////////////////////////////////////////////
  //
  //  Entry class for the file parser
  //
  /////////////////////////////////////////////////////////////////////////

  // //////////////////////////////////////////////////////////////////////
  InventoryFileParser::
  InventoryFileParser (stdair::BomRoot& ioBomRoot,
                    const std::string& iFilename)
    : _filename (iFilename), _bomRoot (ioBomRoot),
      _nbOfFlights (0) {
    init();
  }

  // //////////////////////////////////////////////////////////////////////
  void InventoryFileParser::init() {
    // Open the file
    _startIterator = iterator_t (_filename);

    // Check the filename exists and can be open
    if (!_startIterator) {
      std::ostringstream oMessage;
      oMessage << "The file " << _filename << " can not be open.";
      throw stdair::FileNotFoundException (oMessage.str());
    }

    // Create an EOF iterator
    _endIterator = _startIterator.make_end();
  }
    
  // //////////////////////////////////////////////////////////////////////
  bool InventoryFileParser::buildInventory () {
    bool oResult = false;
      
    STDAIR_LOG_DEBUG ("Parsing inventory input file: " << _filename);

    // Initialise the parser (grammar) with the helper/staging structure.
    InventoryParserHelper::InventoryParser lInventoryParser (_bomRoot,
                                                             _flightDate,
                                                             _nbOfFlights);
      
    // Launch the parsing of the file and, thanks to the doEndFlightDate
    // call-back structure, the building of the whole Inventory BOM
    // (i.e., including Inventory, FlightDate, LegDate, SegmentDate, etc.)
    boost::spirit::classic::parse_info<iterator_t> info =
      boost::spirit::classic::parse (_startIterator, _endIterator,
                                     lInventoryParser,
                                     boost::spirit::classic::space_p);

    // Retrieves whether or not the parsing was successful
    oResult = info.hit;
      
    const std::string hasBeenFullyReadStr = (info.full == true)?"":"not ";
    if (oResult == true) {
      STDAIR_LOG_DEBUG ("Parsing of inventory input file: " << _filename
                       << " succeeded: read " << info.length
                       << " characters. The input file has "
                       << hasBeenFullyReadStr
                       << "been fully read. Stop point: " << info.stop);
        
    } else {
      STDAIR_LOG_ERROR ("Parsing of inventory input file: " << _filename
                        << " failed: read " << info.length
                        << " characters. The input file has "
                        << hasBeenFullyReadStr
                        << "been fully read. Stop point: " << info.stop);
      throw stdair::ParserException ("Parsing of inventory input file: "
                                     + _filename + " failed.");
    }

    return oResult;
  }
    
}
