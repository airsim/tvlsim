#ifndef __AIRSCHED_BOM_ORIGINDESTINATIONSETKEY_HPP
#define __AIRSCHED_BOM_ORIGINDESTINATIONSETKEY_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/bom/KeyAbstract.hpp>

namespace AIRSCHED {
  /** Key of network. */
  struct OriginDestinationSetKey : public stdair::KeyAbstract {

  private:
    // /////////// Default constructor //////////
    OriginDestinationSetKey ();

  public:
    // /////////// Construction ///////////
    /** Constructors. */
    OriginDestinationSetKey (const stdair::AirportCode_T& );
    OriginDestinationSetKey (const OriginDestinationSetKey&);
    /** Destructor. */
    ~OriginDestinationSetKey ();
    
    // /////////// Getters //////////
    /** Get the destination airport code. */
    const stdair::AirportCode_T& getOffPoint () const {
      return _destination;
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
    /** OriginDestinationSet ID. */
    stdair::AirportCode_T _destination;
  };

}
#endif // __AIRSCHED_BOM_ORIGINDESTINATIONSETKEY_HPP
