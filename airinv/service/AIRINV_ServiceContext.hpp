#ifndef __AIRINV_SVC_AIRINVSERVICECONTEXT_HPP
#define __AIRINV_SVC_AIRINVSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// Boost
#include <boost/shared_ptr.hpp>
// StdAir
#include <stdair/stdair_service_types.hpp>
#include <stdair/bom/Inventory.hpp>
// RMOL
#include <rmol/RMOL_Types.hpp>
// AirInv
#include <airinv/AIRINV_Types.hpp>
#include <airinv/service/ServiceAbstract.hpp>

namespace AIRINV {

  /** Class holding the context of the Airinv services. */
  class AIRINV_ServiceContext : public ServiceAbstract {
    /** The AIRINV_Service class should be the sole class to get access to
        ServiceContext content: general users do not want to bother
        with a context interface. */
    friend class AIRINV_Service;
    friend class FacAirinvServiceContext;

  private:
    /// //////////////// Constructors and destructors /////////////
    /** Main constructor. */
    AIRINV_ServiceContext ();
    /** Default copy constructor (not to be used). */
    AIRINV_ServiceContext (const AIRINV_ServiceContext&);
    /** Destructor. */
    ~AIRINV_ServiceContext();


  private:
    // ///////////////// Getters ///////////////////
    /** Get the pointer on the STDAIR service handler. */
    stdair::STDAIR_ServicePtr_T getSTDAIR_ServicePtr () const {
      return _stdairService;
    }

    /** Get the STDAIR service handler. */
    stdair::STDAIR_Service& getSTDAIR_Service () const {
      assert (_stdairService != NULL);
      return *_stdairService;
    }

    /** Get the RMOL service handler. */
    RMOL::RMOL_Service& getRMOL_Service () const {
      assert (_rmolService != NULL);
      return *_rmolService;
    }
    
    // ///////////////// Setters ///////////////////
    /** Set the pointer on the STDAIR service handler. */
    void setSTDAIR_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr) {
      _stdairService = ioSTDAIR_ServicePtr;
    }
    
    /** Set the pointer on the RMOL service handler. */
    void setRMOL_Service (RMOL::RMOL_ServicePtr_T ioRMOL_ServicePtr) {
      _rmolService = ioRMOL_ServicePtr;
    }

  private:
    // //////////////////// Display Methods /////////////////////
    /** Display the short AIRINV_ServiceContext content. */
    const std::string shortDisplay() const;
    
    /** Display the full AIRINV_ServiceContext content. */
    const std::string display() const;
    
    
  private:
    // /////////////// Children ///////////////
    /** Standard Airline (StdAir) Service Handler. */
    stdair::STDAIR_ServicePtr_T _stdairService;
    
    /** Standard Airline (RMOL) Service Handler. */
    RMOL::RMOL_ServicePtr_T _rmolService;
  };

}
#endif // __AIRINV_SVC_AIRINVSERVICECONTEXT_HPP
