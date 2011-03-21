#ifndef __AIRINV_CMD_INVENTORYBUILDER_HPP
#define __AIRINV_CMD_INVENTORYBUILDER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/command/CmdAbstract.hpp>
// AirInv
#include <airinv/AIRINV_Types.hpp>

/// Forward declarations
namespace stdair {
  class BomRoot;
  class Inventory;
  class FlightDate;
  class LegDate;
  class LegCabin;
  class Bucket;
  class SegmentDate;
  class SegmentCabin;
  class FareFamily;
}

namespace AIRINV {

  /// Forward declarations
  struct FlightDateStruct;
  struct LegStruct;
  struct LegCabinStruct;
  struct BucketStruct;
  struct SegmentStruct;
  struct SegmentCabinStruct;
  struct FareFamilyStruct;
  struct BookingClassStruct;
  namespace InventoryParserHelper {
    struct doEndFlightDate;
  }
  
  /**
   * @brief Class handling the generation / instantiation of the Inventory BOM.
   */
  class InventoryBuilder : public stdair::CmdAbstract {
    /**
     * Only the following class may use methods of InventoryBuilder.
     * Indeed, as those methods build the BOM, it is not good to expose
     * them publicly.
     */
    friend struct InventoryParserHelper::doEndFlightDate;

  private:
    /**
     * Build the inventory object corresponding to the given
     * FlightDateStruct, and add them to the given bom root.
     */
    static void buildInventory (stdair::BomRoot&, const FlightDateStruct&);
    
    /**
     * Build the flight-date objects corresponding to the given
     * FlightDateStruct, and add them to the given nventory.
     */
    static void buildFlightDate (stdair::Inventory&, const FlightDateStruct&);

    /**
     * Build the leg-date objects corresponding to the given
     * LegDateStruct, and add them to the given flight-date.
     */
    static void buildLegDate (stdair::FlightDate&, const LegStruct&);
    
    /**
     * Build the leg-cabin objects corresponding to the given
     * LegCabinStruct, and add them to the given leg-date.
     */
    static void buildLegCabin (stdair::LegDate&, const LegCabinStruct&);

    /**
     * Build the leg-cabin objects corresponding to the given
     * LegCabinStruct, and add them to the given leg-date.
     */
    static void buildBucket (stdair::LegCabin&, const BucketStruct&);

    /**
     * Build the segment-date objects corresponding to the given
     * SegmentDateStruct, and add them to the given flight-date.
     */
    static void buildSegmentDate (stdair::FlightDate&, const SegmentStruct&);

    /**
     * Build the segment-cabin objects corresponding to the given
     * SegmentCabinStruct, and add them to the given segment-date.
     */
    static void buildSegmentCabin (stdair::SegmentDate&,
                                   const SegmentCabinStruct&);

    /**
     * Build the fare family objects corresponding to the given
     * FareFamilyStruct, and add them to the given segment-cabin.
     */
    static void buildFareFamily (stdair::SegmentCabin&,
                                 const FareFamilyStruct&);

    /**
     * Build the booking class objects corresponding to the given
     * BookingClassStruct, and add them to the given fare family.
     */
    static void buildBookingClass (stdair::FareFamily&,
                                   const BookingClassStruct&);
  };

}
#endif // __AIRINV_CMD_INVENTORYBUILDER_HPP
