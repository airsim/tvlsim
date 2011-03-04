#ifndef __TRAVELCCM_SVC_TRAVELCCM_SERVICE_CONTEXT_HPP
#define __TRAVELCCM_SVC_TRAVELCCM_SERVICE_CONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/stdair_service_types.hpp>
#include <stdair/service/ServiceAbstract.hpp>
// TravelCCM
#include <travelccm/TRAVELCCM_Types.hpp>

namespace TRAVELCCM {

  /** Inner class holding the context for the TRAVELCCM Service object. */
  class TRAVELCCM_ServiceContext : public stdair::ServiceAbstract {
    /** The TRAVELCCM_Service class should be the sole class to get access to
        ServiceContext content: general users do not want to bother
        with a context interface. */
    friend class TRAVELCCM_Service;
    friend class FacTRAVELCCMServiceContext;    
    
  private:
    // /////// Construction / initialisation ////////
    /** Constructors. */
    TRAVELCCM_ServiceContext ();
    TRAVELCCM_ServiceContext (const TRAVELCCM_ServiceContext&);

    /** Destructor. */
    ~TRAVELCCM_ServiceContext();
    
  private:
    // ///////// Getters //////////
    /** Get the pointer on the STDAIR service handler. */
    stdair::STDAIR_ServicePtr_T getSTDAIR_ServicePtr () const {
      return _stdairService;
    }

    /** Get the reference on the STDAIR service handler. */
    stdair::STDAIR_Service& getSTDAIR_Service () const {
      assert (_stdairService != NULL);
      return *_stdairService;
    }

  private:
    // ///////// Setters //////////
    /** Set the pointer on the STDAIR service handler. */
    void setSTDAIR_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr) {
      _stdairService = ioSTDAIR_ServicePtr;
    }

  private:
    // ///////// Display Methods //////////
    /** Display the short TRAVELCCM_ServiceContext content. */
    const std::string shortDisplay() const;
    
    /** Display the full TRAVELCCM_ServiceContext content. */
    const std::string display() const;

    
  private:
    // //////////// Attributes //////////////////
    /** Standard Airline (StdAir) Service Handler. */
    stdair::STDAIR_ServicePtr_T _stdairService;
  };

}
#endif // __TRAVELCCM_SVC_TRAVELCCM_SERVICE_CONTEXT_HPP
