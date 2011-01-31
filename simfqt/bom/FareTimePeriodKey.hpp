#ifndef __SIMFQT_BOM_FARETIMEPERIODKEY_HPP
#define __SIMFQT_BOM_FARETIMEPERIODKEY_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// SIMFQT
#include <stdair/bom/KeyAbstract.hpp>
#include <stdair/stdair_date_time_types.hpp>

namespace SIMFQT  {
  /** Key of time-period. */
  struct FareTimePeriodKey : public stdair::KeyAbstract {

  public:
    // /////////// Construction ///////////
    /** Constructors. */
    FareTimePeriodKey (const stdair::Time_T&,
                       const stdair::Time_T&);
    FareTimePeriodKey (const FareTimePeriodKey&);
    /** Destructor. */
    ~FareTimePeriodKey ();
    
    // /////////// Getter //////////
    /** Get the time period. */
    const stdair::Time_T& getTimeRangeStart() const {
      return _timeRangeStart;
    }

    /** Get the time period. */
    const stdair::Time_T& getTimeRangeEnd() const {
      return _timeRangeEnd;
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
    /** Fare time range start */
    stdair::Time_T _timeRangeStart;

    /** Fare time range end */
    stdair::Time_T _timeRangeEnd;

  };

}
#endif // __SIMFQT_BOM_FARETIMEPERIODKEY_HPP
