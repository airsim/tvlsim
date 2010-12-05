// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// Boost
#include <boost/make_shared.hpp>
// StdAir
#include <stdair/basic/BasChronometer.hpp>
#include <stdair/basic/BasFileMgr.hpp>
#include <stdair/bom/BomManager.hpp>
#include <stdair/service/Logger.hpp>
#include <stdair/stdair_exceptions.hpp>
#include <stdair/STDAIR_Service.hpp>
// SimLFS
#include <simlfs/basic/BasConst_SIMLFS_Service.hpp>
#include <simlfs/command/LowFareSearchManager.hpp>
#include <simlfs/factory/FacSimlfsServiceContext.hpp>
#include <simlfs/service/SIMLFS_ServiceContext.hpp>
#include <simlfs/SIMLFS_Service.hpp>

namespace SIMLFS {

  // //////////////////////////////////////////////////////////////////////
  SIMLFS_Service::SIMLFS_Service ()
    : _simlfsServiceContext (NULL) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  SIMLFS_Service::SIMLFS_Service (const SIMLFS_Service& iService) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  SIMLFS_Service::
  SIMLFS_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr,
                    const stdair::Filename_T& iFareInputFilename)
    : _simlfsServiceContext (NULL) {

    // Initialise the service context
    initServiceContext ();
    
    // Retrieve the Simlfs service context
    assert (_simlfsServiceContext != NULL);
    SIMLFS_ServiceContext& lSIMLFS_ServiceContext =
      *_simlfsServiceContext;
    
    // Store the STDAIR service object within the (SIMLFS) service context
    lSIMLFS_ServiceContext.setSTDAIR_Service (ioSTDAIR_ServicePtr);
    
    // Initialise the context
    init (iFareInputFilename);
  }

  // ////////////////////////////////////////////////////////////////////
  SIMLFS_Service::
  SIMLFS_Service (const stdair::BasLogParams& iLogParams,
                    const stdair::BasDBParams& iDBParams,
                    const stdair::Filename_T& iFareInputFilename) 
    : _simlfsServiceContext (NULL) {
    
    // Initialise the service context
    initServiceContext ();
    
    // Initialise the STDAIR service handler
    initStdAirService (iLogParams, iDBParams);
    
    // Initialise the (remaining of the) context
    init (iFareInputFilename);
  }

  // ////////////////////////////////////////////////////////////////////
  SIMLFS_Service::
  SIMLFS_Service (const stdair::BasLogParams& iLogParams,
                    const stdair::Filename_T& iFareInputFilename) 
    : _simlfsServiceContext (NULL) {
    
    // Initialise the service context
    initServiceContext ();
    
    // Initialise the STDAIR service handler
    initStdAirService (iLogParams);
    
    // Initialise the (remaining of the) context
    init (iFareInputFilename);
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
    SIMLFS_ServiceContext& lSIMLFS_ServiceContext =
      *_simlfsServiceContext;
    
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
    SIMLFS_ServiceContext& lSIMLFS_ServiceContext =
      *_simlfsServiceContext;
    
    // Initialise the STDAIR service handler
    // Note that the track on the object memory is kept thanks to the Boost
    // Smart Pointers component.
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams);

    // Store the STDAIR service object within the (SIMLFS) service context
    lSIMLFS_ServiceContext.setSTDAIR_Service (lSTDAIR_Service_ptr);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void SIMLFS_Service::
  init (const stdair::Filename_T& iFareInputFilename) {

    // Check that the file path given as input corresponds to an actual file
    const bool doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (iFareInputFilename);
    if (doesExistAndIsReadable == false) {
      STDAIR_LOG_ERROR ("The fare input file, '" << iFareInputFilename
                        << "', can not be retrieved on the file-system");
      throw stdair::FileNotFoundException ("The fare input file, '"
                                           + iFareInputFilename
                                           + "', can not be retrieved on the file-system");
    }

    // Retrieve the Simlfs service context
    assert (_simlfsServiceContext != NULL);
    SIMLFS_ServiceContext& lSIMLFS_ServiceContext = *_simlfsServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lSIMLFS_ServiceContext.getSTDAIR_Service();
    assert (lSTDAIR_Service_ptr != NULL);
    
    // Get the root of the BOM tree, on which all of the other BOM objects
    // will be attached
    stdair::BomRoot& lBomRoot = lSTDAIR_Service_ptr->getBomRoot();
  }

  // //////////////////////////////////////////////////////////////////////
  Price_T SIMLFS_Service::priceQuote (const AirlineCode_T& iAirlineCode,
                                      const PartySize_T& iPartySize) {
    Price_T oPrice = 0.0;
    
    if (_simlfsServiceContext == NULL) {
      throw NonInitialisedServiceException();
    }
    assert (_simlfsServiceContext != NULL);
    SIMLFS_ServiceContext& lSIMLFS_ServiceContext = *_simlfsServiceContext;

    try {
      
      // Delegate the price quotation to the dedicated command
      stdair::BasChronometer lPriceQuotingChronometer;
      lPriceQuotingChronometer.start();
      // oPrice = LowFareSearchManager::priceQuote (iAirlineCode, iPartySize);
      // const double lPriceQuotingMeasure = lPriceQuotingChronometer.elapsed();
      
      // // DEBUG
      // STDAIR_LOG_DEBUG ("Price quoting: " << lPriceQuotingMeasure << " - "
      //                   << lSIMLFS_ServiceContext.display());

    } catch (const std::exception& error) {
      STDAIR_LOG_ERROR ("Exception: "  << error.what());
      throw QuotingException();
    }

    return oPrice;
  }
  
}
