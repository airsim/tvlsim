// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <fstream>
// StdAir
#include <stdair/stdair_types.hpp>
#include <stdair/bom/BookingRequestStruct.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/bom/FareOptionStruct.hpp>
#include <stdair/service/Logger.hpp>
// TRAVEL-CCM
#include <travelccm/bom/PriceOrientedModel.hpp>

namespace TRAVELCCM {
  // ////////////////////////////////////////////////////////////////////
  const stdair::TravelSolutionStruct* PriceOrientedModel::
  chooseTravelSolution (stdair::TravelSolutionList_T& ioTravelSolutionList,
                        const stdair::BookingRequestStruct& iBookingRequest) {
    stdair::TravelSolutionStruct* oChosenTS_ptr = NULL;

    // Get the willingness-to-pay of the customer
    const stdair::WTP_T& lWTP = iBookingRequest.getWTP();

    stdair::Fare_T lLowestFare = std::numeric_limits<stdair::Fare_T>::max();
    // Browse the travel solution list and choose the cheapest one.
    for (stdair::TravelSolutionList_T::iterator itTS =
           ioTravelSolutionList.begin(); itTS != ioTravelSolutionList.end();
         ++itTS) {
      stdair::TravelSolutionStruct& lCurrentTS = *itTS;

      const stdair::FareOptionList_T& lFareOptionList =
        lCurrentTS.getFareOptionList();
      for (stdair::FareOptionList_T::const_iterator itFO =
             lFareOptionList.begin(); itFO != lFareOptionList.end(); ++itFO) {
        const stdair::FareOptionStruct& lCurrentFO = *itFO;

        // Check the availability.
        const stdair::NbOfSeats_T& lPartySize = iBookingRequest.getPartySize();
        const stdair::ClassList_StringList_T& lClassPath =
          lCurrentFO.getClassPath();
        const stdair::ClassAvailabilityMapHolder_T& lClassAvailabilityMapHolder =
          lCurrentTS.getClassAvailabilityMapHolder();
        bool lAvlSuff = true;
        stdair::ClassAvailabilityMapHolder_T::const_iterator itCAMH =
          lClassAvailabilityMapHolder.begin();
        stdair::ClassList_StringList_T::const_iterator itClassList =
          lClassPath.begin();
        assert (lClassAvailabilityMapHolder.size() > 0 && lClassPath.size() > 0);

        for (; itCAMH != lClassAvailabilityMapHolder.end(),
               itClassList != lClassPath.end(); ++itCAMH, ++itClassList) {
          const stdair::ClassList_String_T& lCurrentClassList = *itClassList;
          const stdair::ClassAvailabilityMap_T& lClassAvlMap = *itCAMH;
          stdair::ClassCode_T lFirstClass;
          lFirstClass.append (lCurrentClassList, 0, 1);
          const stdair::ClassAvailabilityMap_T::const_iterator itClassAvl =
            lClassAvlMap.find (lFirstClass);

          // DEBUG
          if (itClassAvl == lClassAvlMap.end()) {
            std::ostringstream ostr;
            for (stdair::ClassAvailabilityMap_T::const_iterator it =
                   lClassAvlMap.begin(); it != lClassAvlMap.end(); ++it) {
              ostr << it->first << ", " << it->second << "    ";
            }

            STDAIR_LOG_DEBUG ("Can not find the class: " << lFirstClass
                              << " within the following map: " << ostr.str());
                        
          }
          assert (itClassAvl != lClassAvlMap.end());

          const stdair::Availability_T& lAvl = itClassAvl->second;
          if (lAvl < lPartySize) {
            lAvlSuff = false;
          }
        }

        // Choose the current fare option and the current solution
        // if the current fare is lower than the current lowest fare.
        const stdair::Fare_T& lCurrentFare = lCurrentFO.getFare();
        if (lCurrentFare < lLowestFare) {
          lLowestFare = lCurrentFare;
          oChosenTS_ptr = &lCurrentTS;
          oChosenTS_ptr->setChosenFareOption (lCurrentFO);
        }
      }
    }
    
    return oChosenTS_ptr;
  }

}
