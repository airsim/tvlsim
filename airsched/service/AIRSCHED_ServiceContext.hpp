#ifndef __AIRSCHED_SVC_AIRSCHED_SERVICE_CONTEXT_HPP
#define __AIRSCHED_SVC_AIRSCHED_SERVICE_CONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// Boost
#include <boost/shared_ptr.hpp>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_service_types.hpp>
// AirSched
#include <airsched/AIRSCHED_Types.hpp>
#include <airsched/service/ServiceAbstract.hpp>

namespace AIRSCHED {

  /** Inner class holding the context for the AIRSCHED Service object. */
  class AIRSCHED_ServiceContext : public ServiceAbstract {
    /** The AIRSCHED_Service class should be the sole class to get access to
        ServiceContext content: general users do not want to bother
        with a context interface. */
    friend class AIRSCHED_Service;
    friend class FacAIRSCHEDServiceContext;

  private:
    /// //////////////// Constructors and destructors /////////////
    /** Default constructor. */
    AIRSCHED_ServiceContext ();
    /** Default copy constructor. */
    AIRSCHED_ServiceContext (const AIRSCHED_ServiceContext&);
    /** Initialisation. */
    void init ();
    /** Destructor. */
    ~AIRSCHED_ServiceContext();

    /** Initialise the StudyStatManager. */
    void initStudyStatManager ();

    
    // ///////////////// Getters ///////////////////
    /** Get the pointer on the STDAIR service handler. */
    stdair::STDAIR_ServicePtr_T getSTDAIR_Service () const {
      return _stdairService;
    }
        
    // ///////////////// Setters ///////////////////
    /** Set the pointer on the STDAIR service handler. */
    void setSTDAIR_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr) {
      _stdairService = ioSTDAIR_ServicePtr;
    } 
    
    // ///////// Display Methods //////////
    /** Display the short SIMCRS_ServiceContext content. */
    const std::string shortDisplay() const;
    
    /** Display the full SIMCRS_ServiceContext content. */
    const std::string display() const;
        
    /** Read the input data from a file */
    void readFromInputFile (const stdair::Filename_T& iInputFileName);


  private:
    // ///////////// Children ////////////
    /** Standard Airline (StdAir) Service Handler. */
    stdair::STDAIR_ServicePtr_T _stdairService;
  };

}
#endif // __AIRSCHED_SVC_AIRSCHED_SERVICE_CONTEXT_HPP
