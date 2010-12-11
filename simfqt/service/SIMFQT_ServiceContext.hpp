#ifndef __SIMFQT_SVC_SIMFQTSERVICECONTEXT_HPP
#define __SIMFQT_SVC_SIMFQTSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/stdair_service_types.hpp>
// SimFQT
#include <simfqt/SIMFQT_Types.hpp>
#include <simfqt/service/ServiceAbstract.hpp>

namespace SIMFQT {

  /** Class holding the context of the Simfqt services. */
  class SIMFQT_ServiceContext : public ServiceAbstract {
    friend class FacSimfqtServiceContext;
  public:
    // ///////// Getters //////////
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
    /** Display the short SIMFQT_ServiceContext content. */
    const std::string shortDisplay() const;
    
    /** Display the full SIMFQT_ServiceContext content. */
    const std::string display() const;
    
  private:
    // /////// Construction / initialisation ////////
    /** Constructors. */
    SIMFQT_ServiceContext ();
    SIMFQT_ServiceContext (const FareQuoteID_T&);
    SIMFQT_ServiceContext (const SIMFQT_ServiceContext&);

    /** Destructor. */
    ~SIMFQT_ServiceContext();

  private:
    // //////////// Attributes //////////////////
    /** Standard Airline (StdAir) Service Handler. */
    stdair::STDAIR_ServicePtr_T _stdairService;
  };

}
#endif // __SIMFQT_SVC_SIMFQTSERVICECONTEXT_HPP
