// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// StdAir
#include <stdair/basic/BasConst_General.hpp>
#include <stdair/basic/RandomGeneration.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/FlightDate.hpp>
#include <stdair/bom/LegDate.hpp>
#include <stdair/bom/SegmentDate.hpp>
#include <stdair/bom/LegCabin.hpp>
#include <stdair/bom/SegmentCabin.hpp>
#include <stdair/bom/FareFamily.hpp>
#include <stdair/bom/BookingClass.hpp>
#include <stdair/service/Logger.hpp>
// RMOL
#include <rmol/bom/MCOptimiser.hpp>
#include <rmol/bom/Emsr.hpp>
#include <rmol/bom/DPOptimiser.hpp>
#include <rmol/command/Optimiser.hpp>

namespace RMOL {

  // ////////////////////////////////////////////////////////////////////
  void Optimiser::
  optimalOptimisationByMCIntegration (const int K,
                                      stdair::LegCabin& ioLegCabin) {
    // Retrieve the segment-cabin
    const stdair::SegmentCabinList_T lSegmentCabinList =
      stdair::BomManager::getList<stdair::SegmentCabin> (ioLegCabin);
    stdair::SegmentCabinList_T::const_iterator itSC = lSegmentCabinList.begin();
    assert (itSC != lSegmentCabinList.end());
    const stdair::SegmentCabin* lSegmentCabin_ptr = *itSC;
    assert (lSegmentCabin_ptr != NULL);
    
    // Retrieve the class list.
    const stdair::BookingClassList_T lBookingClassList =
      stdair::BomManager::getList<stdair::BookingClass> (*lSegmentCabin_ptr);
    stdair::RandomGeneration lSeedGenerator (stdair::DEFAULT_RANDOM_SEED);

    // Generate the demand samples for the booking classes.
    for (stdair::BookingClassList_T::const_iterator itBC =
           lBookingClassList.begin(); itBC != lBookingClassList.end(); ++itBC) {
      stdair::RandomSeed_T lRandomSeed =
        lSeedGenerator.generateUniform01 () * 1e9;
      stdair::BookingClass* lBookingClass_ptr = *itBC;
      assert (lBookingClass_ptr != NULL);
      lBookingClass_ptr->generateDemandSamples (K, lRandomSeed);

      // DEBUG
      STDAIR_LOG_DEBUG ("Generating " << K << " demand samples for the class "
                        << lBookingClass_ptr->describeKey());
    }   
    
    // Call the class performing the actual algorithm
    MCOptimiser::optimalOptimisationByMCIntegration (ioLegCabin);
  }

  // ////////////////////////////////////////////////////////////////////
  void Optimiser::optimalOptimisationByDP (stdair::LegCabin& ioLegCabin) {
    DPOptimiser::optimalOptimisationByDP (ioLegCabin);
  }

  // ////////////////////////////////////////////////////////////////////
  void Optimiser::heuristicOptimisationByEmsr (stdair::LegCabin& ioLegCabin) {
    Emsr::heuristicOptimisationByEmsr (ioLegCabin);
  }

