#ifndef __TRADEMGEN_SVC_TRADEMGENSERVICECONTEXT_HPP
#define __TRADEMGEN_SVC_TRADEMGENSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/stdair_service_types.hpp>
#include <stdair/basic/RandomGeneration.hpp>
#include <stdair/bom/BookingRequestTypes.hpp>
#include <stdair/service/ServiceAbstract.hpp>
// TraDemGen
#include <trademgen/TRADEMGEN_Types.hpp>
#include <trademgen/basic/DemandCharacteristicsTypes.hpp>

// Forward declarations
namespace stdair {
  struct DemandCharacteristics;
  struct DemandDistribution;
}
  
namespace TRADEMGEN {

  /**
   * @brief Class holding the context of the Trademgen services.
   */
  class TRADEMGEN_ServiceContext : public stdair::ServiceAbstract {
    /**
     * The TRADEMGEN_Service class should be the sole class to get
     * access to ServiceContext content: general users do not want to
     * bother with a context interface.
     */
    friend class TRADEMGEN_Service;
    friend class FacTRADEMGENServiceContext;
    
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

    /**
     * Get the shared uniform generator.
     */
    stdair::RandomGeneration& getUniformGenerator() {
      return _uniformGenerator;
    }

    /**
     * Get the default POS distribution.
     */
    const POSProbabilityMass_T& getPOSProbabilityMass() const {
      return _posProbabilityMass;
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

    
  private:
    // ///////// Display Methods //////////
    /**
     * Display the short TRADEMGEN_ServiceContext content.
     */
    const std::string shortDisplay() const;
    
    /**
     * Display the full TRADEMGEN_ServiceContext content.
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
    TRADEMGEN_ServiceContext();
    TRADEMGEN_ServiceContext (const std::string& iServiceName);
    /**
     * Copy constructor (not to be used).
     */
    TRADEMGEN_ServiceContext (const TRADEMGEN_ServiceContext&);

    /**
     * Destructor.
     */
    ~TRADEMGEN_ServiceContext();

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
    // ////////////// Attributes ////////////////
    /**
     * Shared generator for seeds and some other values.
     *
     * The state of the random generator, given as parameter, evolves
     * each time a demand request is generated.
     */
    stdair::RandomGeneration _uniformGenerator;

    /**
     * POS probability mass, used when the POS is 'RoW'.
     */
    const POSProbabilityMass_T _posProbabilityMass;
  };

}
#endif // __TRADEMGEN_SVC_TRADEMGENSERVICECONTEXT_HPP
