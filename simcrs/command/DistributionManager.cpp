// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/service/Logger.hpp>
// Airline Inventory
#include <airinv/AIRINV_Service.hpp>
// Simcrs
#include <simcrs/command/DistributionManager.hpp>

namespace SIMCRS {
  
  // ////////////////////////////////////////////////////////////////////
  void DistributionManager::
  getAvailability (const AIRINV::AIRINV_ServicePtr_Map_T& iAIRINV_ServiceMap,
                   const CRSCode_T& iCRSCode,
                   stdair::TravelSolutionList_T& ioTravelSolutionList) {
    for (stdair::TravelSolutionList_T::iterator itTS =
           ioTravelSolutionList.begin();
         itTS != ioTravelSolutionList.end(); ++itTS) {
      //stdair::TravelSolutionStruct& lCurrentTravelSolution = *itTS;
    }
  }
  
  // ////////////////////////////////////////////////////////////////////
  void DistributionManager::
  sell (const AIRINV::AIRINV_ServicePtr_Map_T& iAIRINV_ServiceMap,
        const CRSCode_T& iCRSCode,
        const stdair::TravelSolutionStruct& iTravelSolution,
        const stdair::NbOfSeats_T& iPartySize) {

    try {
      /**const stdair::KeyList_T& lSegmentDateKeyList =
        iTravelSolution.getSegmentPath();
      const stdair::ClassList_String_T& lBookingClassKeyList; //=
        //iTravelSolution.getBookingClassKeyList();
      stdair::ClassList_String_T::const_iterator itBookingClassKey =
        lBookingClassKeyList.begin();
      for (stdair::KeyList_T::const_iterator itKey= lSegmentDateKeyList.begin();
           itKey != lSegmentDateKeyList.end(); ++itKey, ++itBookingClassKey) {
        const std::string& lSegmentDateKey = *itKey;

        std::ostringstream ostrCode;
        ostrCode << lSegmentDateKey.at(0) << lSegmentDateKey.at(1);
        stdair::AirlineCode_T lAirlineCode = ostrCode.str(); 

        // Do the sale within the corresponding inventory.
        AIRINV::AIRINV_ServicePtr_Map_T::const_iterator itAirinvService =
          iAIRINV_ServiceMap.find (lAirlineCode);
        assert (itAirinvService != iAIRINV_ServiceMap.end());
        const AIRINV::AIRINV_ServicePtr_T& lAirinvService_ptr =
          itAirinvService->second;
        assert (lAirinvService_ptr);
        std::ostringstream ostr;
        ostr <<*itBookingClassKey;
        const stdair::ClassCode_T lClassCode (ostr.str());
        lAirinvService_ptr->sell (lSegmentDateKey, lClassCode, iPartySize);
        }*/
            
      // DEBUG
      // STDAIR_LOG_DEBUG ("The booking has been made");
      
    } catch (const std::exception& lStdError) {
      STDAIR_LOG_ERROR ("Error: " << lStdError.what());
      throw BookingException();
    }
  }

}
