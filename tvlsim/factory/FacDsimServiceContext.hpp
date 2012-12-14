#ifndef __TVLSIM_FAC_FACTVLSIMSERVICECONTEXT_HPP
#define __TVLSIM_FAC_FACTVLSIMSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/stdair_date_time_types.hpp>
#include <stdair/service/FacServiceAbstract.hpp>

namespace TVLSIM {

  /** Forward declarations. */
  class TVLSIM_ServiceContext;

  /** Factory for Bucket. */
  class FacDsimServiceContext : public stdair::FacServiceAbstract {
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

    /** Create a new TVLSIM_ServiceContext object.
        <br>This new object is added to the list of instantiated objects.
        @return TVLSIM_ServiceContext& The newly created object. */
    TVLSIM_ServiceContext& create ();

    
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
#endif // __TVLSIM_FAC_FACTVLSIMSERVICECONTEXT_HPP
