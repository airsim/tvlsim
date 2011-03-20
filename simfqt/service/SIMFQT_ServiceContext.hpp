#ifndef __SIMFQT_SVC_SIMFQTSERVICECONTEXT_HPP
#define __SIMFQT_SVC_SIMFQTSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/stdair_service_types.hpp>
#include <stdair/service/ServiceAbstract.hpp>
// SimFQT
#include <simfqt/SIMFQT_Types.hpp>

/// Forward declarations
namespace stdair {
  class STDAIR_Service;
}

namespace SIMFQT {

  /**
   * @brief Class holding the context of the SimFQT services.
   */
  class SIMFQT_ServiceContext : public stdair::ServiceAbstract {
    /**
     * The SIMFQT_Service class should be the sole class to get access to
     * ServiceContext content: general users do not want to bother
     * with a context interface.
     */
    friend class SIMFQT_Service;
    friend class FacSimfqtServiceContext;

  private:
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
     * State whether or not RMOL owns the STDAIR service resources.
     */
    const bool getOwnStdairServiceFlag() const {
      return _ownStdairService;
    }


  private:    
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
     * Clear the context (cabin capacity, bucket holder).
     */
    void reset();


  private:
    // ///////// Display Methods //////////
    /**
     * Display the short STDAIR_ServiceContext content.
     */
    const std::string shortDisplay() const;
    
    /**
     * Display the full STDAIR_ServiceContext content.
     */
    const std::string display() const;

    /**
     * Display of the structure.
     */
    const std::string describe() const;


  private:
    // /////// Construction / initialisation ////////
    /**
     * Main constructor.
     */
    SIMFQT_ServiceContext (const FareQuoteID_T&);

    /**
     * Default constructor.
     */
    SIMFQT_ServiceContext();

    /**
     * Copy constructor.
     */
    SIMFQT_ServiceContext (const SIMFQT_ServiceContext&);
        
    /**
     * Destructor.
     */
    ~SIMFQT_ServiceContext();


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
  };

}
#endif // __SIMFQT_SVC_SIMFQTSERVICECONTEXT_HPP
