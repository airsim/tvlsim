// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// Boost
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
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
  DSIM_Service (std::ostream& ioLogStream)
    : _dsimServiceContext (NULL) {
    init (ioLogStream);
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
  void DSIM_Service::init (std::ostream& ioLogStream) {
    // Set the log file
    logInit (LOG::DEBUG, ioLogStream);

    // Initialise the context
    DSIM_ServiceContext& lDSIM_ServiceContext =
      FacDsimServiceContext::instance().create ();
    _dsimServiceContext = &lDSIM_ServiceContext;
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
      
      // Delegate the booking to the dedicated command
      BasChronometer lSimulationChronometer;
      lSimulationChronometer.start();
      Simulator::simulate();
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
