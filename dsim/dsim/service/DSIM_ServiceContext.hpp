#ifndef __DSIM_SVC_DSIMSERVICECONTEXT_HPP
#define __DSIM_SVC_DSIMSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// Dsim
#include <dsim/DSIM_Types.hpp>
#include <dsim/service/ServiceAbstract.hpp>

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

    
    // ///////// Setters //////////
    /** Set the simulator ID. */
    void setSimulatorID (const SimulatorID_T& iSimulatorID) {
      _simulatorID = iSimulatorID;
    }


    // ///////// Display Methods //////////
    /** Display the short DSIM_ServiceContext content. */
    const std::string shortDisplay() const;
    
    /** Display the full DSIM_ServiceContext content. */
    const std::string display() const;

    
  private:
    // /////// Construction / initialisation ////////
    /** Constructors. */
    DSIM_ServiceContext ();
    DSIM_ServiceContext (const SimulatorID_T&);
    DSIM_ServiceContext (const DSIM_ServiceContext&);

    /** Destructor. */
    ~DSIM_ServiceContext();

  private:
    // //////////// Attributes //////////////////
    /** Simulator ID. */
    SimulatorID_T _simulatorID;
  };

}
#endif // __DSIM_SVC_DSIMSERVICECONTEXT_HPP
