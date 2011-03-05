#ifndef __AIRINV_FAC_FACAIRINVMASTERSERVICECONTEXT_HPP
#define __AIRINV_FAC_FACAIRINVMASTERSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/service/FacServiceAbstract.hpp>

namespace AIRINV {

  /// Forward declarations
  class AIRINV_Master_ServiceContext;

  /**
   * @brief Factory for Bucket.
   */
  class FacAirinvMasterServiceContext : public stdair::FacServiceAbstract {
  public:

    /** Provide the unique instance.
        <br> The singleton is instantiated when first used
        @return FacAirinvMasterServiceContext& */
    static FacAirinvMasterServiceContext& instance();

    /** Destructor.
        <br> The Destruction put the _instance to NULL
        in order to be clean for the next
        FacAirinvMasterServiceContext::instance() */
    ~FacAirinvMasterServiceContext();

    /** Create a new AIRINV_Master_ServiceContext object.
        <br>This new object is added to the list of instantiated objects.
        @return AIRINV_Master_ServiceContext& The newly created object. */
    AIRINV_Master_ServiceContext& create();

    
  protected:
    /** Default Constructor.
        <br>This constructor is protected in order to ensure the singleton
        pattern.*/
    FacAirinvMasterServiceContext() {}

  private:
    /** The unique instance.*/
    static FacAirinvMasterServiceContext* _instance;
  };

}
#endif // __AIRINV_FAC_FACAIRINVMASTERSERVICECONTEXT_HPP
