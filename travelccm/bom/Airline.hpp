#ifndef __TRAVELCCM_BOM_AIRLINE_HPP
#define __TRAVELCCM_BOM_AIRLINE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// TRAVELCCM 
#include <travelccm/bom/BomAbstract.hpp>

namespace TRAVELCCM {

  /** Object description here. */
  class Airline : public BomAbstract {
    friend class FacAirline;
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
    Airline ();
    Airline (const Airline&);

    /** Destructor. */
    virtual ~Airline();

  };

}
#endif // __TRAVELCCM_BOM_AIRLINE_HPP

