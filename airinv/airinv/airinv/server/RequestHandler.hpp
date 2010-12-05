#ifndef __AIRINV_SVR_REQUESTHANDLER_HPP
#define __AIRINV_SVR_REQUESTHANDLER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// Boost
#include <boost/noncopyable.hpp>
// StdAir
#include <stdair/stdair_basic_types.hpp>
// AirInv

// Forward declarations
namespace stdair {
  struct InventoryKey_T;
  struct FlightDateKey_T;
}

namespace AIRINV {

  // Forward declarations.
  struct Reply;
  struct Request;

  /// The common handler for all incoming requests.
  class RequestHandler : private boost::noncopyable {
  public:
    // //////////// Constructors and Destructors /////////////////
    /** Constructor.
        @param const stdair::AirlineCode_T& Airline code of the inventory
               owner. */
    RequestHandler (const stdair::AirlineCode_T&);

    
  public:
    // /////////// Business Support Methods ////////////////
    /** Handle a request and produce a reply. */
    bool handleRequest (Request&, Reply&) const;

   
  private:
    // //////////////// Attributes //////////////////
    /** The directory containing the files to be served. */
    stdair::AirlineCode_T _airlineCode;
  };

}
#endif // __AIRINV_SVR_REQUESTHANDLER_HPP
