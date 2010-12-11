#ifndef __AIRSCHED_FAC_FACAIRSCHEDSERVICECONTEXT_HPP
#define __AIRSCHED_FAC_FACAIRSCHEDSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// AIRSCHED
#include <airsched/AIRSCHED_Types.hpp>
#include <airsched/factory/FacServiceAbstract.hpp>

namespace AIRSCHED {

  /** Forward declarations. */
  class AIRSCHED_ServiceContext;

  /** Factory for Bucket. */
  class FacAIRSCHEDServiceContext : public FacServiceAbstract {
  public:

    /** Provide the unique instance.
        <br> The singleton is instantiated when first used
        @return FacServiceContext& */
    static FacAIRSCHEDServiceContext& instance();

    /** Destructor.
        <br> The Destruction put the _instance to NULL
        in order to be clean for the next
        FacAIRSCHEDServiceContext::instance(). */
    ~FacAIRSCHEDServiceContext();

    /** Create a new ServiceContext object.
        <br>This new object is added to the list of instantiated objects.
        @return ServiceContext& The newly created object. */
    AIRSCHED_ServiceContext& create ();

    
  protected:
    /** Default Constructor.
        <br>This constructor is protected in order to ensure the singleton
        pattern.*/
    FacAIRSCHEDServiceContext () {}

  private:
    /** The unique instance.*/
    static FacAIRSCHEDServiceContext* _instance;

  };
}
#endif // __AIRSCHED_FAC_FACAIRSCHEDSERVICECONTEXT_HPP
