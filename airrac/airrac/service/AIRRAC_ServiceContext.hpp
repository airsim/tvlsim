#ifndef __AIRRAC_SVC_AIRRACSERVICECONTEXT_HPP
#define __AIRRAC_SVC_AIRRACSERVICECONTEXT_HPP

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
#include <stdair/bom/YieldStore.hpp>
// Airrac
#include <airrac/AIRRAC_Types.hpp>
#include <airrac/service/ServiceAbstract.hpp>

namespace AIRRAC {

  /** Class holding the context of the Airrac services. */
  class AIRRAC_ServiceContext : public ServiceAbstract {
    /** The AIRRAC_Service class should be the sole class to get access to
        ServiceContext content: general users do not want to bother
        with a context interface. */
    friend class AIRRAC_Service;
    friend class FacAirracServiceContext;

  private:
    /// //////////////// Constructors and destructors /////////////
    /** Main constructor. */
    AIRRAC_ServiceContext (const stdair::AirlineCode_T&, stdair::YieldStore&);
    /** Default constructor. (not to be used). */
    AIRRAC_ServiceContext ();
    /** Default copy constructor (not to be used). */
    AIRRAC_ServiceContext (const AIRRAC_ServiceContext&);

    /** Destructor. */
    ~AIRRAC_ServiceContext();


  private:
    // ///////////////// Getters ///////////////////
    /** Get the pointer on the STDAIR service handler. */
    stdair::STDAIR_ServicePtr_T getSTDAIR_ServicePtr () const {
      return _stdairService;
    }
    
    /** Get the airline code. */
    const stdair::AirlineCode_T& getAirlineCode () const {
      return _airlineCode;
    }

    /** Get the airline yield store. */
    stdair::YieldStore& getYieldStore () {
      return _yieldStore;
    }

    
    // ///////////////// Setters ///////////////////
    /** Set the pointer on the STDAIR service handler. */
    void setSTDAIR_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr) {
      _stdairService = ioSTDAIR_ServicePtr;
    }
    
    /** Set the airline code. */
    void setAirlineCode (const stdair::AirlineCode_T& iAirlineCode) {
      _airlineCode = iAirlineCode;
    }


  private:
    // //////////////////// Display Methods /////////////////////
    /** Display the short AIRRAC_ServiceContext content. */
    const std::string shortDisplay() const;
    
    /** Display the full AIRRAC_ServiceContext content. */
    const std::string display() const;
    
    
  private:
    // /////////////// Children ///////////////
    /** Standard Airline (StdAir) Service Handler. */
    stdair::STDAIR_ServicePtr_T _stdairService;

    
  private:
    // //////////// Attributes //////////////////
    /** Airline code. */
    stdair::AirlineCode_T _airlineCode;

    /** Airline yield store object. */
    stdair::YieldStore& _yieldStore;
  };

}
#endif // __AIRRAC_SVC_AIRRACSERVICECONTEXT_HPP
