#ifndef __AIRINV_SVC_AIRINVMASTERSERVICECONTEXT_HPP
#define __AIRINV_SVC_AIRINVMASTERSERVICECONTEXT_HPP

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
#include <stdair/service/ServiceAbstract.hpp>
// AirInv
#include <airinv/AIRINV_Types.hpp>

namespace AIRINV {

  /// Forward declarations
  class AIRINV_Service;

  /**
   * Class holding the context of the Airinv services.
   */
  class AIRINV_Master_ServiceContext : public stdair::ServiceAbstract {
    /**
     * The AIRINV_Master_Service class should be the sole class to get
     * access to ServiceContext content: general users do not want to
     * bother with a context interface.
     */
    friend class AIRINV_Master_Service;
    friend class FacAirinvMasterServiceContext;

  private:
    // ///////////////// Getters ///////////////////
    /**
     * Get the pointer on the STDAIR service handler.
     */
    stdair::STDAIR_ServicePtr_T getSTDAIR_ServicePtr () const {
      return _stdairService;
    }

    /**
     * Get the STDAIR service handler.
     */
    stdair::STDAIR_Service& getSTDAIR_Service () const {
      assert (_stdairService != NULL);
      return *_stdairService;
    }

    /**
     * State whether or not RMOL owns the STDAIR service resources.
     */
    const bool getOwnStdairServiceFlag() const {
      return _ownStdairService;
    }

    /**
     * Get a reference on the (slave) AIRINV service handler which
     * corresponds to the given airline code.
     */
    AIRINV_ServicePtr_T getAIRINV_Service() const {
      return _airinvService;
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

    /**
     * Set the pointer on the AIRINV service handler.
     */
    void setAIRINV_Service (AIRINV_ServicePtr_T ioAIRINV_ServicePtr) {
      _airinvService = ioAIRINV_ServicePtr;
    }


  private:
    // //////////////////// Display Methods /////////////////////
    /**
     * Display the short AIRINV_Master_ServiceContext content.
     */
    const std::string shortDisplay() const;

    /**
     * Display the full AIRINV_Master_ServiceContext content.
     */
    const std::string display() const;

    /**
     * Display of the structure.
     */
    const std::string describe() const;


  private:
    /// //////////////// Constructors and destructors /////////////
    /**
     * Default constructor.
     */
    AIRINV_Master_ServiceContext();
    /**
     * Default copy constructor (not to be used).
     */
    AIRINV_Master_ServiceContext (const AIRINV_Master_ServiceContext&);

    /**
     * Destructor.
     */
    ~AIRINV_Master_ServiceContext();

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


  private:
    // //////////// Attributes //////////////////
    /**
     * (Slave) Airline Inventory Service Handler map.
     */
    AIRINV_ServicePtr_T _airinvService;
  };

}
#endif // __AIRINV_SVC_AIRINVMASTERSERVICECONTEXT_HPP
