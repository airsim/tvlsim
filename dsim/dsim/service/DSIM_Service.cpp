// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <ostream>
// StdAir
#include <stdair/basic/BasChronometer.hpp>
#include <stdair/bom/BomManager.hpp> // for display()
#include <stdair/bom/AirlineFeature.hpp>
#include <stdair/bom/AirlineFeatureSet.hpp>
#include <stdair/bom/BomRoot.hpp>
#include <stdair/factory/FacBomContent.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/STDAIR_Service.hpp>
// Distribution
#include <simcrs/SIMCRS_Service.hpp>
// TRADEMGEN
#include <trademgen/TRADEMGEN_Service.hpp>
#include <trademgen/DBParams.hpp>
// Dsim
#include <dsim/basic/BasConst_DSIM_Service.hpp>
#include <dsim/command/Simulator.hpp>
#include <dsim/factory/FacDsimServiceContext.hpp>
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
  DSIM_Service::DSIM_Service (const stdair::Filename_T& iScheduleInputFilename)
    : _dsimServiceContext (NULL) {
    
    // Initialise the service context
    initServiceContext ();
    // Initialise the context
    //init (iScheduleInputFilename);

    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  DSIM_Service::DSIM_Service (const stdair::BasLogParams& iLogParams,
                              const stdair::Filename_T& iScheduleInputFilename)
    : _dsimServiceContext (NULL) {
    
    // Initialise the service context
    initServiceContext ();
    
    // Initialise the STDAIR service handler
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      initStdAirService (iLogParams);
    
    // Initialise the (remaining of the) context
    init (lSTDAIR_Service_ptr, iScheduleInputFilename);
  }

  // //////////////////////////////////////////////////////////////////////
  DSIM_Service::~DSIM_Service () {
    // Delete/Clean all the objects from memory
    finalise();
  }

  // ////////////////////////////////////////////////////////////////////
  void DSIM_Service::initServiceContext () {
    // Initialise the context
    DSIM_ServiceContext& lDSIM_ServiceContext =
      FacDsimServiceContext::instance().create ();
    _dsimServiceContext = &lDSIM_ServiceContext;
  }

  // //////////////////////////////////////////////////////////////////////
  stdair::STDAIR_ServicePtr_T DSIM_Service::
  initStdAirService (const stdair::BasLogParams& iLogParams) {

    // Retrieve the Dsim service context
    assert (_dsimServiceContext != NULL);
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;
    
    // Initialise the STDAIR service handler
    // Note that the track on the object memory is kept thanks to the Boost
    // Smart Pointers component.
    stdair::STDAIR_ServicePtr_T oSTDAIR_Service_ptr = 
      stdair::STDAIR_ServicePtr_T (new stdair::STDAIR_Service (iLogParams));

    // Retrieve the root of the BOM tree, on which all of the other BOM objects
    // will be attached
    assert (oSTDAIR_Service_ptr != NULL);
    stdair::BomRoot& lBomRoot = oSTDAIR_Service_ptr->getBomRoot();

    // TODO: do not hardcode the initialisation of AirlineFeatureSet
    // Initialise the set of required airline features
    stdair::AirlineFeatureSet& lAirlineFeatureSet =
      stdair::FacBomContent::instance().create<stdair::AirlineFeatureSet>();
    
    // Airline code
    stdair::AirlineCode_T lAirlineCode ("BA");
    // Initialise an AirlineFeature object
    stdair::AirlineFeatureKey_T lAirlineFeatureKey (lAirlineCode);
    stdair::AirlineFeature& lAirlineFeature = stdair::FacBomContent::
      instance().create<stdair::AirlineFeature> (lAirlineFeatureKey);
    stdair::FacBomContent::
      linkWithParent<stdair::AirlineFeature> (lAirlineFeature,
                                              lAirlineFeatureSet);

    // Set the AirlineFeatureSet for the BomRoot.
    lBomRoot.setAirlineFeatureSet (&lAirlineFeatureSet);

    return oSTDAIR_Service_ptr;
  }
  
  // //////////////////////////////////////////////////////////////////////
  void DSIM_Service::init (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr,
                           const stdair::Filename_T& iScheduleInputFilename) {
    // Retrieve the service context
    assert (_dsimServiceContext != NULL);
    DSIM_ServiceContext& lDSIM_ServiceContext = *_dsimServiceContext;
    
    // TODO: do not hardcode the CRS code (e.g., take it from a
    // configuration file).
    // Initialise the SIMCRS service handler
    const SIMCRS::CRSCode_T lCRSCode = "1S";
    
    // Note that the (Boost.)Smart Pointer keeps track of the references
    // on the Service object, and deletes that object when it is no longer
    // referenced (e.g., at the end of the process).
    SIMCRS_ServicePtr_T lSIMCRS_Service =
      SIMCRS_ServicePtr_T (new SIMCRS::SIMCRS_Service (ioSTDAIR_ServicePtr,
                                                       lCRSCode,
                                                       iScheduleInputFilename));
    lDSIM_ServiceContext.setSIMCRS_Service (lSIMCRS_Service);

    // TODO: do not hardcode the DBParams.
    // Initialise the TRADEMGEN service handler
    const TRADEMGEN::DBParams lDBParams =
      TRADEMGEN::DBParams ("anguyen", "anguyen", "ncemysqlp.nce.amadeus.net",
                           "3321", "sim_anguyen");
    TRADEMGEN_ServicePtr_T lTRADEMGEN_Service =
      TRADEMGEN_ServicePtr_T (new TRADEMGEN::TRADEMGEN_Service (lDBParams));
    lDSIM_ServiceContext.setTRADEMGEN_Service (lTRADEMGEN_Service);
    
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
      // Get a reference on the TRADEMGEN service handler
      TRADEMGEN::TRADEMGEN_Service& lTRADEMGEN_Service =
        lDSIM_ServiceContext.getTRADEMGEN_Service();
      
      // Delegate the booking to the dedicated command
      stdair::BasChronometer lSimulationChronometer;
      lSimulationChronometer.start();
      Simulator::simulate (lSIMCRS_Service, lTRADEMGEN_Service);
      const double lSimulationMeasure = lSimulationChronometer.elapsed();
      
      // DEBUG
      STDAIR_LOG_DEBUG ("Simulation: " << lSimulationMeasure << " - "
                        << lDSIM_ServiceContext.display());

    } catch (const std::exception& error) {
      STDAIR_LOG_ERROR ("Exception: "  << error.what());
      throw SimulationException();
    }
  }
  
}
