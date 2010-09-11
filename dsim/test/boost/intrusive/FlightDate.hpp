#ifndef __INTRUSIVE_BOM_FLIGHTDATE_HPP
#define __INTRUSIVE_BOM_FLIGHTDATE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
// Local
#include <test/boost/intrusive/BomAbstract.hpp>

/** Alias for the boost::intrusive namespace. */
namespace bi = boost::intrusive;

namespace stdair {

  /** FlightDate. */
  class FlightDate : public BomAbstract {
  public:
    /** Constructors. */
    FlightDate (const std::string& iKey) : BomAbstract (iKey)  {}
    FlightDate (const int idx) : BomAbstract (idx) {}
    /** Destructor. */
    ~FlightDate() {}
    
    bi::list_member_hook<> _childHook;

  public:
    // /////////// Display support methods /////////
    /** Dump a Business Object into an output stream.
        @param ostream& the output stream. */
    void toStream (std::ostream& ioOut) const { ioOut << toString(); }

    /** Read a Business Object from an input stream.
        @param istream& the input stream. */
    void fromStream (std::istream& ioIn) { }

    /** Get the serialised version of the Business Object. */
    std::string toString() const { return describeKey(); }
    
    /** Get a string describing the  key. */
    const std::string describeKey() const { return _key; }
  };

  /** List of child-type FlightDate objects. */
  typedef bi::member_hook <FlightDate, bi::list_member_hook<>,
                           &FlightDate::_childHook> FlightDateChildMemberOption;
  typedef bi::list<FlightDate, FlightDateChildMemberOption> FlightDateChildren;

}
#endif // __INTRUSIVE_BOM_FLIGHTDATE_HPP
