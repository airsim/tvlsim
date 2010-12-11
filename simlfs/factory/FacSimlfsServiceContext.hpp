#ifndef __SIMLFS_FAC_FACSIMLFSSERVICECONTEXT_HPP
#define __SIMLFS_FAC_FACSIMLFSSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// Simlfs
#include <simlfs/factory/FacServiceAbstract.hpp>

namespace SIMLFS {

  /** Forward declarations. */
  class SIMLFS_ServiceContext;

  /** Factory for Bucket. */
  class FacSimlfsServiceContext : public FacServiceAbstract {
  public:

    /** Provide the unique instance.
        <br> The singleton is instantiated when first used
        @return FacSimlfsServiceContext& */
    static FacSimlfsServiceContext& instance();

    /** Destructor.
        <br> The Destruction put the _instance to NULL
        in order to be clean for the next
        FacSimlfsServiceContext::instance() */
    ~FacSimlfsServiceContext();

    /** Create a new SIMLFS_ServiceContext object.
        <br>This new object is added to the list of instantiated objects.
        @return SIMLFS_ServiceContext& The newly created object. */
    SIMLFS_ServiceContext& create ();

    
  protected:
    /** Default Constructor.
        <br>This constructor is protected in order to ensure the singleton
        pattern.*/
    FacSimlfsServiceContext () {}

  private:
    /** The unique instance.*/
    static FacSimlfsServiceContext* _instance;
  };

}
#endif // __SIMLFS_FAC_FACSIMLFSSERVICECONTEXT_HPP
