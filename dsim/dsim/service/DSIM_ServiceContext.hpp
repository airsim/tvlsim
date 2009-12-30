#ifndef __DSIM_SVC_DSIMSERVICECONTEXT_HPP
#define __DSIM_SVC_DSIMSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// Boost
#include <boost/shared_ptr.hpp>
// Dsim
#include <dsim/DSIM_Types.hpp>
#include <dsim/bom/ConfigurationParameters.hpp>
#include <dsim/bom/RDSParameters.hpp>
#include <dsim/service/ServiceAbstract.hpp>

// Forward declarations
namespace SIMCRS {
  class SIMCRS_Service;
}

/** Pointer on the SIMCRS Service handler. */
typedef boost::shared_ptr<SIMCRS::SIMCRS_Service> SIMCRS_ServicePtr_T;


namespace DSIM {
    
  /** Class holding the context of the Dsim services. */
  class DSIM_ServiceContext : public ServiceAbstract {
    friend class FacDsimServiceContext;
  public:
    // ///////// Getters //////////
    /** Get the simulator ID. */
    const SimulatorID_T& getSimulatorID () const {
      return _simulatorID;
    }

    /** Get the configuration parameters. */
    const ConfigurationParameters& getConfigurationParameters () const {
      return _configurationParameters;
    }
    
    /** Get the RDS parameters. */
    const RDSParameters& getRDSParameters () const {
      return _rdsParameters;
    }

    /** Get a reference on the SIMCRS service handler. */
    SIMCRS::SIMCRS_Service& getSIMCRS_Service () const {
      return *_simcrsService.get();
    }
    
    // ///////// Setters //////////
    /** Set the simulator ID. */
    void setSimulatorID (const SimulatorID_T& iSimulatorID) {
      _simulatorID = iSimulatorID;
    }

    /** Set the configuration parameters. */
    void setConfigurationParameters (const ConfigurationParameters& iConfigurationParameters) {
      _configurationParameters = iConfigurationParameters;
    }
    
    /** Set the RDS parameters. */
    void setRDSParameters (const RDSParameters& iRDSParameters) {
      _rdsParameters = iRDSParameters;
    }
    
    /** Set the pointer on the SIMCRS service handler. */
    void setSIMCRS_Service (SIMCRS_ServicePtr_T ioSIMCRS_ServicePtr) {
      _simcrsService = ioSIMCRS_ServicePtr;
    }

    // ///////// Display Methods //////////
    /** Display the short DSIM_ServiceContext content. */
    const std::string shortDisplay() const;
    
    /** Display the full DSIM_ServiceContext content. */
    const std::string display() const;
  };

  private:
    // /////// Construction / initialisation ////////
    /** Constructors. */
    DSIM_ServiceContext ();
    DSIM_ServiceContext (const SimulatorID_T&);
    DSIM_ServiceContext (const DSIM_ServiceContext&);

    /** Destructor. */
    ~DSIM_ServiceContext();

  private:
    // ///////////// Children ////////////
    /** CRS Service Handler. */
    SIMCRS_ServicePtr_T _simcrsService;

  private:
    // //////////// Attributes //////////////////
    /** Simulator ID. */
    SimulatorID_T _simulatorID;

    /** Configuration parameters. */
    ConfigurationParameters _configurationParameters;
    
    /** Reference Data Set parameters. */
    RDSParameters _rdsParameters;

}
#endif // __DSIM_SVC_DSIMSERVICECONTEXT_HPP
