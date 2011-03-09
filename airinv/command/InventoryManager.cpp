// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <exception>
// StdAir
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/Inventory.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/service/Logger.hpp>
// AirInv
#include <airinv/AIRINV_Types.hpp>
#include <airinv/bom/InventoryHelper.hpp>
#include <airinv/command/InventoryManager.hpp>

namespace AIRINV {
  // ////////////////////////////////////////////////////////////////////
  void InventoryManager::
  calculateAvailability (const stdair::BomRoot& iBomRoot,
                         stdair::TravelSolutionStruct& ioTravelSolution) {
    // Browse the list of segments and get the availability for the
    // children classes.
    const stdair::SegmentPath_T& lSegmentPath =
      ioTravelSolution.getSegmentPath();
    for (stdair::SegmentPath_T::const_iterator itSK = lSegmentPath.begin();
         itSK != lSegmentPath.end(); ++itSK) {
      const std::string& lSegmentKey = *itSK;
      std::string lInvKey;
      lInvKey.append (lSegmentKey, 0, 2);
      stdair::Inventory& lInventory =
        stdair::BomManager::getObject<stdair::Inventory>(iBomRoot, lInvKey);

      InventoryHelper::calculateAvailability (lInventory, lSegmentKey,
                                              ioTravelSolution);
    }
  }  

  // ////////////////////////////////////////////////////////////////////
  bool InventoryManager::
  sell (stdair::Inventory& ioInventory, const std::string& iSegmentDateKey,
        const stdair::ClassCode_T& iClassCode,
        const stdair::PartySize_T& iPartySize) {

    try {
      // Make the sale within the inventory.
      return InventoryHelper::sell (ioInventory, iSegmentDateKey,
                                    iClassCode, iPartySize);
      
    } catch (const std::exception& lStdError) {
      STDAIR_LOG_ERROR ("Error: " << lStdError.what());
      throw BookingException();
    }
  }

}
