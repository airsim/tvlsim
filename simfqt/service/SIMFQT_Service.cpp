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
#include <stdair/STDAIR_Service.hpp>
#include <stdair/bom/TravelSolutionStruct.hpp>
// Simfqt
#include <simfqt/basic/BasConst_SIMFQT_Service.hpp>
#include <simfqt/factory/FacSimfqtServiceContext.hpp>
#include <simfqt/command/FareParser.hpp>
#include <simfqt/command/FareQuoter.hpp>
#include <simfqt/service/SIMFQT_ServiceContext.hpp>
#include <simfqt/SIMFQT_Service.hpp>

namespace SIMFQT {

  // //////////////////////////////////////////////////////////////////////
  SIMFQT_Service::SIMFQT_Service ()
    : _simfqtServiceContext (NULL) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  SIMFQT_Service::SIMFQT_Service (const SIMFQT_Service& iService) {
    assert (false);
  }

  // ////////////////////////////////////////////////////////////////////
  SIMFQT_Service::
  SIMFQT_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr,
                    const stdair::Filename_T& iFareInputFilename)
    : _simfqtServiceContext (NULL) {

    // Initialise the service context
    initServiceContext ();
    
    // Retrieve the Simfqt service context
    assert (_simfqtServiceContext != NULL);
    SIMFQT_ServiceContext& lSIMFQT_ServiceContext =
      *_simfqtServiceContext;
    
    // Store the STDAIR service object within the (SIMFQT) service context
    lSIMFQT_ServiceContext.setSTDAIR_Service (ioSTDAIR_ServicePtr);
    
    // Initialise the context
    init (iFareInputFilename);
  }

  // ////////////////////////////////////////////////////////////////////
  SIMFQT_Service::
  SIMFQT_Service (const stdair::BasLogParams& iLogParams,
                    const stdair::BasDBParams& iDBParams,
                    const stdair::Filename_T& iFareInputFilename) 
    : _simfqtServiceContext (NULL) {
    
    // Initialise the service context
    initServiceContext ();
    
    // Initialise the STDAIR service handler
    initStdAirService (iLogParams, iDBParams);
    
    // Initialise the (remaining of the) context
    init (iFareInputFilename);
  }

  // ////////////////////////////////////////////////////////////////////
  SIMFQT_Service::
  SIMFQT_Service (const stdair::BasLogParams& iLogParams,
                    const stdair::Filename_T& iFareInputFilename) 
    : _simfqtServiceContext (NULL) {
    
    // Initialise the service context
    initServiceContext ();
    
    // Initialise the STDAIR service handler
    initStdAirService (iLogParams);
    
    // Initialise the (remaining of the) context
    init (iFareInputFilename);
  }


  // //////////////////////////////////////////////////////////////////////
  SIMFQT_Service::~SIMFQT_Service () {
    // Delete/Clean all the objects from memory
    finalise();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void SIMFQT_Service::finalise () {
    assert (_simfqtServiceContext != NULL);
  }

  // //////////////////////////////////////////////////////////////////////
  void SIMFQT_Service::initServiceContext () {
    // Initialise the service context
    SIMFQT_ServiceContext& lSIMFQT_ServiceContext = 
      FacSimfqtServiceContext::instance().create ();
    _simfqtServiceContext = &lSIMFQT_ServiceContext;
  }

  // //////////////////////////////////////////////////////////////////////
  void SIMFQT_Service::
  initStdAirService (const stdair::BasLogParams& iLogParams,
                     const stdair::BasDBParams& iDBParams) {

    // Retrieve the Simfqt service context
    assert (_simfqtServiceContext != NULL);
    SIMFQT_ServiceContext& lSIMFQT_ServiceContext =
      *_simfqtServiceContext;
    
    // Initialise the STDAIR service handler
    // Note that the track on the object memory is kept thanks to the Boost
    // Smart Pointers component.
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams, iDBParams);
    assert (lSTDAIR_Service_ptr != NULL);
    
    // Store the STDAIR service object within the (SIMFQT) service context
    lSIMFQT_ServiceContext.setSTDAIR_Service (lSTDAIR_Service_ptr);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void SIMFQT_Service::
  initStdAirService (const stdair::BasLogParams& iLogParams) {

    // Retrieve the Simfqt service context
    assert (_simfqtServiceContext != NULL);
    SIMFQT_ServiceContext& lSIMFQT_ServiceContext =
      *_simfqtServiceContext;
    
    // Initialise the STDAIR service handler
    // Note that the track on the object memory is kept thanks to the Boost
    // Smart Pointers component.
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr = 
      boost::make_shared<stdair::STDAIR_Service> (iLogParams);
    assert (lSTDAIR_Service_ptr != NULL);

    // Store the STDAIR service object within the (SIMFQT) service context
    lSIMFQT_ServiceContext.setSTDAIR_Service (lSTDAIR_Service_ptr);
  }
  
  // ////////////////////////////////////////////////////////////////////
  void SIMFQT_Service::
  init (const stdair::Filename_T& iFareInputFilename) {

    // Check that the file path given as input corresponds to an actual file
    const bool doesExistAndIsReadable =
      stdair::BasFileMgr::doesExistAndIsReadable (iFareInputFilename);
    if (doesExistAndIsReadable == false) {
      STDAIR_LOG_ERROR ("The fare input file, '" << iFareInputFilename
                        << "', can not be retrieved on the file-system");
      throw FareInputFileNotFoundException ("The demand file '"
                                              + iFareInputFilename
                                              + "' does not exist or can not be read");
    }

    // Retrieve the Simfqt service context
    assert (_simfqtServiceContext != NULL);
    SIMFQT_ServiceContext& lSIMFQT_ServiceContext =
      *_simfqtServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lSIMFQT_ServiceContext.getSTDAIR_Service();
    assert (lSTDAIR_Service_ptr != NULL);
    
    // Get the root of the BOM tree, on which all of the other BOM objects
    // will be attached
    stdair::BomRoot& lBomRoot = lSTDAIR_Service_ptr->getBomRoot();

    // Initialise the fare parser
    FareParser::fareRuleGeneration (iFareInputFilename, lBomRoot);
  }

  // ////////////////////////////////////////////////////////////////////
  void SIMFQT_Service::
  getFares (stdair::TravelSolutionList_T& ioTravelSolutionList) {

    // Retrieve the Simfqt service context
    assert (_simfqtServiceContext != NULL);
    SIMFQT_ServiceContext& lSIMFQT_ServiceContext =
      *_simfqtServiceContext;

    // Retrieve the StdAir service context
    stdair::STDAIR_ServicePtr_T lSTDAIR_Service_ptr =
      lSIMFQT_ServiceContext.getSTDAIR_Service();
    assert (lSTDAIR_Service_ptr != NULL);
    
    // Get the root of the BOM tree, on which all of the other BOM objects
    // will be attached
    stdair::BomRoot& lBomRoot = lSTDAIR_Service_ptr->getBomRoot();

    // Initialise the fare parser  
    for (stdair::TravelSolutionList_T::iterator itTravelSolution =
           ioTravelSolutionList.begin();
         itTravelSolution != ioTravelSolutionList.end(); ++itTravelSolution) {
      FareQuoter::priceQuote (*itTravelSolution, lBomRoot);
    }
  }
  
}
