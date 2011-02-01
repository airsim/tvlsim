#ifndef __SIMFQT_BOM_FARERULEFEATURES_HPP
#define __SIMFQT_BOM_FARERULEFEATURES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STDAIR
#include <stdair/bom/BomAbstract.hpp>
#include <stdair/stdair_date_time_types.hpp>
// SIMFQT
#include <simfqt/bom/FareRuleFeaturesKey.hpp>
#include <simfqt/bom/FareRuleFeaturesTypes.hpp>

// Forward declaration
namespace stdair {
  template <typename BOM> class FacBom;
  class FacBomManager;
}

namespace SIMFQT {

  /** Class representing the actual attributes for a fare-rule. */
  class FareRuleFeatures : public stdair::BomAbstract {
    template <typename BOM> friend class stdair::FacBom;
    friend class stdair::FacBomManager;

  public:
    // Type definitions.
    /** Definition allowing to retrieve the associated BOM key type. */
    typedef FareRuleFeaturesKey Key_T;
  
  public:
    // /////////// Display support methods /////////
    /** Dump a Business Object into an output stream.
        @param ostream& the output stream. */
    void toStream (std::ostream& ioOut) const { ioOut << toString(); }

    /** Read a Business Object from an input stream.
        @param istream& the input stream. */
    void fromStream (std::istream& ioIn) { }

   /** Get the serialised version of the Business Object. */
    std::string toString() const;
    
    /** Get a string describing the  key. */
    const std::string describeKey() const { return _key.toString(); }

  public:
    // ////////// Getters ////////////
    const Key_T& getKey() const { return _key; }
    stdair::BomAbstract* const getParent() const { return _parent; }
    const  stdair::HolderMap_T& getHolderMap() const { return _holderMap; }

    /** Get the fare day duration. */
    const stdair::DayDuration_T& getAdvancePurchase() const {
      return _key.getAdvancePurchase();
    }

    /** Get the fare saturday stay option. */
    const stdair::SaturdayStay_T& getSaturdayStay() const {
      return _key.getSaturdayStay();
    }   

    /** Get the change fees criterion. */
    const stdair::ChangeFees_T& getChangeFees() const {
      return _key.getChangeFees();
    }

    /** Get the refundable option. */
    const stdair::NonRefundable_T& getRefundableOption() const {
      return _key.getRefundableOption();
    }

    /** Get the minimum stay. */
    const stdair::DayDuration_T& getMinimumStay() const {
      return _key.getMinimumStay();
    }

    /** Get the fare. */
    const stdair::Fare_T& getFare() const {
      return _key.getFare();
    }

  public:
    // ////////////// Business methods ///////////////
    /** Check if a given stay duration is greater or equal to
        the minimum stay of the fare rule. */
    bool IsStayDurationValid (const stdair::DayDuration_T&) const;

    /** Check if a booking request date is valid compared the required
        advance purchase number of days of the fare rule. */
    bool IsAdvancePurchaseValid (const stdair::DateTime_T&,
                                 const stdair::DateTime_T&) const;
    
  protected:
    /** Default constructors. */
    FareRuleFeatures (const Key_T&);
    FareRuleFeatures (const FareRuleFeatures&);
    /** Destructor. */
    ~FareRuleFeatures();

  protected:
    // Attributes
    Key_T _key;
    stdair::BomAbstract* _parent;
    stdair::HolderMap_T _holderMap;

  };

}
#endif // __SIMFQT_BOM_FARERULEFEATURES_HPP

