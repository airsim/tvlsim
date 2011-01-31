#ifndef __SIMFQT_BOM_FARETIMEPERIOD_HPP
#define __SIMFQT_BOM_FARETIMEPERIOD_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STDAIR
#include <stdair/bom/BomAbstract.hpp>
// SIMFQT
#include <simfqt/bom/FareTimePeriodKey.hpp>
#include <simfqt/bom/FareTimePeriodTypes.hpp>

// Forward declaration
namespace stdair {
  template <typename BOM> class FacBom;
  class FacBomManager;
}

namespace SIMFQT {

  /** Class representing the actual attributes for a fare time-period. */
  class FareTimePeriod : public stdair::BomAbstract {
    template <typename BOM> friend class stdair::FacBom;
    friend class stdair::FacBomManager;

  public:
    // Type definitions.
    /** Definition allowing to retrieve the associated BOM key type. */
    typedef FareTimePeriodKey Key_T;
  
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
    const stdair::Time_T& getTimeRangeStart() const {
      return _key.getTimeRangeStart();
    }
    const stdair::Time_T& getTimeRangeEnd() const {
      return _key.getTimeRangeEnd();
    }
    stdair::BomAbstract* const getParent() const { return _parent; }
    const  stdair::HolderMap_T& getHolderMap() const { return _holderMap; }

  public:
    // ////////////// Business methods ///////////////
    /** Check if the given departure time is included in the departure period
        of the segment path. */
    bool isDepartureTimeValid (const stdair::Time_T&) const;
    
  protected:
    /** Default constructors. */
    FareTimePeriod (const Key_T&);
    FareTimePeriod (const FareTimePeriod&);
    /** Destructor. */
    ~FareTimePeriod();

  protected:
    // Attributes
    Key_T _key;
    stdair::BomAbstract* _parent;
    stdair::HolderMap_T _holderMap;

  };

}
#endif // __SIMFQT_BOM_FARETIMEPERIOD_HPP

