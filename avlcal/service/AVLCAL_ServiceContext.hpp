#ifndef __AVLCAL_SVC_AVLCALSERVICECONTEXT_HPP
#define __AVLCAL_SVC_AVLCALSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_service_types.hpp>
#include <stdair/service/ServiceAbstract.hpp>
// Avlcal
#include <avlcal/AVLCAL_Types.hpp>

namespace AVLCAL {

  /**
   * @brief Class holding the context of the Avlcal services.
   */
  class AVLCAL_ServiceContext : public stdair::ServiceAbstract {
    /**
     * The AVLCAL_Service class should be the sole class to get access to
     * ServiceContext content: general users do not want to bother
     * with a context interface.
     */
    friend class AVLCAL_Service;
    friend class FacAvlcalServiceContext;

  public:
    // ///////// Getters //////////
    /**
     * Get the pointer on the STDAIR service handler.
     */
    stdair::STDAIR_ServicePtr_T getSTDAIR_ServicePtr() const {
      return _stdairService;
    }

    /**
     * Get the STDAIR service handler.
     */
    stdair::STDAIR_Service& getSTDAIR_Service() const;

    /**
     * Get the airline code.
     */
    const stdair::AirlineCode_T& getAirlineCode () const {
      return _airlineCode;
    }

    /**
     * State whether or not RMOL owns the STDAIR service resources.
     */
    const bool getOwnStdairServiceFlag() const {
      return _ownStdairService;
    }

    
    // ///////// Setters //////////
    /**
     * Set the pointer on the STDAIR service handler.
     */
    void setSTDAIR_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr,
                            const bool iOwnStdairService) {
      _stdairService = ioSTDAIR_ServicePtr;
      _ownStdairService = iOwnStdairService;
    }

    /**
     * Set the airline code.
     */
    void setAirlineCode (const stdair::AirlineCode_T& iAirlineCode) {
      _airlineCode = iAirlineCode;
    }

    /**
     * Clear the context.
     */
    void reset();


  private:
    // ///////// Display Methods //////////
    /**
     * Display the short AVLCAL_ServiceContext content.
     */
    const std::string shortDisplay() const;
    
    /**
     * Display the full AVLCAL_ServiceContext content.
     */
    const std::string display() const;

    /**
     * Display of the structure.
     */
    const std::string describe() const;

    
  private:
    // /////// Construction / initialisation ////////
    /** Constructors. */
    AVLCAL_ServiceContext ();
    AVLCAL_ServiceContext (const stdair::AirlineCode_T& iAirlineCode);
    AVLCAL_ServiceContext (const AVLCAL_ServiceContext&);

    /** Destructor. */
    ~AVLCAL_ServiceContext();


  private:
    // ///////////// Children ////////////
    /**
     * Standard Airline (StdAir) Service Handler.
     */
    stdair::STDAIR_ServicePtr_T _stdairService;

    /**
     * State whether or not RMOL owns the STDAIR service resources.
     */
    bool _ownStdairService;

  private:
    // //////////// Attributes //////////////////
    /** Airline code. */
    stdair::AirlineCode_T _airlineCode;
  };

}
#endif // __AVLCAL_SVC_AVLCALSERVICECONTEXT_HPP
