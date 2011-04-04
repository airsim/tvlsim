#ifndef __AIRINV_CMD_INVENTORYGENERATOR_HPP
#define __AIRINV_CMD_INVENTORYGENERATOR_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/command/CmdAbstract.hpp>
// Airinv
#include <airinv/AIRINV_Types.hpp>

/// Forward declarations
namespace stdair {
  class BomRoot;
  class Inventory;
  class FlightDate;
  class LegDate;
  class LegCabin;
  class SegmentDate;
  class SegmentCabin;
  class FareFamily;
}

namespace AIRINV {

  // Forward declarations
  struct FlightPeriodStruct;
  struct LegStruct;
  struct SegmentStruct;
  struct LegCabinStruct;
  struct SegmentCabinStruct;
  struct FareFamilyStruct;
  namespace ScheduleParserHelper {
    struct doEndFlight;
  }
    
  /**
   * @brief Class handling the generation / instantiation of the
   * Inventory BOM.
   */
  class InventoryGenerator : public stdair::CmdAbstract {
    /**
     * Only the following class may use methods of InventoryGenerator.
     * Indeed, as those methods build the BOM, it is not good to expose
     * them publicly.
     */
    friend class FlightPeriodFileParser;
    friend class FFFlightPeriodFileParser;
    friend struct ScheduleParserHelper::doEndFlight;
    friend class ScheduleParser;

  private:
    /**
     * Generate the flight-date objects corresponding to the given
     * Flight-Period, and add them to the given BomRoot.
     */
    static void createFlightDate (stdair::BomRoot&,
                                  const FlightPeriodStruct&);

    /**
     * Generate a flight-date.
     */
    static void createFlightDate (stdair::Inventory&,
                                  const stdair::Date_T&,
                                  const FlightPeriodStruct&);
      
    /**
     * Generate a leg-date.
     */
    static stdair::LegDate& createLegDate (stdair::FlightDate&,
                                           const stdair::Date_T&,
                                           const LegStruct&);

    /**
     * Generate a leg-cabin.
     */
    static void createLegCabin (stdair::LegDate&, const LegCabinStruct&);
      
    /**
     * Generate a bucket.
     */
    static void createBucket (stdair::LegCabin&, const BucketStruct&);

    /**
     * Generate a segment-date.
     */
    static void createSegmentDate (stdair::FlightDate&, 
                                   const SegmentStruct&);
      
    /**
     * Generate a segment-cabin.
     */
    static void createSegmentCabin (stdair::SegmentDate&,
                                    const SegmentCabinStruct&);
      
    /**
     * Generate a fare family.
     */
    static void createFareFamily (stdair::SegmentCabin&,
                                  const FareFamilyStruct&);

    /**
     * Generate a booking class.
     */
    static void createClass (stdair::FareFamily&, 
                             const stdair::ClassCode_T&);

  };

}
#endif // __AIRINV_CMD_INVENTORYGENERATOR_HPP
