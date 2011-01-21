#ifndef __AIRINV_AIRINV_TYPES_HPP
#define __AIRINV_AIRINV_TYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
// Boost
#include <boost/shared_ptr.hpp>
// StdAir
#include <stdair/stdair_exceptions.hpp>
#include <stdair/stdair_basic_types.hpp>

namespace AIRINV {

  // Forward declarations
  class AIRINV_Service;

  // ///////// Exceptions ///////////
  class SegmentDateNotFoundException : public stdair::ParserException {
  public:
    /** Constructor. */
    SegmentDateNotFoundException (const std::string& iWhat)
      : stdair::ParserException (iWhat) {}
  };

  class BookingException : public stdair::RootException {
  };


  // //////// Type definitions /////////
  /** Pointer on the AIRINV Service handler. */
  typedef boost::shared_ptr<AIRINV::AIRINV_Service> AIRINV_ServicePtr_T;
  
  /** Typedef which defines a map of airline codes and the corresponding
      airline inventories. */
  typedef std::map<const stdair::AirlineCode_T,
                   AIRINV_ServicePtr_T> AIRINV_ServicePtr_Map_T;

}
#endif // __AIRINV_AIRINV_TYPES_HPP

