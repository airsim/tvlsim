#ifndef __AIRSCHED_BOM_REACHABLEUNIVERSEKEY_HPP
#define __AIRSCHED_BOM_REACHABLEUNIVERSEKEY_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/bom/KeyAbstract.hpp>

namespace AIRSCHED {
  /** Key of airport-date. */
  struct ReachableUniverseKey : public stdair::KeyAbstract {

  private:
    // /////////// Default constructor //////////
    ReachableUniverseKey ();
    
  public:
    // /////////// Construction ///////////
    /** Constructors. */
    ReachableUniverseKey (const stdair::AirportCode_T& iAirportCode);
    ReachableUniverseKey (const ReachableUniverseKey&);
    /** Destructor. */
    ~ReachableUniverseKey ();
    
    // /////////// Getters //////////
    /** Get boarding airport. */
    const stdair::AirportCode_T& getBoardingPoint() const {
      return _origin;
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
       when among children of a given parent Business Object.
       <br>For instance, "H" and "K" allow to differentiate among two
       marketing classes for the same segment-date. */
    const std::string toString() const;

  private:
    // Attributes
    /** The boarding airport. */
    stdair::AirportCode_T _origin;
  };

}

#endif // __AIRSCHED_BOM_REACHABLEUNIVERSEKEY_HPP
