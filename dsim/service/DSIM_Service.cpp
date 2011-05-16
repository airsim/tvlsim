// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// Boost
#include <boost/make_shared.hpp>
// SOCI
#include <soci/core/soci.h>
// StdAir
#include <stdair/basic/BasChronometer.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/bom/AirlineStruct.hpp>
#include <stdair/bom/AirlineFeature.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/service/DBSessionManager.hpp>
#include <stdair/STDAIR_Service.hpp>
// Distribution
#include <simcrs/SIMCRS_Service.hpp>
// TraDemGen
#include <trademgen/TRADEMGEN_Service.hpp>
// Travelccm
#include <travelccm/TRAVELCCM_Service.hpp>
// Dsim
#include <dsim/basic/BasConst_DSIM_Service.hpp>
#include <dsim/factory/FacDsimServiceContext.hpp>
#include <dsim/command/DBManager.hpp>
#include <dsim/command/Simulator.hpp>
#include <dsim/service/DSIM_ServiceContext.hpp>
#include <dsim/DSIM_Service.hpp>

namespace DSIM {

  // //////////////////////////////////////////////////////////////////////
  DSIM_Service::DSIM_Service () : _dsimServiceContext (NULL) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  DSIM_Service::DSIM_Service (const DSIM_Service& iService) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  DSIM_Service::DSIM_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr,
                              const stdair::Date_T& iStartDate,
                              const stdair::Date_T& iEndDate,
                              const stdair::Filename_T& iScheduleInputFilename,
                              const stdair::Filename_T& iODInputFilename,
                              const stdair::Filename_T& iFareInputFilename,
                              const stdair::Filename_T& iYieldInputFilename,
                              const stdair::Filename_T& iDemandInputFilename)
    : _dsimServiceContext (NULL) {
    
    // Initialise the service context
    initServiceContext (iStartDate, iEndDate);
    
    // Retrieve the DSim service context
    assert (_dsimServiceContext != NULL);
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;
    
    // Store the STDAIR service object within the (DSim) service context
    lDSIM_ServiceContext.setSTDAIR_Service (ioSTDAIR_ServicePtr);
    
    // Initialise the context
    init (iScheduleInputFilename, iODInputFilename, iFareInputFilename,
          iYieldInputFilename, iDemandInputFilename);

    // Initialise the snapshot and RM events
    initSnapshotAndRMEvents (iStartDate, iEndDate);
  }

  // //////////////////////////////////////////////////////////////////////
  DSIM_Service::DSIM_Service (const stdair::BasLogParams& iLogParams,
                              const stdair::BasDBParams& iDBParams,
                              const stdair::Date_T& iStartDate,
                              const stdair::Date_T& iEndDate,
                              const stdair::Filename_T& iScheduleInputFilename,
                              const stdair::Filename_T& iODInputFilename,
                              const stdair::Filename_T& iFareInputFilename,
                              const stdair::Filename_T& iYieldInputFilename,
                              const stdair::Filename_T& iDemandInputFilename)
    : _dsimServiceContext (NULL) {
    
    // Initialise the service context
    initServiceContext (iStartDate, iEndDate);
    
    // Initialise the STDAIR service handler
    initStdAirService (iLogParams, iDBParams);
    
    // Initialise the (remaining of the) context
    init (iScheduleInputFilename, iODInputFilename, iFareInputFilename,
          iYieldInputFilename, iDemandInputFilename);

    // Initialise the snapshot and RM events
    initSnapshotAndRMEvents (iStartDate, iEndDate);
  }

  // //////////////////////////////////////////////////////////////////////
  DSIM_Service::~DSIM_Service () {
    // Delete/Clean all the objects from memory
    finalise();
  }

  // ////////////////////////////////////////////////////////////////////
  void DSIM_Service::initServiceContext (const stdair::Date_T& iStartDate,
                                         const stdair::Date_T& iEndDate) {
    // Initialise the context
    DSIM_ServiceContext& lDSIM_ServiceContext =
      FacDsimServiceContext::instance().create (iStartDate, iEndDate);
    _dsimServiceContext = &lDSIM_ServiceContext;
  }

  // //////////////////////////////////////////////////////////////////////
  void DSIM_Service::initStdAirService (const stdair::BasLogParams& iLogParams,
                                        const stdair::BasDBParams& iDBParams) {

    // Retrieve the Dsim service context
    assert (_dsimServiceContext != NULL);
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;
    
    // Initialise the STDAIR service handler
    // Note that the track on the object memory is kept thanks to the Boost
    // Smart Pointers component.
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams, iDBParams);

    // Retrieve the root of the BOM tree, on which all of the other BOM objects
    // will be attached
    assert (lSTDAIR_Service_ptr != NULL);

    // Store the STDAIR service object within the (DSim) service context
    lDSIM_ServiceContext.setSTDAIR_Service (lSTDAIR_Service_ptr);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void DSIM_Service::init (const stdair::Filename_T& iScheduleInputFilename,
                           const stdair::Filename_T& iODInputFilename,
                           const stdair::Filename_T& iFareInputFilename,
                           const stdair::Filename_T& iYieldInputFilename,
                           const stdair::Filename_T& iDemandInputFilename) {
    // Retrieve the service context
    assert (_dsimServiceContext != NULL);
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;
    
    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lDSIM_ServiceContext.getSTDAIR_Service_Ptr();
    assert (lSTDAIR_Service_ptr != NULL);
    
    // TODO: do not hardcode the CRS code (e.g., take it from a
    // configuration file).
    // Initialise the SIMCRS service handler
    const SIMCRS::CRSCode_T lCRSCode = "1S";
    
    // Note that the (Boost.)Smart Pointer keeps track of the references
    // on the Service object, and deletes that object when it is no longer
    // referenced (e.g., at the end of the process).
    SIMCRS_ServicePtr_T lSIMCRS_Service =
      boost::make_shared<SIMCRS::SIMCRS_Service> (lSTDAIR_Service_ptr, lCRSCode,
                                                  iScheduleInputFilename,
                                                  iODInputFilename,
                                                  iFareInputFilename,
                                                  iYieldInputFilename);
    lDSIM_ServiceContext.setSIMCRS_Service (lSIMCRS_Service);

    // Initialise the TRADEMGEN service handler
    TRADEMGEN_ServicePtr_T lTRADEMGEN_Service =
      boost::make_shared<TRADEMGEN::TRADEMGEN_Service> (lSTDAIR_Service_ptr,
                                                        iDemandInputFilename);
    lDSIM_ServiceContext.setTRADEMGEN_Service (lTRADEMGEN_Service);

    // Initialise the TRAVELCCM service handler
    TRAVELCCM_ServicePtr_T lTRAVELCCM_Service =
      boost::make_shared<TRAVELCCM::TRAVELCCM_Service> (lSTDAIR_Service_ptr);
    lDSIM_ServiceContext.setTRAVELCCM_Service (lTRAVELCCM_Service);
  }

  // ////////////////////////////////////////////////////////////////////
  void DSIM_Service::initSnapshotAndRMEvents (const stdair::Date_T& iStartDate,
                                              const stdair::Date_T& iEndDate) {
    // Retrieve the service context
    assert (_dsimServiceContext != NULL);
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;

    // Get a reference on the SIMCRS service handler
    SIMCRS::SIMCRS_Service& lSIMCRS_Service =
      lDSIM_ServiceContext.getSIMCRS_Service();

    lSIMCRS_Service.initSnapshotAndRMEvents (iStartDate, iEndDate);
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

    // Get a reference on the SIMCRS service handler
    SIMCRS::SIMCRS_Service& lSIMCRS_Service =
      lDSIM_ServiceContext.getSIMCRS_Service();

    // Get a reference on the TRADEMGEN service handler
    TRADEMGEN::TRADEMGEN_Service& lTRADEMGEN_Service =
      lDSIM_ServiceContext.getTRADEMGEN_Service();
    
    // Get a reference on the TRAVELCCM service handler
    TRAVELCCM::TRAVELCCM_Service& lTRAVELCCM_Service =
      lDSIM_ServiceContext.getTRAVELCCM_Service();

    // Get a reference on the STDAIR service handler
    stdair::STDAIR_Service& lSTDAIR_Service =
      lDSIM_ServiceContext.getSTDAIR_Service();

    // Delegate the booking to the dedicated command
    stdair::BasChronometer lSimulationChronometer;
    lSimulationChronometer.start();
    Simulator::simulate (lSIMCRS_Service, lTRADEMGEN_Service,
                         lTRAVELCCM_Service, lSTDAIR_Service);
    const double lSimulationMeasure = lSimulationChronometer.elapsed();

    // DEBUG
    STDAIR_LOG_DEBUG ("Simulation: " << lSimulationMeasure << " - "
                      << lDSIM_ServiceContext.display());
  }

  // //////////////////////////////////////////////////////////////////////
  void DSIM_Service::displayAirlineListFromDB () const {
    if (_dsimServiceContext == NULL) {
      throw NonInitialisedServiceException();
    }
    assert (_dsimServiceContext != NULL);
    //DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;

    // Get the date-time for the present time
    boost::posix_time::ptime lNowDateTime =
      boost::posix_time::second_clock::local_time();
    boost::gregorian::date lNowDate = lNowDateTime.date();

    // DEBUG
    STDAIR_LOG_DEBUG (std::endl
                      << "==================================================="
                      << std::endl
                      << lNowDateTime);

    // Delegate the query execution to the dedicated command
    stdair::BasChronometer lDsimChronometer;
    lDsimChronometer.start();

    // Retrieve the database session handler
    stdair::DBSession_T& lDBSession =
      stdair::DBSessionManager::instance().getDBSession();
    
    // Prepare and execute the select statement
    stdair::AirlineStruct lAirline;
    stdair::DBRequestStatement_T lSelectStatement (lDBSession);
    DBManager::prepareSelectStatement(lDBSession, lSelectStatement, lAirline);
    
    // Prepare the SQL request corresponding to the select statement
    bool hasStillData = true;
    unsigned int idx = 0;
    while (hasStillData == true) {
      hasStillData = DBManager::iterateOnStatement (lSelectStatement, lAirline);
      
      // DEBUG
      STDAIR_LOG_DEBUG ("[" << idx << "]: " << lAirline);
      
      // Iteration
      ++idx;
    }
    
    const double lDsimMeasure = lDsimChronometer.elapsed();
    
    // DEBUG
    STDAIR_LOG_DEBUG ("Sample service for Dsim: " << lDsimMeasure);
  }

}
