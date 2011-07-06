#ifndef __SIMFQT_BOM_FARERULESTRUCT_HPP
#define __SIMFQT_BOM_FARERULESTRUCT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <vector>
// StdAir
#include <stdair/stdair_demand_types.hpp>
#include <stdair/stdair_inventory_types.hpp>
#include <stdair/basic/StructAbstract.hpp>
#include <stdair/basic/BasParserTypes.hpp>
// SIMFQT
#include <simfqt/SIMFQT_Types.hpp>

namespace SIMFQT {

  /** Utility Structure for the parsing of fare-rule structures. */
  struct FareRuleStruct : public stdair::StructAbstract {
  public:
 
    /** Default constructor. */
    FareRuleStruct ();

  public:
    // ///////// Getters //////////
    /** Get the fare ID. */
    SIMFQT::FareQuoteID_T getFareID () const {
      return _fareId;
    }
    
    /** Get the origin. */
    stdair::AirportCode_T getOrigin () const {
      return _origin;
    }
    
    /** Get the destination. */
    stdair::AirportCode_T getDestination () const {
      return _destination;
    }

    /** Get the trip type. */
    stdair::TripType_T getTripType () const {
      return _tripType;
    }

    /** Get the date range start. */
    stdair::Date_T getDateRangeStart () const {
      return _dateRangeStart;
    }

    /** Get the date range end. */
    stdair::Date_T getDateRangeEnd () const {
      return _dateRangeEnd;
    }

    /** Get the time range start. */
    stdair::Duration_T getTimeRangeStart () const {
      return _timeRangeStart;
    }

    /** Get the time range end. */
    stdair::Duration_T getTimeRangeEnd () const {
      return _timeRangeEnd;
    }

    /** Get the cabin code. */
    stdair::CabinCode_T getCabinCode () const {
      return _cabinCode;
    }
    
    /** Get the point-of-sale. */
    const stdair::CityCode_T getPOS () const {
      return _pos;
    }

    /** Get the channel. */
    stdair::ChannelLabel_T getChannel () const {
      return _channel;
    }

    /** Get the advance purchase. */
    stdair::DayDuration_T getAdvancePurchase () const {
      return _advancePurchase;
    }

    /** Get the saturday stay option. */
    stdair::SaturdayStay_T getSaturdayStay () const {
      return _saturdayStay;
    }

    /** Get the change fees. */
    stdair::ChangeFees_T getChangeFees () const {
      return _changeFees;
    }

    /** Get the refundable option. */
    stdair::NonRefundable_T getNonRefundable () const {
      return _nonRefundable;
    }

    /** Get the minimum stay. */
    stdair::DayDuration_T getMinimumStay () const {
      return _minimumStay;
    }

    /** Get the fare. */
    stdair::PriceValue_T getFare () const {
      return _fare;
    }

    /** Get the airline code. */
    stdair::AirlineCode_T getAirlineCode () const {
      return _airlineCode;
    }

    /** Get the class code. */
    stdair::ClassCode_T getClassCode () const {
      return _classCode;
    }

    /** Get the size of the airline code list. */
    const unsigned int getAirlineListSize () const {
      return _airlineCodeList.size();
    }

    /** Get the size of the class code list. */
    const unsigned int getClassCodeListSize () const {
      return _classCodeList.size();
    }

    /** Get the airline code list. */
    stdair::AirlineCodeList_T getAirlineList () const {
      return _airlineCodeList;
    }

    /** Get the class code list. */
    stdair::ClassList_StringList_T getClassCodeList () const {
      return _classCodeList;
    }

  public:
    // ///////// Getters //////////
    /** Get the date from the staging details. */
    stdair::Date_T getDate() const;

    /** Get the time from the staging details. */
    stdair::Duration_T getTime() const;    

    /** Display of the structure. */
    const std::string describe() const;

  public:
    // ///////// Setters //////////
    /** Set the fare ID. */
    void setFareID (const SIMFQT::FareQuoteID_T& iFareQuoteID) {
      _fareId = iFareQuoteID;
    }

    /** Set the origin. */
    void setOrigin (const stdair::AirportCode_T& iOrigin) {
      _origin = iOrigin;
    }
    
    /** Set the destination. */
    void setDestination (const stdair::AirportCode_T& iDestination) {
      _destination = iDestination;
    }

    /** Set the trip type. */
    void setTripType (const stdair::TripType_T& iTripType) {
      _tripType = iTripType;
    }

    /** Set the date range start. */
    void setDateRangeStart (const stdair::Date_T& iDateRangeStart) {
      _dateRangeStart = iDateRangeStart;
    }

    /** Set the date range end. */
    void setDateRangeEnd (const stdair::Date_T& iDateRangeEnd) {
      _dateRangeEnd = iDateRangeEnd;
    }

    /** Set the time range start. */
    void setTimeRangeStart (const stdair::Duration_T& iTimeRangeStart) {
      _timeRangeStart = iTimeRangeStart;
    }

    /** Set the time range end. */
    void setTimeRangeEnd (const stdair::Duration_T& iTimeRangeEnd) {
      _timeRangeEnd = iTimeRangeEnd;
    }

