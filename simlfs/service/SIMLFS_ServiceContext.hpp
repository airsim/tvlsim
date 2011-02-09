#ifndef __SIMLFS_SVC_SIMLFSSERVICECONTEXT_HPP
#define __SIMLFS_SVC_SIMLFSSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// Boost
#include <boost/shared_ptr.hpp>
// StdAir
#include <stdair/stdair_service_types.hpp>
// SimLFS
#include <simlfs/SIMLFS_Types.hpp>
#include <simlfs/service/ServiceAbstract.hpp>

namespace AIRINV {
  class AIRINV_Master_Service;
}

namespace SIMFQT {
  class SIMFQT_Service;
}

namespace SIMLFS {

  /** Pointer on the AIRINV Master Service handler. */
  typedef boost::shared_ptr<AIRINV::AIRINV_Master_Service> AIRINV_Master_ServicePtr_T;

  /** Pointer on the SIMFQT Service handler. */
  typedef boost::shared_ptr<SIMFQT::SIMFQT_Service> SIMFQT_ServicePtr_T;


  /** Class holding the context of the SimLFS services. */
  class SIMLFS_ServiceContext : public ServiceAbstract {
    friend class SIMLFS_Service;
    friend class FacSimlfsServiceContext;
  public:
    // ///////// Getters //////////
    /** Get the pointer on the STDAIR service handler. */
    stdair::STDAIR_ServicePtr_T getSTDAIR_Service () const {
      return _stdairService;
    }

    /** Get a reference on the AIRINV Master service handler. */
    AIRINV_Master_ServicePtr_T getAIRINV_Master_Service () const {
      return _airinvMasterService;
    }

    /** Get a reference on the SIMFQT service handler. */
    SIMFQT_ServicePtr_T getSIMFQT_Service () const {
      return _simfqtService;
    }

    
    // ///////////////// Setters ///////////////////
    /** Set the pointer on the STDAIR service handler. */
    void setSTDAIR_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr) {
      _stdairService = ioSTDAIR_ServicePtr;
    } 

    /** Set the pointer on the AIRINV Master service handler. */
    void setAIRINV_Master_Service (AIRINV_Master_ServicePtr_T ioAIRINV_Master_ServicePtr) {
      _airinvMasterService = ioAIRINV_Master_ServicePtr;
    }

    /** Set the pointer on the SIMFQT service handler. */
    void setSIMFQT_Service (SIMFQT_ServicePtr_T ioSIMFQT_ServicePtr) {
      _simfqtService = ioSIMFQT_ServicePtr;
    }
    
  private:    
    // ///////// Display Methods //////////
    /** Display the short SIMLFS_ServiceContext content. */
    const std::string shortDisplay() const;
    
    /** Display the full SIMLFS_ServiceContext content. */
    const std::string display() const;

    
  private:
    // /////// Construction / initialisation ////////
    /** Constructors. */
    SIMLFS_ServiceContext ();
    SIMLFS_ServiceContext (const SIMLFS_ServiceContext&);

    /** Destructor. */
    ~SIMLFS_ServiceContext();

    
  private:
    // /////////////// Children ///////////////
    /** Standard Airline (StdAir) Service Handler. */
    stdair::STDAIR_ServicePtr_T _stdairService;

    /** Fare Quote Service Handler. */
    SIMFQT_ServicePtr_T _simfqtService;

    /** Airline Inventory Service Handler. */
    AIRINV_Master_ServicePtr_T _airinvMasterService;
  };

}
#endif // __SIMLFS_SVC_SIMLFSSERVICECONTEXT_HPP
