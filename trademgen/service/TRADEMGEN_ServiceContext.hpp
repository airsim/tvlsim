#ifndef __TRADEMGEN_SVC_TRADEMGENSERVICECONTEXT_HPP
#define __TRADEMGEN_SVC_TRADEMGENSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// Boost
#include <boost/shared_ptr.hpp>
// Boost Random
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_service_types.hpp>
#include <stdair/bom/BookingRequestTypes.hpp>
// Trademgen
#include <trademgen/TRADEMGEN_Types.hpp>
#include <trademgen/basic/DemandCharacteristicTypes.hpp>
#include <trademgen/service/ServiceAbstract.hpp>

// Forward declarations
namespace stdair {
  struct DemandCharacteristics;
  struct DemandDistribution;
}
  
namespace TRADEMGEN {

  /** Class holding the context of the Trademgen services. */
  class TRADEMGEN_ServiceContext : public ServiceAbstract {
    /** The TRADEMGEN_Service class should be the sole class to get access to
        ServiceContext content: general users do not want to bother
        with a context interface. */
    friend class TRADEMGEN_Service;
    friend class FacTRADEMGENServiceContext;
    
  private:
    // /////// Construction / initialisation ////////
    /** Constructors. */
    TRADEMGEN_ServiceContext ();
    TRADEMGEN_ServiceContext (const std::string&);
    TRADEMGEN_ServiceContext (const TRADEMGEN_ServiceContext&);

    /** Destructor. */
    ~TRADEMGEN_ServiceContext();
      
  private:
    // ///////// Getters //////////
    /** Get the pointer on the STDAIR service handler. */
    stdair::STDAIR_ServicePtr_T getSTDAIR_ServicePtr () const {
      return _stdairService;
    }

    /** Get the shared uniform generator. */
    stdair::UniformGenerator_T& getUniformGenerator () {
      return _uniformGenerator;
    }

    /** Get the default POS distribution. */
    const POSProbabilityMass_T& getPOSProbabilityMass () const {
      return _posProbabilityMass;
    }

  private:
    // ///////// Setters //////////
    /** Set the pointer on the STDAIR service handler. */
    void setSTDAIR_Service (stdair::STDAIR_ServicePtr_T ioSTDAIR_ServicePtr) {
      _stdairService = ioSTDAIR_ServicePtr;
    }

  private:
    // ///////// Display Methods //////////
    /** Display the short TRADEMGEN_ServiceContext content. */
    const std::string shortDisplay() const;
    
    /** Display the full TRADEMGEN_ServiceContext content. */
    const std::string display() const;

    
  private:
    // ///////////// Children ////////////
    /** Standard Airline (StdAir) Service Handler. */
    stdair::STDAIR_ServicePtr_T _stdairService;

  private:
    // ////////////// Attributes ////////////////
    /** Shared generator for seeds and some other values. */
    stdair::RandomSeed_T _seed;
    stdair::BaseGenerator_T _generator;
    stdair::UniformGenerator_T _uniformGenerator;
    /** POS probability mass, used when the POS is 'RoW'. */
    const POSProbabilityMass_T _posProbabilityMass;
  };

}
#endif // __TRADEMGEN_SVC_TRADEMGENSERVICECONTEXT_HPP
