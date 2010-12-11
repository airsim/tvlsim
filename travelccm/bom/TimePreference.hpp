#ifndef __TRAVELCCM_BOM_TIMEPREFERENCE_HPP
#define __TRAVELCCM_BOM_TIMEPREFERENCE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// TRAVELCCM 
#include <travelccm/bom/BomAbstract.hpp>

namespace TRAVELCCM {

  /** Class used to deal with the time preference of a passenger.
      It contains different functions to compare dates and times; it
      contains the time preference of the different types of passenger
      for the moment. Later that information should be stored in a .csv
      file and parsed to be used.
  */
  class TimePreference : public BomAbstract {
    friend class FacTimePreference;
  public:

    // /////////// Display support methods /////////
    /** Dump a Business Object into an output stream.
        @param ostream& the output stream. */
    void toStream (std::ostream& ioOut) const;

    /** Read a Business Object from an input stream.
        @param istream& the input stream. */
    void fromStream (std::istream& ioIn);

    /** Get the serialised version of the Business Object. */
    std::string toString() const;
    
    /** Get a string describing the whole key (differentiating two objects
        at any level). */
    const std::string describeKey() const;

    /** Get a string describing the short key (differentiating two objects
        at the same level). */
    const std::string describeShortKey() const;

    
  private:
    /** Constructors are private so as to force the usage of the Factory
        layer. */
    /** Default constructors. */
    TimePreference ();
    TimePreference (const TimePreference&);


    /** Destructor. */
    virtual ~TimePreference();
    

  };

}
#endif // __TRAVELCCM_BOM_TIMEPREFERENCE_HPP

