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
#include <stdair/stdair_inventory_types.hpp>

namespace AIRINV {

  // Forward declarations
  class AIRINV_Service;
  class AIRINV_Master_Service;


  // ///////// Exceptions ///////////
  /**
   * Specific exception when some BOM objects can not be found within
   * the inventory.
   */
  class SegmentDateNotFoundException : public stdair::ParserException {
  public:
    /**
     * Constructor.
     */
    SegmentDateNotFoundException (const std::string& iWhat)
      : stdair::ParserException (iWhat) {}
  };

  /**
   * Specific exception related to bookings made against the inventory.
   */
  class BookingException : public stdair::RootException {
  };


  // //////// Type definitions /////////
  /**
   * (Smart) Pointer on the AirInv (slave) service handler.
   */
  typedef boost::shared_ptr<AIRINV_Service> AIRINV_ServicePtr_T;
  
  /**
   * (Smart) Pointer on the AirInv master service handler.
   */
  typedef boost::shared_ptr<AIRINV_Master_Service> AIRINV_Master_ServicePtr_T;
  
  /**
   * Type defining a map of airline codes and the corresponding
   * airline inventories.
   */
  typedef std::map<const stdair::AirlineCode_T,
                   AIRINV_ServicePtr_T> AIRINV_ServicePtr_Map_T;

  /**
   * Define the FRAT5 curve.
   */
  typedef std::map<const stdair::DTD_T, double> FRAT5Curve_T;

}
#endif // __AIRINV_AIRINV_TYPES_HPP

