#ifndef __AIRSCHED_FAC_FACAIRSCHEDSERVICECONTEXT_HPP
#define __AIRSCHED_FAC_FACAIRSCHEDSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/service/FacServiceAbstract.hpp>

namespace AIRSCHED {

  /// Forward declarations.
  class AIRSCHED_ServiceContext;

  /**
   * @brief Factory for the service context.
   */
  class FacAIRSCHEDServiceContext : public stdair::FacServiceAbstract {
  public:

    /**
     * Provide the unique instance.
     *
     * The singleton is instantiated when first used.
     * @return FacServiceContext&
     */
    static FacAIRSCHEDServiceContext& instance();

    /**
     * Destructor.
     *
     * The Destruction put the _instance to NULL in order to be clean
     * for the next FacSimfqtServiceContext::instance().
     */
    ~FacAIRSCHEDServiceContext();

    /**
     * Create a new ServiceContext object.
     *
     * This new object is added to the list of instantiated objects.
     *
     * @return ServiceContext& The newly created object.
     */
    AIRSCHED_ServiceContext& create();


  protected:
    /**
     * Default Constructor.
     *
     * This constructor is protected in order to ensure the singleton pattern.
     */
    FacAIRSCHEDServiceContext() {}

  private:
    /**
     * The unique instance.
     */
    static FacAIRSCHEDServiceContext* _instance;

  };
}
#endif // __AIRSCHED_FAC_FACAIRSCHEDSERVICECONTEXT_HPP
