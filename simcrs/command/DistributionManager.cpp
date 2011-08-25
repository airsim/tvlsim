// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/bom/FareOptionStruct.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/bom/CancellationStruct.hpp>
#include <stdair/service/Logger.hpp>
// Airline Inventory
#include <airinv/AIRINV_Master_Service.hpp>
// SimCRS
#include <simcrs/command/DistributionManager.hpp>

namespace SIMCRS {
  
  // ////////////////////////////////////////////////////////////////////
  void DistributionManager::
  calculateAvailability (AIRINV::AIRINV_Master_Service& ioAIRINV_Master_Service,
                         stdair::TravelSolutionList_T& ioTravelSolutionList,
                         const stdair::PartnershipTechnique& iPartnershipTechnique) {
    for (stdair::TravelSolutionList_T::iterator itTS =
           ioTravelSolutionList.begin();
         itTS != ioTravelSolutionList.end(); ++itTS) {
      stdair::TravelSolutionStruct& lCurrentTravelSolution = *itTS;

      // Forward the work to the dedicated service.
      ioAIRINV_Master_Service.calculateAvailability (lCurrentTravelSolution,
                                                     iPartnershipTechnique);
    }
  }
  
  // ////////////////////////////////////////////////////////////////////
  bool DistributionManager::
  sell (AIRINV::AIRINV_Master_Service& ioAIRINV_Master_Service,
        const stdair::TravelSolutionStruct& iTravelSolution,
        const stdair::NbOfSeats_T& iPartySize) {
    bool hasSaleBeenSuccessful = false;

    const stdair::KeyList_T& lSegmentDateKeyList =
      iTravelSolution.getSegmentPath();
    const stdair::FareOptionStruct& lChosenFareOption =
      iTravelSolution.getChosenFareOption ();
    const stdair::ClassList_StringList_T& lClassPath =
      lChosenFareOption.getClassPath();
    stdair::ClassList_StringList_T::const_iterator itClassKeyList =
      lClassPath.begin();
    for (stdair::KeyList_T::const_iterator itKey= lSegmentDateKeyList.begin();
         itKey != lSegmentDateKeyList.end(); ++itKey, ++itClassKeyList) {
      const std::string& lSegmentDateKey = *itKey;
      
      // TODO: Removed this hardcode.
      std::ostringstream ostr;
      const stdair::ClassList_String_T& lClassList = *itClassKeyList;
      assert (lClassList.size() > 0);
      ostr << lClassList.at(0);
      const stdair::ClassCode_T lClassCode (ostr.str());
      
      hasSaleBeenSuccessful = 
        ioAIRINV_Master_Service.sell (lSegmentDateKey, lClassCode, iPartySize);
    }

    return hasSaleBeenSuccessful;
  }

  // ////////////////////////////////////////////////////////////////////
  bool DistributionManager::
  playCancellation (AIRINV::AIRINV_Master_Service& ioAIRINV_Master_Service,
                    const stdair::CancellationStruct& iCancellation) {
    bool hasCancellationBeenSuccessful = false;

    const stdair::PartySize_T& lPartySize = iCancellation.getPartySize();
    const stdair::KeyList_T& lSegmentDateKeyList =
      iCancellation.getSegmentPath();
    const stdair::ClassList_String_T& lClassList = iCancellation.getClassList();
    stdair::ClassList_String_T::const_iterator itClass = lClassList.begin();
    for (stdair::KeyList_T::const_iterator itKey= lSegmentDateKeyList.begin();
         itKey != lSegmentDateKeyList.end(); ++itKey, ++itClass) {
      const std::string& lSegmentDateKey = *itKey;
      
      // TODO: Removed this hardcode.
      std::ostringstream ostr;
      ostr << *itClass;
      const stdair::ClassCode_T lClassCode (ostr.str());
      
      hasCancellationBeenSuccessful =
        ioAIRINV_Master_Service.cancel (lSegmentDateKey, lClassCode,
                                        lPartySize);
    }    
    return hasCancellationBeenSuccessful;
  }
  
}
