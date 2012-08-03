#ifndef __DSIM_SVC_DSIMSERVICECONTEXT_HPP
#define __DSIM_SVC_DSIMSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/stdair_service_types.hpp>
#include <stdair/service/ServiceAbstract.hpp>
// SEvMgr
#include <sevmgr/SEVMGR_Types.hpp>
// SimCRS
#include <simcrs/SIMCRS_Types.hpp>
// TraDemGen
#include <trademgen/TRADEMGEN_Types.hpp>
// TravelCCM
#include <travelccm/TRAVELCCM_Types.hpp>
// DSim
#include <dsim/DSIM_Types.hpp>
#include <dsim/bom/SimulationStatus.hpp>
#include <dsim/bom/RDSParameters.hpp>

namespace DSIM {
    
  /**
   * @brief Class holding the context of the Dsim services.
   */
  class DSIM_ServiceContext : public stdair::ServiceAbstract {
    /**
     * The SIMCRS_Service class should be the sole class to get access
     * to ServiceContext content: general users do not want to bother
     * with a context interface.
     */
    friend class DSIM_Service;
    friend class FacDsimServiceContext;
    
  private:
    // ///////////// Getters on attributes //////////////
    /**
     * Get the simulator ID.
     */
    const SimulatorID_T& getSimulatorID() const {
      return _simulatorID;
    }

    /**
     * Get the pointer on the simulation status instance.
     */
    SimulationStatus& getSimulationStatus() const {
      assert (_simulationStatus != NULL);
      return *_simulationStatus;
    }
    
    // /////////////// Getters on children //////////////
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
     * Get the pointer on the SEVMGR service handler.
     */
    SEVMGR::SEVMGR_ServicePtr_T getSEVMGR_ServicePtr() const {
      return _sevmgrService;
    }

    /**
     * Get the SEVMGR service handler.
     */
    SEVMGR::SEVMGR_Service& getSEVMGR_Service() const {
      assert (_sevmgrService != NULL);
      return *_sevmgrService;
    }
    /**
     * State whether or not SIMCRS_Service owns the STDAIR service resources.
     */
    const bool getOwnStdairServiceFlag() const {
      return _ownStdairService;
    }

    /**
     * Get the RDS parameters.
     */
    const RDSParameters& getRDSParameters() const {
      return _rdsParameters;
    }

    /**
     * Get a reference on the SIMCRS service handler.
     */
    SIMCRS::SIMCRS_Service& getSIMCRS_Service() const {
      return *_simcrsService.get();
    }
    
    /**
     * Get a reference on the TRADEMGEN service handler.
     */
    TRADEMGEN::TRADEMGEN_Service& getTRADEMGEN_Service() const {
      return *_trademgenService.get();
    }
    
    /**
     * Get a reference on the TRAVELCCM service handler.
     */
    TRAVELCCM::TRAVELCCM_Service& getTRAVELCCM_Service() const {
      return *_travelccmService.get();
    }
    

  private:
    // //////// Setters on attributes /////////     
    /** Set the simulator ID. */
    void setSimulatorID (const SimulatorID_T& iSimulatorID) {
      _simulatorID = iSimulatorID;
    }
    
    // ///////// Setters on children //////////
    /**
     * Set the pointer on the STDAIR service handler.
     */
    void setSTDAIR_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr,
                            const bool iOwnStdairService) {
      _stdairService = ioSTDAIR_ServicePtr;
      _ownStdairService = iOwnStdairService;
    }  

    /**
     * Set the pointer on the SEVMGR service handler.
     */
    void setSEVMGR_Service (SEVMGR::SEVMGR_ServicePtr_T ioSEVMGR_ServicePtr) {
      _sevmgrService = ioSEVMGR_ServicePtr;
    }
    
    /**
     * Set the RDS parameters.
     */
    void setRDSParameters (const RDSParameters& iRDSParameters) {
      _rdsParameters = iRDSParameters;
    }
    
    /**
     * Set the pointer on the SIMCRS service handler.
     */
    void setSIMCRS_Service (SIMCRS::SIMCRS_ServicePtr_T ioServicePtr) {
      _simcrsService = ioServicePtr;
    }

    /**
     * Set the pointer on the TRADEMGEN service handler.
     */
    void setTRADEMGEN_Service (TRADEMGEN::TRADEMGEN_ServicePtr_T ioServicePtr) {
      _trademgenService = ioServicePtr;
    }

    /**
     * Set the pointer on the TRAVELCCM service handler.
     */
    void setTRAVELCCM_Service (TRAVELCCM::TRAVELCCM_ServicePtr_T ioServicePtr) {
      _travelccmService = ioServicePtr;
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
     * Default constructor.
     */
    DSIM_ServiceContext ();

    /**
     * Main constructor.
     */
    DSIM_ServiceContext (const SimulatorID_T&);

    /**
     * Copy constructor (not to be used).
     */
    DSIM_ServiceContext (const DSIM_ServiceContext&);

    /**
     * Destructor.
     */
    ~DSIM_ServiceContext();

    /**
     * Clear the context.
     */
    void reset();

    /**
     * Initialisation.
     *
     * The Simulation Status object is created by that method, and then
     * stored within the service context.
     */
    void initSimulationStatus (const SimulatorID_T&);


  private:
    // //////////////////// Children ///////////////////////
    /**
     * Standard Airline (StdAir) service handler.
     */
    stdair::STDAIR_ServicePtr_T _stdairService;

    /**
     * State whether or not AIRINV owns the STDAIR service resources.
     */
    bool _ownStdairService;

    /**
     * SEvMgr service handler.
     */
    SEVMGR::SEVMGR_ServicePtr_T _sevmgrService;

    /**
     * CRS service handler.
     */
    SIMCRS::SIMCRS_ServicePtr_T _simcrsService;

    /**
     * TraDemGen service handler.
     */
    TRADEMGEN::TRADEMGEN_ServicePtr_T _trademgenService;

    /**
     * TravelCCM service handler.
     */
    TRAVELCCM::TRAVELCCM_ServicePtr_T _travelccmService;


  private:
    // /////////////////// Attributes ////////////////////////
    /**
     * Simulator ID.
     *
     * \note: it is currently not used.
     */
    SimulatorID_T _simulatorID;

    /**
     * Pointer on the Simulation Status.
     */
    SimulationStatus* _simulationStatus;
    
    /**
     * Reference Data Set parameters.
     */
    RDSParameters _rdsParameters;
  };

}
#endif // __DSIM_SVC_DSIMSERVICECONTEXT_HPP
