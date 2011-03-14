#ifndef __TRAVELCCM_SVC_TRAVELCCM_SERVICE_CONTEXT_HPP
#define __TRAVELCCM_SVC_TRAVELCCM_SERVICE_CONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// Boost
#include <boost/shared_ptr.hpp>
// StdAir
#include <stdair/stdair_service_types.hpp>
#include <stdair/service/ServiceAbstract.hpp>
// TravelCCM
#include <travelccm/TRAVELCCM_Types.hpp>

namespace TRAVELCCM {

  /**
   * @brief Inner class holding the context for the TravelCCM Service
   * object.
   */
  class TRAVELCCM_ServiceContext : public stdair::ServiceAbstract {
    /**
     * The TRAVELCCM_Service class should be the sole class to get
     * access to ServiceContext content: general users do not want to
     * bother with a context interface.
     */
    friend class TRAVELCCM_Service;
    friend class FacTRAVELCCMServiceContext;    
    
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

    /**
     * State whether or not RMOL owns the STDAIR service resources.
     */
    const bool getOwnStdairServiceFlag() const {
      return _ownStdairService;
    }


    // ///////////////// Setters ///////////////////
    /**
     * Set the pointer on the STDAIR service handler.
     */
    void setSTDAIR_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr,
                            const bool iOwnStdairService) {
      _stdairService = ioSTDAIR_ServicePtr;
      _ownStdairService = iOwnStdairService;
    }
    

  private:
    // //////////////////// Display Methods /////////////////////
    /**
     * Display the short AIRINV_ServiceContext content.
     */
    const std::string shortDisplay() const;
    
    /**
     * Display the full AIRINV_ServiceContext content.
     */
    const std::string display() const;
    
    /**
     * Display of the structure.
     */
    const std::string describe() const;

    
  private:
    /// //////////////// Constructors and destructors /////////////
    /**
     * Main constructor.
     */
    TRAVELCCM_ServiceContext();
    /**
     * Copy constructor (not to be used).
     */
    TRAVELCCM_ServiceContext (const TRAVELCCM_ServiceContext&);

    /**
     * Destructor.
     */
    ~TRAVELCCM_ServiceContext();

    /**
     * Clear the context (cabin capacity, bucket holder).
     */
    void reset();


  private:
    // /////////////// Children ///////////////
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
#endif // __TRAVELCCM_SVC_TRAVELCCM_SERVICE_CONTEXT_HPP
