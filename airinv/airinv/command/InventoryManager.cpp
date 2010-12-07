// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <exception>
// StdAir
#include <stdair/service/Logger.hpp>
#include <stdair/bom/Inventory.hpp>
// AirInv
#include <airinv/AIRINV_Types.hpp>
#include <airinv/bom/InventoryHelper.hpp>
#include <airinv/command/InventoryManager.hpp>

namespace AIRINV {

  // //////////////////////////////////////////////////////////////////////
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