  // ////////////////////////////////////////////////////////////////////
  void Optimiser::heuristicOptimisationByEmsrA (stdair::LegCabin& ioLegCabin) {
    Emsr::heuristicOptimisationByEmsrA (ioLegCabin);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void Optimiser::heuristicOptimisationByEmsrB (stdair::LegCabin& ioLegCabin) {
    Emsr::heuristicOptimisationByEmsrB (ioLegCabin);
  }

  // ////////////////////////////////////////////////////////////////////
  void Optimiser::optimise (stdair::FlightDate& ioFlightDate) {
    // Browse the leg-cabin list and build the virtual class list for
    // each cabin.
    const stdair::LegDateList_T& lLDList =
      stdair::BomManager::getList<stdair::LegDate> (ioFlightDate);
    for (stdair::LegDateList_T::const_iterator itLD = lLDList.begin();
         itLD != lLDList.end(); ++itLD) {
      stdair::LegDate* lLD_ptr = *itLD;
      assert (lLD_ptr != NULL);

      //
      const stdair::LegCabinList_T& lLCList =
        stdair::BomManager::getList<stdair::LegCabin> (*lLD_ptr);
      for (stdair::LegCabinList_T::const_iterator itLC = lLCList.begin();
           itLC != lLCList.end(); ++itLC) {
        stdair::LegCabin* lLC_ptr = *itLC;
        assert (lLC_ptr != NULL);

        // Build the virtual class list.
        buildVirtualClassListForLegBasedOptimisation (*lLC_ptr);

        // Optimise using Monte-Carlo Integration method.
        optimalOptimisationByMCIntegration (10000, *lLC_ptr);
      }
    }
  }

  // ////////////////////////////////////////////////////////////////////
  void Optimiser::
  buildVirtualClassListForLegBasedOptimisation (stdair::LegCabin& ioLegCabin) {
    // The map holding all virtual classes to be created.
    stdair::VirtualClassMap_T lVirtualClassMap;
    
    // Retrieve the segment-cabin
    const stdair::SegmentCabinList_T lSegmentCabinList =
      stdair::BomManager::getList<stdair::SegmentCabin> (ioLegCabin);
    stdair::SegmentCabinList_T::const_iterator itSC = lSegmentCabinList.begin();
    assert (itSC != lSegmentCabinList.end());
    const stdair::SegmentCabin* lSegmentCabin_ptr = *itSC;
    assert (lSegmentCabin_ptr != NULL);
    
    // Retrieve the class list.
    const stdair::BookingClassList_T lBookingClassList =
      stdair::BomManager::getList<stdair::BookingClass> (*lSegmentCabin_ptr);

    // Generate the demand samples for the booking classes.
    for (stdair::BookingClassList_T::const_iterator itBC =
           lBookingClassList.begin(); itBC != lBookingClassList.end(); ++itBC) {
      stdair::BookingClass* lBookingClass_ptr = *itBC;
      assert (lBookingClass_ptr != NULL);

      const stdair::Yield_T& lYield = lBookingClass_ptr->getYield();
      stdair::VirtualClassStruct lVirtualClass (*lBookingClass_ptr);
      lVirtualClass.setYield (lYield);
      lVirtualClass.setMean (lBookingClass_ptr->getMean());
      lVirtualClass.setStdDev (lBookingClass_ptr->getStdDev());

      lVirtualClassMap.insert (stdair::VirtualClassMap_T::
                               value_type (lYield, lVirtualClass));
    }

    // Browse the virtual class map from high to low yield.
    ioLegCabin.resetVirtualClassList();
    for (stdair::VirtualClassMap_T::reverse_iterator itVC =
           lVirtualClassMap.rbegin(); itVC != lVirtualClassMap.rend(); ++itVC) {
      stdair::VirtualClassStruct& lVC = itVC->second;
      
      ioLegCabin.addVirtualClass (lVC);
    }
  }

  // ////////////////////////////////////////////////////////////////////
  void Optimiser::
  optimiseBPWithDemandAggregation (stdair::SegmentCabin& ioSegmentCabin) {
    // Retrieve the class list.
    const stdair::BookingClassList_T lBookingClassList =
      stdair::BomManager::getList<stdair::BookingClass> (ioSegmentCabin);

    stdair::YieldList_T lCabinYieldList;
    stdair::MeanValueList_T lCabinMeanList;
    stdair::StdDevValueList_T lCabinStdDevList;
      
    for (stdair::BookingClassList_T::const_iterator itBC = lBookingClassList.begin();
         itBC != lBookingClassList.end(); ++itBC) {
      stdair::BookingClass* lBookingClass_ptr = *itBC;
      assert(lBookingClass_ptr != NULL);
      //Initialisation
      stdair::Yield_T lYield = lBookingClass_ptr->getYield();
      stdair::MeanValue_T lMean = lBookingClass_ptr->getMean();      
      const stdair::MeanValueList_T& lMeanList = lBookingClass_ptr->getMeanValueList();
      const stdair::StdDevValueList_T& lStdDevList = lBookingClass_ptr->getStdDevValueList();
      stdair::MeanValueList_T::const_iterator itM = lMeanList.begin();
      stdair::StdDevValueList_T::const_iterator itSD = lStdDevList.begin();
      // Sanity check
      assert(lMeanList.size() == lStdDevList.size());
      // Std deviation calculation for the sum of independent gaussian variables
      stdair::StdDevValue_T lStdDev = pow(lBookingClass_ptr->getStdDev(),2);
      for (; itM != lMeanList.end(); ++itM, ++itSD) {
        lMean += *itM;
        lStdDev += pow(*itSD, 2);
      }
      lStdDev = sqrt(lStdDev);

      // Insert the information into the lists without loosing the yield order
      stdair::YieldList_T::reverse_iterator itYieldReverseInsertion =
        std::lower_bound(lCabinYieldList.rbegin(), lCabinYieldList.rend(), lYield);
      stdair::YieldList_T::iterator itYieldInsertion = itYieldReverseInsertion.base();
      const stdair::UnsignedIndex_T pos = std::distance (lCabinYieldList.begin(), itYieldInsertion);
      stdair::MeanValueList_T::iterator itMeanInsertion = lCabinMeanList.begin();
      stdair::StdDevValueList_T::iterator itStdDevInsertion = lCabinStdDevList.begin();
      std::advance (itMeanInsertion, pos);
      std::advance (itStdDevInsertion, pos);

      lCabinYieldList.insert (itYieldInsertion, lYield);
      lCabinMeanList.insert (itMeanInsertion, lMean);
      lCabinStdDevList.insert (itStdDevInsertion, lStdDev);
    }   

    assert (!lCabinYieldList.empty());
    const stdair::BidPrice_T lMinBP = 0.8 * lCabinYieldList.back();
    if (lCabinMeanList.back() != 0) {
      // Optimise using MC integration
      const stdair::Availability_T& lCabinCapacity = ioSegmentCabin.getAvailabilityPool();
      stdair::BidPriceVector_T lBidPriceVector;
      MCOptimiser::optimisationByMCIntegration (lBidPriceVector, lCabinCapacity, lCabinYieldList,
                                                lCabinMeanList, lCabinStdDevList, lMinBP);
      
      assert (!lBidPriceVector.empty());

      STDAIR_LOG_DEBUG ("Cabin " << ioSegmentCabin.getCabinCode() << " Bid price " << lBidPriceVector.back());
      
      ioSegmentCabin.setBidPriceVector (lBidPriceVector);
    }
  }

 // ////////////////////////////////////////////////////////////////////
  void Optimiser::
  optimiseBPWithDemandAggregation (stdair::FlightDate& ioFlightDate) {
    const stdair::SegmentDateList_T& lSegmentDateList =
      stdair::BomManager::getList<stdair::SegmentDate> (ioFlightDate);
    for (stdair::SegmentDateList_T::const_iterator itSegmentDate =
           lSegmentDateList.begin();
         itSegmentDate != lSegmentDateList.end(); ++itSegmentDate) {
      stdair::SegmentDate* lCurrentSegmentDate_ptr = *itSegmentDate;
      assert (lCurrentSegmentDate_ptr != NULL);
      
      const stdair::SegmentCabinList_T& lSegmentCabinList =
        stdair::BomManager::getList<stdair::SegmentCabin> (*lCurrentSegmentDate_ptr);
      for (stdair::SegmentCabinList_T::const_iterator itSegmentCabin =
             lSegmentCabinList.begin();
           itSegmentCabin != lSegmentCabinList.end(); ++itSegmentCabin) {
        stdair::SegmentCabin* lCurrentSegmentCabin_ptr = *itSegmentCabin;
        assert (lCurrentSegmentCabin_ptr != NULL);
        STDAIR_LOG_DEBUG ("O&D " << lCurrentSegmentDate_ptr->getBoardingPoint()
                          << "-" << lCurrentSegmentDate_ptr->getOffPoint());
        optimiseBPWithDemandAggregation (*lCurrentSegmentCabin_ptr);
      }
    }
  }

    // ////////////////////////////////////////////////////////////////////
  void Optimiser::
  optimiseBPWithYieldProration (stdair::SegmentCabin& ioSegmentCabin) {
    // Retrieve the class list.
    const stdair::BookingClassList_T lBookingClassList =
      stdair::BomManager::getList<stdair::BookingClass> (ioSegmentCabin);

    stdair::YieldList_T lCabinYieldList;
    stdair::MeanValueList_T lCabinMeanList;
    stdair::StdDevValueList_T lCabinStdDevList;
      
    for (stdair::BookingClassList_T::const_iterator itBC = lBookingClassList.begin();
         itBC != lBookingClassList.end(); ++itBC) {
      stdair::BookingClass* lBookingClass_ptr = *itBC;
      assert(lBookingClass_ptr != NULL);
      //Initialisation
      stdair::Yield_T lYield = lBookingClass_ptr->getYield();
      stdair::MeanValue_T lMean = lBookingClass_ptr->getMean();
      stdair::StdDevValue_T lStdDev = lBookingClass_ptr->getStdDev();
      
      // Insert the information into the lists without loosing the yield order
      stdair::YieldList_T::reverse_iterator itYieldReverseInsertion =
        std::lower_bound(lCabinYieldList.rbegin(), lCabinYieldList.rend(), lYield);
      stdair::YieldList_T::iterator itYieldInsertion = itYieldReverseInsertion.base();
      const stdair::UnsignedIndex_T pos = std::distance (lCabinYieldList.begin(), itYieldInsertion);
      stdair::MeanValueList_T::iterator itMeanInsertion = lCabinMeanList.begin();
      stdair::StdDevValueList_T::iterator itStdDevInsertion = lCabinStdDevList.begin();
      std::advance (itMeanInsertion, pos);
      std::advance (itStdDevInsertion, pos);

      lCabinYieldList.insert (itYieldInsertion, lYield);
      lCabinMeanList.insert (itMeanInsertion, lMean);
      lCabinStdDevList.insert (itStdDevInsertion, lStdDev);

      const stdair::AirlineCodeListList_T& lACListList =
        lBookingClass_ptr->getItineraryAirlineListList();

      const stdair::YieldList_T&  lYieldList = lBookingClass_ptr->getItineraryYieldList();
      const stdair::MeanValueList_T& lMeanList = lBookingClass_ptr->getMeanValueList();
      const stdair::StdDevValueList_T& lStdDevList = lBookingClass_ptr->getStdDevValueList();
      stdair::YieldList_T::const_iterator itY = lYieldList.begin();
      stdair::MeanValueList_T::const_iterator itM = lMeanList.begin();
      stdair::StdDevValueList_T::const_iterator itSD = lStdDevList.begin();
      stdair::AirlineCodeListList_T::const_iterator itACL = lACListList.begin();
      // Sanity check
      assert(lMeanList.size() == lStdDevList.size()
             && lYieldList.size() == lMeanList.size()
             && lACListList.size() == lMeanList.size());
      // Std deviation calculation for the sum of independent gaussian variables
      for (; itY != lYieldList.end(); ++itY, ++itM, ++itSD, ++itACL) {
        const stdair::Yield_T lTotalCnxYield = *itY;
        const stdair::AirlineCodeList_T lAirlineCodeList = *itACL;
        const unsigned int lNbOfSegments = lAirlineCodeList.size();
        stdair::Yield_T lCnxYield = lTotalCnxYield/lNbOfSegments;
        stdair::MeanValue_T lCnxMean = *itM;
        stdair::StdDevValue_T lCnxStdDev = *itSD;
        // Insert the information into the lists without loosing the yield order
        stdair::YieldList_T::reverse_iterator itYieldReverseInsertion =
          std::lower_bound(lCabinYieldList.rbegin(), lCabinYieldList.rend(), lCnxYield);
        stdair::YieldList_T::iterator itYieldInsertion = itYieldReverseInsertion.base();
        const stdair::UnsignedIndex_T pos = std::distance (lCabinYieldList.begin(), itYieldInsertion);
        stdair::MeanValueList_T::iterator itMeanInsertion = lCabinMeanList.begin();
        stdair::StdDevValueList_T::iterator itStdDevInsertion = lCabinStdDevList.begin();
        std::advance (itMeanInsertion, pos);
        std::advance (itStdDevInsertion, pos);
        
        lCabinYieldList.insert (itYieldInsertion, lCnxYield);
        lCabinMeanList.insert (itMeanInsertion, lCnxMean);
        lCabinStdDevList.insert (itStdDevInsertion, lCnxStdDev);
      }            
    }

    assert (!lCabinYieldList.empty());
    const stdair::BidPrice_T lMinBP = lCabinYieldList.back();
    
    
    if (lCabinMeanList.back() != 0) {
      // Optimise using MC integration
      const stdair::Availability_T& lCabinCapacity = ioSegmentCabin.getAvailabilityPool();
      stdair::BidPriceVector_T lBidPriceVector;
      MCOptimiser::optimisationByMCIntegration (lBidPriceVector, lCabinCapacity, lCabinYieldList,
                                                lCabinMeanList, lCabinStdDevList, lMinBP);
      
      assert (!lBidPriceVector.empty());

      STDAIR_LOG_DEBUG ("Cabin " << ioSegmentCabin.getCabinCode() << " Bid price " << lBidPriceVector.back());
      
      ioSegmentCabin.setBidPriceVector (lBidPriceVector);
    }
  }

 // ////////////////////////////////////////////////////////////////////
  void Optimiser::
  optimiseBPWithYieldProration (stdair::FlightDate& ioFlightDate) {
    const stdair::SegmentDateList_T& lSegmentDateList =
      stdair::BomManager::getList<stdair::SegmentDate> (ioFlightDate);
    for (stdair::SegmentDateList_T::const_iterator itSegmentDate =
           lSegmentDateList.begin();
         itSegmentDate != lSegmentDateList.end(); ++itSegmentDate) {
      stdair::SegmentDate* lCurrentSegmentDate_ptr = *itSegmentDate;
      assert (lCurrentSegmentDate_ptr != NULL);
      
      const stdair::SegmentCabinList_T& lSegmentCabinList =
        stdair::BomManager::getList<stdair::SegmentCabin> (*lCurrentSegmentDate_ptr);
      for (stdair::SegmentCabinList_T::const_iterator itSegmentCabin =
             lSegmentCabinList.begin();
           itSegmentCabin != lSegmentCabinList.end(); ++itSegmentCabin) {
        stdair::SegmentCabin* lCurrentSegmentCabin_ptr = *itSegmentCabin;
        assert (lCurrentSegmentCabin_ptr != NULL);
        STDAIR_LOG_DEBUG ("O&D " << lCurrentSegmentDate_ptr->getBoardingPoint()
                          << "-" << lCurrentSegmentDate_ptr->getOffPoint());
        optimiseBPWithYieldProration (*lCurrentSegmentCabin_ptr);
      }
    }
  }
  
}
