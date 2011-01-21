// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// StdAir
#include <stdair/basic/BasChronometer.hpp>
#include <stdair/bom/BomManager.hpp> // for display()
#include <stdair/service/Logger.hpp>
// Avlcal
#include <avlcal/basic/BasConst_AVLCAL_Service.hpp>
#include <avlcal/command/AvailabilityCalculator.hpp>
#include <avlcal/factory/FacAvlcalServiceContext.hpp>
#include <avlcal/service/AVLCAL_ServiceContext.hpp>
#include <avlcal/AVLCAL_Service.hpp>

namespace AVLCAL {

  // //////////////////////////////////////////////////////////////////////
  AVLCAL_Service::AVLCAL_Service ()
    : _avlcalServiceContext (NULL) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  AVLCAL_Service::AVLCAL_Service (const AVLCAL_Service& iService) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  AVLCAL_Service::AVLCAL_Service (const stdair::AirlineCode_T& iAirlineCode)
    : _avlcalServiceContext (NULL) {

    // Initialise the context
    init (iAirlineCode);
  }

  // //////////////////////////////////////////////////////////////////////
  AVLCAL_Service::AVLCAL_Service (const stdair::BasLogParams& iLogParams,
                                  const stdair::AirlineCode_T& iAirlineCode)
    : _avlcalServiceContext (NULL) {
    
    // Set the log file
    logInit (iLogParams);

    // Initialise the (remaining of the) context
    init (iAirlineCode);
  }

  // //////////////////////////////////////////////////////////////////////
  AVLCAL_Service::~AVLCAL_Service () {
    // Delete/Clean all the objects from memory
    finalise();
  }

  // //////////////////////////////////////////////////////////////////////
  void AVLCAL_Service::logInit (const stdair::BasLogParams& iLogParams) {
    stdair::Logger::init (iLogParams);
  }

  // //////////////////////////////////////////////////////////////////////
  void AVLCAL_Service::init (const stdair::AirlineCode_T& iAirlineCode) {
    // Initialise the context
    AVLCAL_ServiceContext& lAVLCAL_ServiceContext = 
      FacAvlcalServiceContext::instance().create (iAirlineCode);
    _avlcalServiceContext = &lAVLCAL_ServiceContext;
  }
  
  // //////////////////////////////////////////////////////////////////////
  void AVLCAL_Service::finalise () {
    assert (_avlcalServiceContext != NULL);
  }

  // //////////////////////////////////////////////////////////////////////
  void AVLCAL_Service::avlCalculate (const stdair::PartySize_T& iPartySize) {
    
    if (_avlcalServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The AvlCal service has not been initialised");
    }
    assert (_avlcalServiceContext != NULL);
    AVLCAL_ServiceContext& lAVLCAL_ServiceContext= *_avlcalServiceContext;

    try {
      
      // Retrieve the airline code
      const stdair::AirlineCode_T& lAirlineCode =
        lAVLCAL_ServiceContext.getAirlineCode();
      
      // Delegate the booking to the dedicated command
      stdair::BasChronometer lAvlCalcChronometer;
      lAvlCalcChronometer.start();
      AvailabilityCalculator::avlCalculate (lAirlineCode, iPartySize);
      const double lAvlCalcMeasure = lAvlCalcChronometer.elapsed();
      
      // DEBUG
      STDAIR_LOG_DEBUG ("Availability Calculation: " << lAvlCalcMeasure << " - "
                        << lAVLCAL_ServiceContext.display());

    } catch (const std::exception& error) {
      STDAIR_LOG_ERROR ("Exception: "  << error.what());
      throw AvlCalcultationException();
    }
  }
  
}
