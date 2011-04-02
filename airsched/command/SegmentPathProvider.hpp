#ifndef __AIRSCHED_COM_CMD_SEGMENTPATHPROVIDER_HPP
#define __AIRSCHED_COM_CMD_SEGMENTPATHPROVIDER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/bom/TravelSolutionTypes.hpp>
#include <stdair/command/CmdAbstract.hpp>

/// Forward declarations
namespace stdair {
  class BomRoot;
  struct BookingRequestStruct;
}

namespace AIRSCHED {

  /// Forward declarations
  class ReachableUniverse;
  class OriginDestinationSet;
  class SegmentPathPeriod;

  /**
   * @brief Class building the travel solutions from airline schedules.
   */
  class SegmentPathProvider : public stdair::CmdAbstract {
    friend class AIRSCHED_Service;
    
  private:
    // ////////////////// Business Methods ///////////////////
    /**
     * Calculate and return a list of travel solutions corresponding
     * to a given product demand (taking the shape of a booking request).
     *
     * @param stdair::TravelSolutionList_T& List of travel solutions,
     *        to be filled by that method (it is first cleared).
     * @param const stdair::BomRoot& Root of the BOM tree.
     * @param const stdair::BookingRequestStruct& Booking request for which
     *        the travel solutions are retrieved.
     */
    static void buildSegmentPathList (stdair::TravelSolutionList_T&,
                                      const stdair::BomRoot&,
                                      const stdair::BookingRequestStruct&);

    /**
     * Calculate and return a list of travel solutions corresponding
     * to a given product demand (taking the shape of a booking request).
     *
     * @param stdair::TravelSolutionList_T& List of travel solutions,
     *        to be filled by that method (it is first cleared).
     * @param const ReachableUniverse& Root of the BOM tree.
     * @param const stdair::BookingRequestStruct& Booking request for which
     *        the travel solutions are retrieved.
     */
    static void buildSegmentPathList (stdair::TravelSolutionList_T&,
                                      const ReachableUniverse&,
                                      const stdair::BookingRequestStruct&);

    /**
     * Calculate and return a list of travel solutions corresponding
     * to a given product demand (taking the shape of a booking request).
     *
     * @param stdair::TravelSolutionList_T& List of travel solutions,
     *        to be filled by that method (it is first cleared).
     * @param const OriginDestinationSet& Root of the BOM tree.
     * @param const stdair::BookingRequestStruct& Booking request for which
     *        the travel solutions are retrieved.
     */
    static void buildSegmentPathList (stdair::TravelSolutionList_T&,
                                      const OriginDestinationSet&,
                                      const stdair::BookingRequestStruct&);

    /**
     * Calculate and return a list of travel solutions corresponding
     * to a given product demand (taking the shape of a booking request).
     *
     * @param stdair::TravelSolutionList_T& List of travel solutions,
     *        to be filled by that method (it is first cleared).
     * @param const SegmentPathPeriod& Root of the BOM tree.
     * @param const stdair::BookingRequestStruct& Booking request for which
     *        the travel solutions are retrieved.
     */
    static void buildSegmentPathList (stdair::TravelSolutionList_T&,
                                      const SegmentPathPeriod&,
                                      const stdair::BookingRequestStruct&);
  };

}
#endif // __AIRSCHED_COM_CMD_SEGMENTPATHPROVIDER_HPP
