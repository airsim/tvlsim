#ifndef __TRADEMGEN_BOM_DEMANDSTREAMKEY_HPP
#define __TRADEMGEN_BOM_DEMANDSTREAMKEY_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_date_time_types.hpp>
#include <stdair/bom/KeyAbstract.hpp>

namespace TRADEMGEN {

  /** Key of demand-stream. */
  struct DemandStreamKey : public stdair::KeyAbstract {
  public:
    // /////////// Construction ///////////
    /** Constructors. */
    DemandStreamKey (const stdair::AirportCode_T& iOrigin,
                     const stdair::AirportCode_T& iDestination,
                     const stdair::Date_T& iPreferredDepartureDate,
                     const stdair::CabinCode_T& iPreferredCabin);
    DemandStreamKey (const DemandStreamKey&);

    /** Destructor. */
    ~DemandStreamKey ();
    
  public:
    /** Default constructor */
    DemandStreamKey ();
    
    // /////////// Getters //////////
    /** Get the origin. */
    const stdair::AirportCode_T& getOrigin() const {
      return _origin;
    }

    /** Get the destination. */
    const stdair::AirportCode_T& getDestination() const {
      return _destination;
    }

    /** Get the preferred departure date. */
    const stdair::Date_T& getPreferredDepartureDate () const {
      return _preferredDepartureDate;
    }
    
    /** Get the preferred cabin. */
    const stdair::CabinCode_T& getPreferredCabin() const {
      return _preferredCabin;
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
       marketing classes for the same segment-stream. */
    const std::string toString() const;
    
  private:
    // Attributes
    stdair::AirportCode_T _origin;
    stdair::AirportCode_T _destination;
    stdair::Date_T _preferredDepartureDate;
    stdair::CabinCode_T _preferredCabin;
  };

}
#endif // __TRADEMGEN_BOM_DEMANDSTREAMKEY_HPP
