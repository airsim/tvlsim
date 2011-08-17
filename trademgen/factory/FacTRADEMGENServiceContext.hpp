#ifndef __TRADEMGEN_FAC_FACTRADEMGENSERVICECONTEXT_HPP
#define __TRADEMGEN_FAC_FACTRADEMGENSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_maths_types.hpp>
#include <stdair/service/FacServiceAbstract.hpp>
// TraDemGen
#include <trademgen/TRADEMGEN_Types.hpp>

namespace TRADEMGEN {

  /// Forward declarations
  class TRADEMGEN_ServiceContext;

  /**
   * @brief Factory for creating the TraDemGen service context instance.
   */
  class FacTRADEMGENServiceContext : public stdair::FacServiceAbstract {
  public:
    /**
     * Provide the unique instance.
     *
     * The singleton is instantiated when first used.
     *
     * @return FacTRADEMGENServiceContext&
     */
    static FacTRADEMGENServiceContext& instance();

    /**
     * Destructor.
     *
     * The Destruction put the _instance to NULL
     * in order to be clean for the next FacTRADEMGENServiceContext::instance().
     */
    ~FacTRADEMGENServiceContext();

    /**
     * Create a new TRADEMGEN_ServiceContext object.
     *
     * This new object is added to the list of instantiated objects.
     *
     * @param const stdair::RandomSeed_T& Seed for the random generation.
     * @return TRADEMGEN_ServiceContext& The newly created object.
     */
    TRADEMGEN_ServiceContext& create (const stdair::RandomSeed_T&);

    
  protected:
    /**
     * Default Constructor.
     *
     * This constructor is protected in order to ensure the singleton pattern.
     */
    FacTRADEMGENServiceContext () {}

  private:
    /**
     * The unique instance.
     */
    static FacTRADEMGENServiceContext* _instance;
  };

}
#endif // __TRADEMGEN_FAC_FACTRADEMGENSERVICECONTEXT_HPP
