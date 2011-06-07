// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// StdAir
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/FlightDate.hpp>
#include <stdair/bom/LegCabin.hpp>
#include <stdair/bom/SegmentCabin.hpp>
#include <stdair/bom/FareFamily.hpp>
#include <stdair/bom/BookingClass.hpp>
// AirInv
#include <airinv/bom/SegmentCabinHelper.hpp>
#include <airinv/bom/FlightDateHelper.hpp>

namespace AIRINV {

  // ////////////////////////////////////////////////////////////////////
  void SegmentCabinHelper::initialiseAU (stdair::SegmentCabin& iSegmentCabin) {

    // Initialise the capacity and availability pool.
    const stdair::LegCabinList_T& lLCList =
      stdair::BomManager::getList<stdair::LegCabin> (iSegmentCabin);

    stdair::CabinCapacity_T lCapacity =
      std::numeric_limits<stdair::CabinCapacity_T>::max();
    for (stdair::LegCabinList_T::const_iterator itLC = lLCList.begin();
         itLC != lLCList.end(); ++itLC) {

      const stdair::LegCabin* lLC_ptr = *itLC;
      assert (lLC_ptr != NULL);

      const stdair::CabinCapacity_T& lCabinCap = lLC_ptr->getOfferedCapacity();
      if (lCapacity > lCabinCap) {
        lCapacity = lCabinCap;
      }
    }
    iSegmentCabin.setCapacity (lCapacity);
    iSegmentCabin.setAvailabilityPool (lCapacity);
    
    // Browse the list of booking classes and set the AU of each booking
    // class to the availability pool of the cabin.
    const stdair::BookingClassList_T& lBCList =
      stdair::BomManager::getList<stdair::BookingClass> (iSegmentCabin);
    for (stdair::BookingClassList_T::const_iterator itBC = lBCList.begin();
         itBC != lBCList.end(); ++itBC) {
      stdair::BookingClass* lBC_ptr = *itBC;
      assert (lBC_ptr != NULL);
      lBC_ptr->setAuthorizationLevel (lCapacity);
    }
  }
  
  // ////////////////////////////////////////////////////////////////////
  void SegmentCabinHelper::
  updateFromReservation (const stdair::FlightDate& iFlightDate,
                         stdair::SegmentCabin& ioSegmentCabin,
                         const stdair::PartySize_T& iNbOfBookings){
    // Update the commited space of the segment-cabin.
    ioSegmentCabin.updateFromReservation (iNbOfBookings);

    // Update the commited space of the member leg-cabins.
    const stdair::LegCabinList_T& lLegCabinList =
      stdair::BomManager::getList<stdair::LegCabin> (ioSegmentCabin);
    for (stdair::LegCabinList_T::const_iterator itLegCabin =
           lLegCabinList.begin();
         itLegCabin != lLegCabinList.end(); ++itLegCabin) {
      stdair::LegCabin* lLegCabin_ptr = *itLegCabin;
      assert (lLegCabin_ptr != NULL);
      lLegCabin_ptr->updateFromReservation (iNbOfBookings);
    }

    // Update the availability pool of all the segment-cabin which belong to the
    // same flight-date.
    const stdair::CabinCode_T& lCabinCode = ioSegmentCabin.getCabinCode();
    FlightDateHelper::updateAvailablityPool (iFlightDate, lCabinCode);
  }

  // ////////////////////////////////////////////////////////////////////
  void SegmentCabinHelper::
  buildPseudoBidPriceVector (stdair::SegmentCabin& ioSegmentCabin) {
    // Retrieve the segment-cabin capacity.
    const stdair::Availability_T& lAvlPool=ioSegmentCabin.getAvailabilityPool();
    const unsigned int lAvlPoolInt =
      static_cast<unsigned int> (lAvlPool);
    stdair::BidPriceVector_T lPseudoBidPriceVector (lAvlPoolInt, 0.0);

    // Browse the leg-cabin list.
    const stdair::LegCabinList_T& lLCList =
      stdair::BomManager::getList<stdair::LegCabin> (ioSegmentCabin);
    for (stdair::LegCabinList_T::const_iterator itLC = lLCList.begin();
         itLC != lLCList.end(); ++itLC) {
      const stdair::LegCabin* lLC_ptr = *itLC;
      assert (lLC_ptr != NULL);

      const stdair::BidPriceVector_T& lBPV = lLC_ptr->getBidPriceVector();
      stdair::BidPriceVector_T::const_reverse_iterator itBP = lBPV.rbegin();
      for (stdair::BidPriceVector_T::reverse_iterator itPBP =
             lPseudoBidPriceVector.rbegin();
           itPBP != lPseudoBidPriceVector.rend(); ++itPBP, ++itBP) {
        assert (itBP != lBPV.rend());
        stdair::BidPrice_T& lCurrentPBP = *itPBP;
        const stdair::BidPrice_T& lCurrentBP = *itBP;
        lCurrentPBP += lCurrentBP;
      }
    }

    ioSegmentCabin.setBidPriceVector (lPseudoBidPriceVector);
    
    // // DEBUG
    // std::ostringstream ostr;
    // ostr << "Pseudo BPV: ";
    // for (stdair::BidPriceVector_T::const_iterator itBP =
    //       lPseudoBidPriceVector.begin(); itBP != lPseudoBidPriceVector.end();
    //      ++itBP) {
    //   const stdair::BidPrice_T& lCurrentBP = *itBP;      
    //   ostr << lCurrentBP << " ";
    // }
    // //    STDAIR_LOG_DEBUG (ostr.str());
    // std::cout << ostr.str() << std::endl;
  }

