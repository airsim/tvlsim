#ifndef __AIRINV_FAC_FACAIRINVSERVICECONTEXT_HPP
#define __AIRINV_FAC_FACAIRINVSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/service/FacServiceAbstract.hpp>

namespace AIRINV {

  /** Forward declarations. */
  class AIRINV_ServiceContext;

  /** Factory for Bucket. */
  class FacAirinvServiceContext : public stdair::FacServiceAbstract {
  public:

    /** Provide the unique instance.
        <br> The singleton is instantiated when first used
        @return FacAirinvServiceContext& */
    static FacAirinvServiceContext& instance();

    /** Destructor.
        <br> The Destruction put the _instance to NULL
        in order to be clean for the next
        FacAirinvServiceContext::instance() */
    ~FacAirinvServiceContext();

    /** Create a new AIRINV_ServiceContext object.
        <br>This new object is added to the list of instantiated objects.
        @return AIRINV_ServiceContext& The newly created object. */
    AIRINV_ServiceContext& create();

    
  protected:
    /** Default Constructor.
        <br>This constructor is protected in order to ensure the singleton
        pattern.*/
    FacAirinvServiceContext() {}

  private:
    /** The unique instance.*/
    static FacAirinvServiceContext* _instance;
  };

}
#endif // __AIRINV_FAC_FACAIRINVSERVICECONTEXT_HPP
