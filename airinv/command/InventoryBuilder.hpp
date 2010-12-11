#ifndef __AIRINV_CMD_INVENTORYBUILDER_HPP
#define __AIRINV_CMD_INVENTORYBUILDER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/command/CmdAbstract.hpp>
// AirSched
#include <airinv/AIRINV_Types.hpp>

// Forward declarations
namespace stdair {
  class BomRoot;
  class Inventory;
  class FlightDate;
  class LegDate;
  class SegmentDate;
  class SegmentCabin;
}

namespace AIRINV {
  // Forward declarations
  struct FlightDateStruct;
  struct LegStruct;
  struct LegCabinStruct;
  struct SegmentStruct;
  struct SegmentCabinStruct;
  struct BookingClassStruct;
  namespace InventoryParserHelper {
    struct doEndFlightDate;
  }
  
  /** Class handling the generation / instantiation of the Inventory BOM. */
  class InventoryBuilder : public stdair::CmdAbstract {
    // Only the following class may use methods of InventoryBuilder.
    // Indeed, as those methods build the BOM, it is not good to expose
    // them publicly.
    friend struct InventoryParserHelper::doEndFlightDate;

  private:
    /** Build the inventory object corresponding to the given
        FlightDateStruct, and add them to the given bom root. */
    static void buildInventory (stdair::BomRoot&,
                                const FlightDateStruct&);
    
    /** Build the flight-date objects corresponding to the given
        FlightDateStruct, and add them to the given nventory. */
    static void buildFlightDate (stdair::Inventory&,
                                 const FlightDateStruct&);

    /** Build the leg-date objects corresponding to the given
        LegDateStruct, and add them to the given flight-date. */
    static void buildLegDate (stdair::FlightDate&,
                              const LegStruct&);
    
    /** Build the leg-cabin objects corresponding to the given
        LegCabinStruct, and add them to the given leg-date. */
    static void buildLegCabin (stdair::LegDate&,
                               const LegCabinStruct&);

    /** Build the segment-date objects corresponding to the given
        SegmentDateStruct, and add them to the given flight-date. */
    static void buildSegmentDate (stdair::FlightDate&,
                                  const SegmentStruct&);

    /** Build the segment-cabin objects corresponding to the given
        SegmentCabinStruct, and add them to the given segment-date. */
    static void buildSegmentCabin (stdair::SegmentDate&,
                                   const SegmentCabinStruct&);

    /** Build the booking class objects corresponding to the given
        BookingClassStruct, and add them to the given segment-cabin. */
    static void buildBookingClass (stdair::SegmentCabin&,
                                   const BookingClassStruct&);

  };

}
#endif // __AIRINV_CMD_INVENTORYBUILDER_HPP
