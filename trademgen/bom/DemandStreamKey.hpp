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

  /**
   * Key of a given demand-stream, made of a pair of required
   * airports/cities (origin and destination), a preferred departure
   * date and a preferred cabin. Those attributes correspond to a the
   * travel requirements of a simulated traveller.
   */
  struct DemandStreamKey : public stdair::KeyAbstract {

    // /////////// Constructors and destructors ///////////
  private:
    /** Default constructor. */
    DemandStreamKey();

  public:
    /** Constructor. */
    DemandStreamKey (const stdair::AirportCode_T& iOrigin,
                     const stdair::AirportCode_T& iDestination,
                     const stdair::Date_T& iPreferredDepartureDate,
                     const stdair::CabinCode_T& iPreferredCabin);
    /** Default copy constructor. */
    DemandStreamKey (const DemandStreamKey&);

    /** Destructor. */
    ~DemandStreamKey();

    
  public:
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
    // ///////////////// Attributes ///////////////
    /** Required origin airport/city. */
    stdair::AirportCode_T _origin;

    /** Required destination airport/city. */
    stdair::AirportCode_T _destination;

    /** Preferred departure date. */
    stdair::Date_T _preferredDepartureDate;

    /** Preferred cabin. */
    stdair::CabinCode_T _preferredCabin;
  };

}
#endif // __TRADEMGEN_BOM_DEMANDSTREAMKEY_HPP
