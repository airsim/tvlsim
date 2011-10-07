// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// Boost
#include <boost/make_shared.hpp>
// StdAir
#include <stdair/stdair_exceptions.hpp>
#include <stdair/stdair_basic_types.hpp>
#include <stdair/basic/BasChronometer.hpp>
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/STDAIR_Service.hpp>
// Airline Inventory
#include <airinv/AIRINV_Master_Service.hpp>
//Fare Quote
#include <simfqt/SIMFQT_Service.hpp>
// SimLFS
#include <simlfs/basic/BasConst_SIMLFS_Service.hpp>
//#include <simlfs/command/LowYieldSearchManager.hpp>
#include <simlfs/factory/FacSimlfsServiceContext.hpp>
#include <simlfs/service/SIMLFS_ServiceContext.hpp>
#include <simlfs/SIMLFS_Service.hpp>

namespace SIMLFS {

  // //////////////////////////////////////////////////////////////////////
  SIMLFS_Service::SIMLFS_Service () : _simlfsServiceContext (NULL) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  SIMLFS_Service::SIMLFS_Service (const SIMLFS_Service& iService) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  SIMLFS_Service::
  SIMLFS_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr,
                  const stdair::Filename_T& iScheduleInputFilename,
                  const stdair::Filename_T& iODInputFilename,
                  const SIMFQT::FareFilePath& iFareInputFilepath,
                  const AIRRAC::YieldFilePath& iYieldInputFilepath)
    : _simlfsServiceContext (NULL) {

    // Initialise the service context
    initServiceContext ();
    
    // Retrieve the Simlfs service context
    assert (_simlfsServiceContext != NULL);
    SIMLFS_ServiceContext& lSIMLFS_ServiceContext = *_simlfsServiceContext;
    
    // Store the STDAIR service object within the (SIMLFS) service context
    lSIMLFS_ServiceContext.setSTDAIR_Service (ioSTDAIR_ServicePtr);
    
    // Initialise the context
    init (iScheduleInputFilename, iODInputFilename, iFareInputFilepath,
          iYieldInputFilepath);
  }

  // ////////////////////////////////////////////////////////////////////
  SIMLFS_Service::
  SIMLFS_Service (const stdair::BasLogParams& iLogParams,
                  const stdair::BasDBParams& iDBParams,
                  const stdair::Filename_T& iScheduleInputFilename,
                  const stdair::Filename_T& iODInputFilename,
                  const SIMFQT::FareFilePath& iFareInputFilepath,
                  const AIRRAC::YieldFilePath& iYieldInputFilepath) 
    : _simlfsServiceContext (NULL) {
    
    // Initialise the service context
    initServiceContext ();
    
    // Initialise the STDAIR service handler
    initStdAirService (iLogParams, iDBParams);
    
    // Initialise the (remaining of the) context
    init (iScheduleInputFilename, iODInputFilename, iFareInputFilepath,
          iYieldInputFilepath);
  }

  // ////////////////////////////////////////////////////////////////////
  SIMLFS_Service::
  SIMLFS_Service (const stdair::BasLogParams& iLogParams,
                  const stdair::Filename_T& iScheduleInputFilename,
                  const stdair::Filename_T& iODInputFilename,
                  const SIMFQT::FareFilePath& iFareInputFilepath,
                  const AIRRAC::YieldFilePath& iYieldInputFilepath) 
    : _simlfsServiceContext (NULL) {
    
    // Initialise the service context
    initServiceContext ();
    
    // Initialise the STDAIR service handler
    initStdAirService (iLogParams);
    
    // Initialise the (remaining of the) context
    init (iScheduleInputFilename, iODInputFilename, iFareInputFilepath,
          iYieldInputFilepath);
  }

  // //////////////////////////////////////////////////////////////////////
  SIMLFS_Service::~SIMLFS_Service () {
    // Delete/Clean all the objects from memory
    finalise();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void SIMLFS_Service::finalise () {
    assert (_simlfsServiceContext != NULL);
  }

  // //////////////////////////////////////////////////////////////////////
  void SIMLFS_Service::initServiceContext () {
    // Initialise the service context
    SIMLFS_ServiceContext& lSIMLFS_ServiceContext = 
      FacSimlfsServiceContext::instance().create ();
    _simlfsServiceContext = &lSIMLFS_ServiceContext;
  }

  // //////////////////////////////////////////////////////////////////////
  void SIMLFS_Service::
  initStdAirService (const stdair::BasLogParams& iLogParams,
                     const stdair::BasDBParams& iDBParams) {

    // Retrieve the Simlfs service context
    assert (_simlfsServiceContext != NULL);
    SIMLFS_ServiceContext& lSIMLFS_ServiceContext = *_simlfsServiceContext;
    
    // Initialise the STDAIR service handler
    // Note that the track on the object memory is kept thanks to the Boost
    // Smart Pointers component.
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams, iDBParams);

    // Store the STDAIR service object within the (SIMLFS) service context
    lSIMLFS_ServiceContext.setSTDAIR_Service (lSTDAIR_Service_ptr);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void SIMLFS_Service::
  initStdAirService (const stdair::BasLogParams& iLogParams) {

    // Retrieve the Simlfs service context
    assert (_simlfsServiceContext != NULL);
    SIMLFS_ServiceContext& lSIMLFS_ServiceContext = *_simlfsServiceContext;
    
    // Initialise the STDAIR service handler
    // Note that the track on the object memory is kept thanks to the Boost
    // Smart Pointers component.
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams);

    // Store the STDAIR service object within the (SIMLFS) service context
    lSIMLFS_ServiceContext.setSTDAIR_Service (lSTDAIR_Service_ptr);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void SIMLFS_Service::init (const stdair::Filename_T& iScheduleInputFilename,
                             const stdair::Filename_T& iODInputFilename,
                             const SIMFQT::FareFilePath& iFareInputFilepath,
                             const AIRRAC::YieldFilePath& iYieldInputFilepath) {
    
    const stdair::Filename_T lFareFilename = iFareInputFilepath.name();

    // Check that the file path given as input corresponds to an actual file
    const bool doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (lFareFilename);
    if (doesExistAndIsReadable == false) {
      STDAIR_LOG_ERROR ("The fare input file, '" << lFareFilename
                        << "', can not be retrieved on the file-system");
      throw stdair::FileNotFoundException ("The fare input file, '"
                                           + lFareFilename
                                           + "', can not be retrieved on the "
                                           + "file-system");
    }

    // Initialise the children AirInv service context
    initAIRINV_Master_Service (iScheduleInputFilename, iODInputFilename,
                               iYieldInputFilepath);

    // Initialise the children SimFQT service context
    initSIMFQTService (iFareInputFilepath);
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMLFS_Service::
  initSIMFQTService (const SIMFQT::FareFilePath& iFareInputFilepath) {
    
    // Retrieve the SimLFS service context
    assert (_simlfsServiceContext != NULL);
    SIMLFS_ServiceContext& lSIMLFS_ServiceContext = *_simlfsServiceContext;
    
    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lSIMLFS_ServiceContext.getSTDAIR_Service();
    assert (lSTDAIR_Service_ptr != NULL);

    // Initialise the SIMFQT service handler
    // Note that the (Boost.)Smart Pointer keeps track of the references
    // on the Service object, and deletes that object when it is no longer
    // referenced (e.g., at the end of the process).
    SIMFQT_ServicePtr_T lSIMFQT_Service_ptr =
      boost::make_shared<SIMFQT::SIMFQT_Service> (lSTDAIR_Service_ptr);

    // Store the Simfqt service object within the (SimLFS) service context
    lSIMLFS_ServiceContext.setSIMFQT_Service (lSIMFQT_Service_ptr); 

    // Parse the fare input file and load its content into memory
    lSIMFQT_Service_ptr->parseAndLoad (iFareInputFilepath);
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMLFS_Service::
  initAIRINV_Master_Service (const stdair::Filename_T& iScheduleInputFilename,
                             const stdair::Filename_T& iODInputFilename,
                             const AIRRAC::YieldFilePath& iYieldInputFilepath) {
    
    // Retrieve the SimLFS service context
    assert (_simlfsServiceContext != NULL);
    SIMLFS_ServiceContext& lSIMLFS_ServiceContext = *_simlfsServiceContext;
    
    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lSIMLFS_ServiceContext.getSTDAIR_Service();
    assert (lSTDAIR_Service_ptr != NULL);

    // Initialise the AIRINV service handler
    // Note that the (Boost.)Smart Pointer keeps track of the references
    // on the Service object, and deletes that object when it is no longer
    // referenced (e.g., at the end of the process).
    AIRINV_Master_ServicePtr_T lAIRINV_Master_Service_ptr =
      boost::make_shared<AIRINV::AIRINV_Master_Service> (lSTDAIR_Service_ptr);

    // Parse and load the schedule and O&D input files
    lAIRINV_Master_Service_ptr->parseAndLoad (iScheduleInputFilename,
                                              iODInputFilename,
                                              iYieldInputFilepath);

    // Store the Airinv service object within the (SimLFS) service context
    lSIMLFS_ServiceContext.setAIRINV_Master_Service(lAIRINV_Master_Service_ptr);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void SIMLFS_Service::
  fareQuote (const stdair::BookingRequestStruct& iBookingRequest,
             stdair::TravelSolutionList_T& ioTravelSolutionList) {
     
    if (_simlfsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimLFS service has "
                                                    "not been initialised");
    }
    assert (_simlfsServiceContext != NULL);
    SIMLFS_ServiceContext& lSIMLFS_ServiceContext = *_simlfsServiceContext;

    stdair::TravelSolutionList_T oTravelSolutionList;

    // Get a reference on the SIMFQT service handler
    SIMFQT_ServicePtr_T lSIMFQT_Service_ptr =
      lSIMLFS_ServiceContext.getSIMFQT_Service();  
    assert (lSIMFQT_Service_ptr != NULL);

    // Delegate the action to the dedicated command
    stdair::BasChronometer lFareQuoteRetrievalChronometer;
    lFareQuoteRetrievalChronometer.start();
    lSIMFQT_Service_ptr->quotePrices (iBookingRequest, ioTravelSolutionList);

    // DEBUG 
    const double lFareQuoteRetrievalMeasure =
      lFareQuoteRetrievalChronometer.elapsed(); 
    STDAIR_LOG_DEBUG ("Fare Quote retrieving: " << lFareQuoteRetrievalMeasure
                      << " - " << lSIMLFS_ServiceContext.display());   
  }
  
  // ////////////////////////////////////////////////////////////////////
  void SIMLFS_Service::
  calculateAvailability (stdair::TravelSolutionList_T& ioTravelSolutionList) {
    if (_simlfsServiceContext == NULL) {
      throw stdair::NonInitialisedServiceException ("The SimLFS service has "
                                                    "not been initialised");
    }
    assert (_simlfsServiceContext != NULL);

    //SIMLFS_ServiceContext& lSIMLFS_ServiceContext = *_simlfsServiceContext;
  }
  
}
