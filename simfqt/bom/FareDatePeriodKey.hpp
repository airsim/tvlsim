#ifndef __SIMFQT_BOM_FAREDATEPERIODKEY_HPP
#define __SIMFQT_BOM_FAREDATEPERIODKEY_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// SIMFQT
#include <stdair/bom/KeyAbstract.hpp>
#include <stdair/stdair_date_time_types.hpp>

namespace SIMFQT  {
  /** Key of date-period. */
  struct FareDatePeriodKey : public stdair::KeyAbstract {

  public:
    // /////////// Construction ///////////
    /** Constructors. */
    FareDatePeriodKey (const stdair::DatePeriod_T&);
    FareDatePeriodKey (const FareDatePeriodKey&);
    /** Destructor. */
    ~FareDatePeriodKey ();
    
    // /////////// Getter //////////
    /** Get the date period. */
    const stdair::DatePeriod_T& getDatePeriod() const {
      return _datePeriod;
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
    /** Fare date period */
    stdair::DatePeriod_T _datePeriod;

  };

}
#endif // __SIMFQT_BOM_FAREDATEPERIODKEY_HPP
