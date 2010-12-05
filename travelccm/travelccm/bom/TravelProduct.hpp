#ifndef __TRAVELCCM_BOM_TRAVELPRODUCT_HPP
#define __TRAVELCCM_BOM_TRAVELPRODUCT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// TRAVELCCM
#include <travelccm/bom/BomAbstract.hpp>

namespace TRAVELCCM {

  /** Class representing a travel product, i.e., a list of segment-classes.
      <br>For instance, the travel product
      (BA341, 20JAN2009, NCE-LHR, H class)-(BA179, 20JAN2009, LHR-JFK, K class)
      represents a trip departing from Nice on the 20th January and arriving
      at New York through London Heathrow, taking British Airways flights.
  */
  class TravelProduct : public BomAbstract {
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

  };

}
#endif // __TRAVELCCM_COM_BOM_TRAVELPRODUCT_HPP
