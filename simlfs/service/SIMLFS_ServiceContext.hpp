#ifndef __SIMLFS_SVC_SIMLFSSERVICECONTEXT_HPP
#define __SIMLFS_SVC_SIMLFSSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/stdair_service_types.hpp>
// Simlfs
#include <simlfs/SIMLFS_Types.hpp>
#include <simlfs/service/ServiceAbstract.hpp>

namespace SIMLFS {

  /** Class holding the context of the Simlfs services. */
  class SIMLFS_ServiceContext : public ServiceAbstract {
    friend class FacSimlfsServiceContext;
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
    /** Display the short SIMLFS_ServiceContext content. */
    const std::string shortDisplay() const;
    
    /** Display the full SIMLFS_ServiceContext content. */
    const std::string display() const;
    
  private:
    // /////// Construction / initialisation ////////
    /** Constructors. */
    SIMLFS_ServiceContext ();
    SIMLFS_ServiceContext (const FareQuoteID_T&);
    SIMLFS_ServiceContext (const SIMLFS_ServiceContext&);

    /** Destructor. */
    ~SIMLFS_ServiceContext();

  private:
    // //////////// Attributes //////////////////
    /** Standard Airline (StdAir) Service Handler. */
    stdair::STDAIR_ServicePtr_T _stdairService;
  };

}
#endif // __SIMLFS_SVC_SIMLFSSERVICECONTEXT_HPP
