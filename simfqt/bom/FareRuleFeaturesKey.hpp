#ifndef __SIMFQT_BOM_FARERULEKEY_HPP
#define __SIMFQT_BOM_FARERULEKEY_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// SIMFQT
#include <stdair/bom/KeyAbstract.hpp>
#include <stdair/stdair_demand_types.hpp>
#include <stdair/stdair_inventory_types.hpp>
#include <stdair/stdair_date_time_types.hpp>

namespace SIMFQT  {
  /** Key of fare-rule. */
  struct FareRuleFeaturesKey : public stdair::KeyAbstract {

  private:
    // /////////// Default constructor //////////
    FareRuleFeaturesKey () { };
  public:
    // /////////// Construction ///////////
    /** Constructors. */
    FareRuleFeaturesKey (const stdair::DayDuration_T&,const stdair::SaturdayStay_T&,
                         const stdair::ChangeFees_T&, const stdair::NonRefundable_T&,
                         const stdair::DayDuration_T&, const stdair::Fare_T&);
    FareRuleFeaturesKey (const FareRuleFeaturesKey&);
    /** Destructor. */
    ~FareRuleFeaturesKey ();  

    // /////////// Getters //////////
    /** Get the fare day duration. */
    const stdair::DayDuration_T& getAdvancePurchase() const {
      return _advancePurchase;
    }

    /** Get the fare saturday stay option. */
    const stdair::SaturdayStay_T& getSaturdayStay() const {
      return _saturdayStay;
    }   

    /** Get the change fees criterion. */
    const stdair::ChangeFees_T& getChangeFees() const {
      return _changeFees;
    }

    /** Get the refundable option. */
    const stdair::NonRefundable_T& getRefundableOption() const {
      return _nonRefundable;
    }

    /** Get the minimum stay. */
    const stdair::DayDuration_T& getMinimumStay() const {
      return _minimumStay;
    }

    /** Get the fare. */
    const stdair::Fare_T& getFare() const {
      return _fare;
    }
    
    // /////////// Display support methods /////////
    /** Dump a Business Object Key into an output stream.
        @param ostream& the output stream. */
    void toStream (std::ostream& ioOut) const;

    /** Read a Business Object Key from an input stream.
        @param istream& the input stream. */
    void fromStream (std::istream& ioIn);

   /** Get the serialised version of the Business Object Key.
       <br>That string is unique, at the level of a given Business Object,
       when among children of a given parent Business Object. */
    const std::string toString() const;

  private:
    // Attributes   
    /** Number of days that the ticket is sold before the
	flightDate.*/
    stdair::DayDuration_T _advancePurchase;
     
    /** Boolean saying whether a saturday is considered
	during the stay .*/
    stdair::SaturdayStay_T _saturdayStay;
    
    /** Boolean saying whether the change fees option is
	requested or not.*/
    stdair::ChangeFees_T _changeFees;

    /** Boolean saying whether the refundable option is
	requested or not.*/
    stdair::NonRefundable_T _nonRefundable;
     
    /** Number of days that the customer spent
	into the destination city.*/
    stdair::DayDuration_T _minimumStay;
 
    /** Fare value.*/
    stdair::Fare_T _fare;

  };

}
#endif // __SIMFQT_BOM_FARERULEKEY_HPP
