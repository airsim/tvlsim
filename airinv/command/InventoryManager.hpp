#ifndef __AIRINV_CMD_INVENTORYMANAGER_HPP
#define __AIRINV_CMD_INVENTORYMANAGER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// STDAIR
#include <stdair/stdair_basic_types.hpp>

// Forward declarations
namespace stdair {
  class BomRoot;
  class Inventory;
  class FlightDate;
  class SegmentDate;
  class EventQueue;
  struct TravelSolutionStruct;
}

namespace AIRINV {

  // //////////////// Type definitions ////////////////
  typedef std::map<const stdair::Date_T,
                   const stdair::FlightDate*> DepartureDateFlightDateMap_T;
  typedef std::map<const stdair::GuillotineNumber_T,
                   DepartureDateFlightDateMap_T> GuillotineNumberSimilarFlightDateSetMap_T;

  /** Command wrapping the travel request process. */
  class InventoryManager {
    friend class AIRINV_Master_Service;
    friend class AIRINV_Service;

  private:
    /** Initialise the snapshot events for the inventories. */
    static void initSnapshotEvents (const stdair::Date_T&, const stdair::Date_T&,
                                    stdair::EventQueue&);
    
    /** Compute the availability for the given travel solution. */
    static void calculateAvailability (const stdair::BomRoot&,
                                       stdair::TravelSolutionStruct&);
    
    /** Compute the availability for each fare option using the AU's. */
    static void calculateAvailabilityByAU (stdair::TravelSolutionStruct&);

    /** Register a booking (segment sell). */
    static bool sell (stdair::Inventory&, const std::string& iSegmentDateKey,
                      const stdair::ClassCode_T&, const stdair::PartySize_T&);

    /** Take inventory snapshots. */
    static void takeSnapshots (const stdair::Inventory&,
                               const stdair::DateTime_T&);

  public:
    /** Create the direct accesses within the invetories suck as links between
        leg-date and segment-date, ect. */
    static void createDirectAccesses (const stdair::BomRoot&);
    static void createDirectAccesses (stdair::Inventory&);
    static void createDirectAccesses (stdair::FlightDate&);
    static void createDirectAccesses (stdair::SegmentDate&);

    /** Build the similar flight-date sets and the corresponding guillotine
        blocks for snapshots and other data. */
    static void buildSimilarFlightDateSets (const stdair::BomRoot&);
    static void buildSimilarFlightDateSets (stdair::Inventory&);
    static void buildGuillotineBlock (stdair::Inventory&,
                                      const stdair::GuillotineNumber_T&,
                                      const DepartureDateFlightDateMap_T&);
    
  private:
    /** Constructors. */
    InventoryManager() {}
    InventoryManager(const InventoryManager&) {}
    /** Destructor. */
    ~InventoryManager() {}
  };

}
#endif // __AIRINV_CMD_INVENTORYMANAGER_HPP
