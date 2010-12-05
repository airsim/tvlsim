#ifndef __TRAVELCCM_BOM_REQUEST_HPP
#define __TRAVELCCM_BOM_REQUEST_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// TYPES
#include <travelccm/TRAVELCCM_Types.hpp>
// TRAVELCCM 
#include <travelccm/bom/BomAbstract.hpp>

namespace TRAVELCCM {
  
  /** Class representing a travel request (for a given passenger). */
  class Request : public BomAbstract {
    friend class FacRequest;
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

    /** Get the departure time of the request */
    const DateTime_T getDepartureTime() const;

    /** Get the refundability of the desired ticket */
    const bool getRefundability() const;

    /** Get the cahngeability of the desired ticket */
    const bool getChangeability() const;

    /** Returns if the saturday night stay is mandatory for this ticket */
    const bool getSaturdayNightStay() const;

    /** Get the preferred airline */
    const std::string getPreferredAirline() const;

    /** Get the preferred cabin */
    const std::string getPreferredCabin() const;

    /** Get the number of restrictions taken into account, that is the number
        of private fields of this class */
    const int getNumberOfRestrictions() const {
      return 6;
    };

  private:
    /** Constructors are private so as to force the usage of the Factory
        layer. */
    /** Default constructors. */
    Request ();
    Request (const Request&);
    Request (bool _refundability, bool _changeability, bool _saturdayNightStay,
             std::string _preferredAirline, std::string _preferredCabin,
             DateTime_T _departureTime);
    
  private:
    /* different fields that describe the characteristics of the ticket wanted
       by the passenger (this class request is a field of a passenger class).
       To be noted, other restrictions can be added later if need be */
    bool _refundability;
    bool _changeability;
    bool _saturdayNightStay;
    std::string _preferredAirline;
    std::string _preferredCabin;
    DateTime_T _departureTime;
    // to add: number of stops, unwanted airline
    
    /** Destructor. */
    virtual ~Request();

  };

}
#endif // __TRAVELCCM_BOM_REQUEST_HPP

