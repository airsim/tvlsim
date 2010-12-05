#ifndef __AVLCAL_SVC_AVLCALSERVICECONTEXT_HPP
#define __AVLCAL_SVC_AVLCALSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// Avlcal
#include <avlcal/AVLCAL_Types.hpp>
#include <avlcal/service/ServiceAbstract.hpp>

namespace AVLCAL {

  /** Class holding the context of the Avlcal services. */
  class AVLCAL_ServiceContext : public ServiceAbstract {
    friend class FacAvlcalServiceContext;
  public:
    // ///////// Getters //////////
    /** Get the airline code. */
    const AirlineCode_T& getAirlineCode () const {
      return _airlineCode;
    }

    
    // ///////// Setters //////////
    /** Set the airline code. */
    void setAirlineCode (const AirlineCode_T& iAirlineCode) {
      _airlineCode = iAirlineCode;
    }


    // ///////// Display Methods //////////
    /** Display the short AVLCAL_ServiceContext content. */
    const std::string shortDisplay() const;
    
    /** Display the full AVLCAL_ServiceContext content. */
    const std::string display() const;

    
  private:
    // /////// Construction / initialisation ////////
    /** Constructors. */
    AVLCAL_ServiceContext ();
    AVLCAL_ServiceContext (const AirlineCode_T& iAirlineCode);
    AVLCAL_ServiceContext (const AVLCAL_ServiceContext&);

    /** Destructor. */
    ~AVLCAL_ServiceContext();

  private:
    // //////////// Attributes //////////////////
    /** Airline code. */
    AirlineCode_T _airlineCode;
  };

}
#endif // __AVLCAL_SVC_AVLCALSERVICECONTEXT_HPP