  // ////////////////////////////////////////////////////////////////////
  void SegmentCabinHelper::
  updateBookingControlsUsingPseudoBidPriceVector (const stdair::SegmentCabin& iSegmentCabin) {
    // Retrieve the pseudo bid price vector.
    const stdair::BidPriceVector_T& lPseudoBPV =
      iSegmentCabin.getBidPriceVector();
    const stdair::Availability_T& lAvlPool=iSegmentCabin.getAvailabilityPool();
    
    // Update the cumulative booking limit for all booking classes.
    const stdair::BookingClassList_T& lBCList =
      stdair::BomManager::getList<stdair::BookingClass> (iSegmentCabin);
    for (stdair::BookingClassList_T::const_iterator itBC = lBCList.begin();
         itBC != lBCList.end(); ++itBC) {
      stdair::BookingClass* lBC_ptr = *itBC;
      assert (lBC_ptr != NULL);

      lBC_ptr->setCumulatedBookingLimit (lAvlPool);
      const stdair::Yield_T& lYield = lBC_ptr->getYield();
      for (stdair::BidPriceVector_T::const_reverse_iterator itBP =
             lPseudoBPV.rbegin(); itBP != lPseudoBPV.rend(); ++itBP) {
        const stdair::BidPrice_T& lBP = *itBP;
        if (lYield < lBP) {
          stdair::BookingLimit_T lCumuBL = itBP - lPseudoBPV.rbegin();
          lBC_ptr->setCumulatedBookingLimit (lCumuBL);
          break;
        }
      }
    }

    // Update the authorization levels from the booking limits
    updateAUs (iSegmentCabin);
  }

  // ////////////////////////////////////////////////////////////////////
  void SegmentCabinHelper::updateAUs(const stdair::SegmentCabin& iSegmentCabin){
    // Browse the booking class list and compute the AU from the
    // cumulative booking counter and the cumulative booking limit.
    stdair::NbOfBookings_T lCumulativeBookingCounter = 0.0;
    const stdair::BookingClassList_T& lBCList =
      stdair::BomManager::getList<stdair::BookingClass> (iSegmentCabin);
    for (stdair::BookingClassList_T::const_reverse_iterator itBC =
           lBCList.rbegin(); itBC != lBCList.rend(); ++itBC) {
      stdair::BookingClass* lBC_ptr = *itBC;
      assert (lBC_ptr != NULL);

      const stdair::NbOfBookings_T& lNbOfBookings = lBC_ptr->getNbOfBookings();
      lCumulativeBookingCounter += lNbOfBookings;

      const stdair::BookingLimit_T& lCumuBookingLimit =
        lBC_ptr->getCumulatedBookingLimit();

      stdair::AuthorizationLevel_T lAU =
        lCumulativeBookingCounter + lCumuBookingLimit;
      lBC_ptr->setAuthorizationLevel (lAU);

      // DEBUG
      // STDAIR_LOG_DEBUG ("Updating the AU for class: "
      //                   << lBC_ptr->describeKey()
      //                   << ", with BL: " << lCumuBookingLimit
      //                   << ", CumuBkg: " << lCumulativeBookingCounter
      //                   << ", AU: " << lAU);
    }
  }

  // ////////////////////////////////////////////////////////////////////
  void SegmentCabinHelper::
  updateAvailabilities (const stdair::SegmentCabin& iSegmentCabin) {
    // Browse the booking class list and compute the avl from the
    // cumulative booking counter and the AU.
    stdair::NbOfBookings_T lCumulativeBookingCounter = 0.0;
    const stdair::BookingClassList_T& lBCList =
      stdair::BomManager::getList<stdair::BookingClass> (iSegmentCabin);
    for (stdair::BookingClassList_T::const_reverse_iterator itBC =
           lBCList.rbegin(); itBC != lBCList.rend(); ++itBC) {
      stdair::BookingClass* lBC_ptr = *itBC;
      assert (lBC_ptr != NULL);

      const stdair::NbOfBookings_T& lNbOfBookings = lBC_ptr->getNbOfBookings();
      lCumulativeBookingCounter += lNbOfBookings;

      const stdair::AuthorizationLevel_T& lAU=lBC_ptr->getAuthorizationLevel();

      const stdair::Availability_T lAvl = lAU - lCumulativeBookingCounter;
      lBC_ptr->setSegmentAvailability (lAvl);
    }
  }
}
