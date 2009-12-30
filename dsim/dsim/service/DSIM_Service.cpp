// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// Boost
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
// Distribution
#include <simcrs/SIMCRS_Service.hpp>
// Dsim
#include <dsim/basic/BasConst_DSIM_Service.hpp>
#include <dsim/basic/BasChronometer.hpp>
#include <dsim/command/Simulator.hpp>
#include <dsim/factory/FacDsimServiceContext.hpp>
#include <dsim/service/DSIM_ServiceContext.hpp>
#include <dsim/service/Logger.hpp>
#include <dsim/DSIM_Service.hpp>

namespace DSIM {

  // //////////////////////////////////////////////////////////////////////
  DSIM_Service::
  DSIM_Service (const std::string& iScheduleInputFilename,
                std::ostream& ioLogStream)
    : _dsimServiceContext (NULL) {
    init (iScheduleInputFilename, ioLogStream);
  }

  // //////////////////////////////////////////////////////////////////////
  DSIM_Service::DSIM_Service ()
    : _dsimServiceContext (NULL) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  DSIM_Service::DSIM_Service (const DSIM_Service& iService) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  DSIM_Service::~DSIM_Service () {
    // Delete/Clean all the objects from memory
    finalise();
  }

  // //////////////////////////////////////////////////////////////////////
  void logInit (const LOG::EN_LogLevel iLogLevel,
                std::ostream& ioLogOutputFile) {
    Logger::instance().setLogParameters (iLogLevel, ioLogOutputFile);
  }

  // //////////////////////////////////////////////////////////////////////
  void DSIM_Service::init (const std::string& iScheduleInputFilename,
                           std::ostream& ioLogStream) {
    // Set the log file
    logInit (LOG::DEBUG, ioLogStream);

    // Initialise the context
    DSIM_ServiceContext& lDSIM_ServiceContext =
      FacDsimServiceContext::instance().create ();
    _dsimServiceContext = &lDSIM_ServiceContext;

    // TODO: do not hardcode the CRS code (e.g., take it from a
    // configuration file).
    // Initialise the SIMCRS service handler
    const SIMCRS::CRSCode_T lCRSCode = "1S";
    SIMCRS_ServicePtr_T lSIMCRS_Service =
      SIMCRS_ServicePtr_T (new SIMCRS::SIMCRS_Service (ioLogStream, lCRSCode,
                                                       iScheduleInputFilename));
    lDSIM_ServiceContext.setSIMCRS_Service (lSIMCRS_Service);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void DSIM_Service::finalise () {
    assert (_dsimServiceContext != NULL);
  }

  // //////////////////////////////////////////////////////////////////////
  void DSIM_Service::simulate() {
    
    if (_dsimServiceContext == NULL) {
      throw NonInitialisedServiceException();
    }
    assert (_dsimServiceContext != NULL);
    DSIM_ServiceContext& lDSIM_ServiceContext= *_dsimServiceContext;

    try {

      // Get a reference on the SIMCRS service handler
      SIMCRS::SIMCRS_Service& lSIMCRS_Service =
        lDSIM_ServiceContext.getSIMCRS_Service();
      
      // Delegate the booking to the dedicated command
      BasChronometer lSimulationChronometer;
      lSimulationChronometer.start();
      Simulator::simulate (lSIMCRS_Service);
      const double lSimulationMeasure = lSimulationChronometer.elapsed();
      
      // DEBUG
      DSIM_LOG_DEBUG ("Simulation: " << lSimulationMeasure << " - "
                        << lDSIM_ServiceContext.display());

    } catch (const std::exception& error) {
      DSIM_LOG_ERROR ("Exception: "  << error.what());
      throw SimulationException();
    }
  }
  
}
