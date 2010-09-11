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

namespace intrusive {

  /** FlightDate. */
  class FlightDate : public BomAbstract {
  public:
    FlightDate (const std::string& iKey) : BomAbstract (iKey)  {}
    FlightDate (const int idx) : BomAbstract (idx) {}
    
    bi::list_member_hook<> _childHook;
  };

  /** List of child-type FlightDate objects. */
  typedef bi::member_hook <FlightDate, bi::list_member_hook<>,
                           &FlightDate::_childHook> FlightDateChildMemberOption;
  typedef bi::list<FlightDate, FlightDateChildMemberOption> FlightDateChildren;

}
#endif // __INTRUSIVE_BOM_FLIGHTDATE_HPP
