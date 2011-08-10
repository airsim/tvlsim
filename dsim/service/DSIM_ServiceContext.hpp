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
// SimCRS
#include <simcrs/SIMCRS_Types.hpp>
// TraDemGen
#include <trademgen/TRADEMGEN_Types.hpp>
// TravelCCM
#include <travelccm/TRAVELCCM_Types.hpp>
// DSim
#include <dsim/DSIM_Types.hpp>
#include <dsim/bom/ConfigurationParameters.hpp>
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
     * Get the configuration parameters.
     */
    const ConfigurationParameters& getConfigurationParameters() const {
      return _configurationParameters;
    }
    
    /**
     * Get the start date of the simulation (delegated to the
     * ConfigurationParameters object).
     */
    const stdair::Date_T& getStartDate() const {
      return _configurationParameters.getStartDate();
    }
      
    /**
     * Get the end date of the simulation (delegated to the
     * ConfigurationParameters object).
     */
    const stdair::Date_T& getEndDate() const {
      return _configurationParameters.getEndDate();
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

    /**
     * Set the start date of the simulation (delegated to the
     * ConfigurationParameters object).
     */
    void setStartDate (const stdair::Date_T& iStartDate) {
      _configurationParameters.setEndDate (iStartDate);
    }
      
    /**
     * Set the end date of the simulation (delegated to the
     * ConfigurationParameters object).
     */
    void setEndDate (const stdair::Date_T& iEndDate) {
      _configurationParameters.setEndDate (iEndDate);
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
     * Main constructors.
     */
    DSIM_ServiceContext (const stdair::Date_T& iStartDate,
                         const stdair::Date_T& iEndDate);
    DSIM_ServiceContext (const SimulatorID_T&, const stdair::Date_T& iStartDate,
                         const stdair::Date_T& iEndDate);

    /**
     * Default constructor (not to be used).
     */
    DSIM_ServiceContext();
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
     * Configuration parameters.
     */
    ConfigurationParameters _configurationParameters;
    
    /**
     * Reference Data Set parameters.
     */
    RDSParameters _rdsParameters;
  };

}
#endif // __DSIM_SVC_DSIMSERVICECONTEXT_HPP
