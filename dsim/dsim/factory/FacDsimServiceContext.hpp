#ifndef __DSIM_FAC_FACDSIMSERVICECONTEXT_HPP
#define __DSIM_FAC_FACDSIMSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// Dsim
#include <dsim/factory/FacServiceAbstract.hpp>

namespace DSIM {

  /** Forward declarations. */
  class DSIM_ServiceContext;

  /** Factory for Bucket. */
  class FacDsimServiceContext : public FacServiceAbstract {
  public:

    /** Provide the unique instance.
        <br> The singleton is instantiated when first used
        @return FacDsimServiceContext& */
    static FacDsimServiceContext& instance();

    /** Destructor.
        <br> The Destruction put the _instance to NULL
        in order to be clean for the next
        FacDsimServiceContext::instance() */
    ~FacDsimServiceContext();

    /** Create a new DSIM_ServiceContext object.
        <br>This new object is added to the list of instantiated objects.
        @return DSIM_ServiceContext& The newly created object. */
    DSIM_ServiceContext& create();

    
  protected:
    /** Default Constructor.
        <br>This constructor is protected in order to ensure the singleton
        pattern.*/
    FacDsimServiceContext () {}

  private:
    /** The unique instance.*/
    static FacDsimServiceContext* _instance;
  };

}
#endif // __DSIM_FAC_FACDSIMSERVICECONTEXT_HPP
