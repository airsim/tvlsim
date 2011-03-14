#ifndef __SIMCRS_SVC_SIMCRSSERVICECONTEXT_HPP
#define __SIMCRS_SVC_SIMCRSSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <map>
// Boost
#include <boost/shared_ptr.hpp>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_service_types.hpp>
// SimCRS
#include <simcrs/SIMCRS_Types.hpp>
#include <simcrs/service/ServiceAbstract.hpp>

namespace AIRINV {
  class AIRINV_Master_Service;
}

namespace AIRSCHED {
  class AIRSCHED_Service;
}

namespace SIMFQT {
  class SIMFQT_Service;
}

namespace SIMCRS {

  /** Pointer on the AIRINV Master Service handler. */
  typedef boost::shared_ptr<AIRINV::AIRINV_Master_Service> AIRINV_Master_ServicePtr_T;

  /** Pointer on the AIRSCHED Service handler. */
  typedef boost::shared_ptr<AIRSCHED::AIRSCHED_Service> AIRSCHED_ServicePtr_T;

  /** Pointer on the SIMFQT Service handler. */
  typedef boost::shared_ptr<SIMFQT::SIMFQT_Service> SIMFQT_ServicePtr_T;

  
  /**
   * @brief Class holding the context of the Simcrs services.
   */
  class SIMCRS_ServiceContext : public ServiceAbstract {
    /**
     * The SIMCRS_Service class should be the sole class to get access
     * to ServiceContext content: general users do not want to bother
     * with a context interface.
     */
    friend class SIMCRS_Service;
    friend class FacSimcrsServiceContext;
    
  private:
    /// //////////////// Constructors and destructors /////////////
    /** Constructor. */
    SIMCRS_ServiceContext (const CRSCode_T& iCRSCode);
    /** Default constructor (not to be used). */
    SIMCRS_ServiceContext ();
    /** Default copy constructor (not to be used). */
    SIMCRS_ServiceContext (const SIMCRS_ServiceContext&);

    /** Destructor. */
    ~SIMCRS_ServiceContext();
    
  private:
    // ///////////////// Getters ///////////////////
    /**
     * Get the pointer on the STDAIR service handler.
     */
    stdair::STDAIR_ServicePtr_T getSTDAIR_ServicePtr() const {
      return _stdairService;
    }
    
    /**
     * Get the STDAIR service handler.
     */
    stdair::STDAIR_Service& getSTDAIR_Service() const {
      assert (_stdairService != NULL);
      return *_stdairService;
    }

    /** Get the CRS code. */
    const CRSCode_T& getCRSCode () const {
      return _CRSCode;
    }

    /** Get a reference on the AIRINV Master service handler. */
    AIRINV::AIRINV_Master_Service& getAIRINV_Master_Service () const {
      assert (_airinvMasterService != NULL);
      return *_airinvMasterService;
    }

    /** Get a reference on the AIRSCHED service handler. */
    AIRSCHED::AIRSCHED_Service& getAIRSCHED_Service () const {
      assert (_airschedService != NULL);
      return *_airschedService;
    }

    /** Get a reference on the SIMFQT service handler. */
    SIMFQT::SIMFQT_Service& getSIMFQT_Service () const {
      assert (_simfqtService != NULL);
      return *_simfqtService;
    }
      
    // ///////////////// Setters ///////////////////
    /** Set the pointer on the STDAIR service handler. */
    void setSTDAIR_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr) {
      _stdairService = ioSTDAIR_ServicePtr;
    }
    
    /** Set the CRS code. */
    void setCRSCode (const CRSCode_T& iCRSCode) {
      _CRSCode = iCRSCode;
    }

    /** Set the pointer on the AIRINV Master service handler. */
    void setAIRINV_Master_Service (AIRINV_Master_ServicePtr_T ioAIRINV_Master_ServicePtr) {
      _airinvMasterService = ioAIRINV_Master_ServicePtr;
    }

    /** Set the pointer on the AIRSCHED service handler. */
    void setAIRSCHED_Service (AIRSCHED_ServicePtr_T ioAIRSCHED_ServicePtr) {
      _airschedService = ioAIRSCHED_ServicePtr;
    }

    /** Set the pointer on the SIMFQT service handler. */
    void setSIMFQT_Service (SIMFQT_ServicePtr_T ioSIMFQT_ServicePtr) {
      _simfqtService = ioSIMFQT_ServicePtr;
    }


  private:
    // //////////////////// Display Methods /////////////////////
    /** Display the short SIMCRS_ServiceContext content. */
    const std::string shortDisplay() const;
    
    /** Display the full SIMCRS_ServiceContext content. */
    const std::string display() const;

    
  private:
    // /////////////// Children ///////////////
    /** Standard Airline (StdAir) Service Handler. */
    stdair::STDAIR_ServicePtr_T _stdairService;

    /** Airline Schedule Service Handler. */
    AIRSCHED_ServicePtr_T _airschedService;

    /** Fare Quote Service Handler. */
    SIMFQT_ServicePtr_T _simfqtService;

    /** Airline Inventory Service Handler. */
    AIRINV_Master_ServicePtr_T _airinvMasterService;

    
  private:
    // //////////// Attributes //////////////////
    /** CRS code. */
    CRSCode_T _CRSCode;
  };
  
}
#endif // __SIMCRS_SVC_SIMCRSSERVICECONTEXT_HPP
