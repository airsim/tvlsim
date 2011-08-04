#ifndef __AIRSCHED_AIRSCHED_TYPES_HPP
#define __AIRSCHED_AIRSCHED_TYPES_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// Boost
#include <boost/shared_ptr.hpp>
// StdAir
#include <stdair/stdair_exceptions.hpp>

namespace AIRSCHED {

  // Forward declarations
  class AIRSCHED_Service;


  // ///////// Exceptions ///////////
  /**
   * Specific exception when some BOM objects can not be found within
   * the schedule.
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
   * The O&D input file cannot be retrieved.
   */
  class OnDInputFileNotFoundException : public stdair::FileNotFoundException {
  public:
    /**
     * Constructor.
     */
    OnDInputFileNotFoundException (const std::string& iWhat)
      : stdair::FileNotFoundException (iWhat) {}
  };

  /**
   * The schedule input file cannot be retrieved.
   */
  class ScheduleInputFileNotFoundException
    : public stdair::FileNotFoundException {
  public:
    /**
     * Constructor.
     */
    ScheduleInputFileNotFoundException (const std::string& iWhat)
      : stdair::FileNotFoundException (iWhat) {}
  };


  // //////// Type definitions specific to AirSched /////////
  /**
   * (Smart) Pointer on the AirSched service handler.
   */
  typedef boost::shared_ptr<AIRSCHED_Service> AIRSCHED_ServicePtr_T;
  
}
#endif // __AIRSCHED_AIRSCHED_TYPES_HPP