    /** Set the cabin code. */
    void setCabinCode (const stdair::CabinCode_T& iCabinCode) {
      _cabinCode = iCabinCode;
    }

    /** Set the point-of-sale. */
    void setPOS (const stdair::CityCode_T& iPOS) {
      _pos = iPOS;
    }

    /** Set the channel. */
    void setChannel (const stdair::ChannelLabel_T& iChannel) {
      _channel = iChannel;
    }

    /** Set the advance purchase. */
    void setAdvancePurchase (const stdair::DayDuration_T& iAdvancePurchase) {
      _advancePurchase = iAdvancePurchase;
    }

    /** Set the saturday stay option. */
    void setSaturdayStay (const stdair::SaturdayStay_T& iSaturdayStay) {
      _saturdayStay = iSaturdayStay;
    }

    /** Set the change fees. */
    void setChangeFees (const stdair::ChangeFees_T& iChangeFees) {
      _changeFees = iChangeFees;
    }

    /** Set the refundable option. */
    void setNonRefundable (const stdair::NonRefundable_T& iNonRefundable) {
      _nonRefundable = iNonRefundable;
    }

    /** Set the minimum stay. */
    void setMinimumStay (const stdair::DayDuration_T& iMinimumStay) {
      _minimumStay = iMinimumStay;
    }

    /** Set the fare. */
    void setFare (const stdair::PriceValue_T& iFare) {
      _fare = iFare;
    }

    /** Set the airline code. */
    void setAirlineCode (const stdair::AirlineCode_T&  iAirlineCode) {
      _airlineCode = iAirlineCode;
    }

    /** Set the class code. */
    void setClassCode (const stdair::ClassCode_T& iClassCode) {
      _classCode = iClassCode;
    }

    /** Empty the airline code list.*/
    void clearAirlineCodeList () {
      _airlineCodeList.clear();
    }

    /** Empty the class code list.*/
    void clearClassCodeList () {
      _classCodeList.clear();
    }

    /** Add an airline code to the list.*/
    void addAirlineCode (stdair::AirlineCode_T& iAirlineCode)  {
      _airlineCodeList.push_back (iAirlineCode);
    }

    /** Add a class code to the list.*/
    void addClassCode (stdair::ClassCode_T& iClassCode) {
      _classCodeList.push_back (iClassCode);
    }

  public:
    // ////////////////// Attributes /////////////////
    /** Staging Date. */
    stdair::year_t _itYear;
    stdair::month_t _itMonth;
    stdair::day_t _itDay;
      
    /** Staging Time. */
    stdair::hour_t _itHours;
    stdair::minute_t _itMinutes;
    stdair::second_t _itSeconds;

  private:
    // ////////////////// Attributes /////////////////
    /** Fare ID. */
    SIMFQT::FareQuoteID_T _fareId;

    /** Origin. */
    stdair::AirportCode_T _origin;

    /** Destination. */
    stdair::AirportCode_T _destination;

    /** Type of trip (RT=round-trip, OW=one way). */
    stdair::TripType_T _tripType;

    /** Start date from the date range available for this fare rule.*/
    stdair::Date_T _dateRangeStart;

    /** End date from the date range available for this fare rule.*/
    stdair::Date_T _dateRangeEnd;
    
    /** Start time from the time range available for this fare rule.*/
    stdair::Duration_T _timeRangeStart;
     
    /** End time from the time range available for this fare rule.*/
    stdair::Duration_T _timeRangeEnd;

    /** Cabin code. */
    stdair::CabinCode_T _cabinCode;
    
    /** Point-of-sale. */
    stdair::CityCode_T _pos;
    
    /** Channel distribution. */
    stdair::ChannelLabel_T _channel;
    
    /** Minimum number of days that the ticket is sold before the flightDate.*/
    stdair::DayDuration_T _advancePurchase;
    
    /** Boolean saying whether a saturday is considered during the stay .*/
    stdair::SaturdayStay_T _saturdayStay;

    /** Boolean saying whether the change fees option is requested or not.*/
    stdair::ChangeFees_T _changeFees;

    /** Boolean saying whether the refundable option is requested or not.*/
    stdair::NonRefundable_T _nonRefundable;
     
    /** Number of days that the customer spent into the destination city.*/
    stdair::DayDuration_T _minimumStay;
    
    /** Price.*/
    stdair::PriceValue_T _fare;

    /** Airline code */
    stdair::AirlineCode_T _airlineCode;

    /** Class code */
    stdair::ClassCode_T _classCode;

    /** Airline Code List: first airline code is the code of the airline flying the first segment,
        second airline code is the code of the airline flying the second segment, ...*/
    stdair::AirlineCodeList_T _airlineCodeList;

    /** Class Code List: first class code corresponds to the class of the first segment,
        second class code corresponds to the class of the second segment, ...*/
    stdair::ClassList_StringList_T _classCodeList;

  };

}
#endif // __SIMFQT_BOM_FARERULESTRUCT_HPP
