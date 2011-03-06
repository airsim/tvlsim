#ifndef __TRADEMGEN_FAC_FACTRADEMGENSERVICECONTEXT_HPP
#define __TRADEMGEN_FAC_FACTRADEMGENSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/service/FacServiceAbstract.hpp>
// TraDemGen
#include <trademgen/TRADEMGEN_Types.hpp>

namespace TRADEMGEN {

  /** Forward declarations. */
  class TRADEMGEN_ServiceContext;

  /** Factory for Bucket. */
  class FacTRADEMGENServiceContext : public stdair::FacServiceAbstract {
  public:

    /** Provide the unique instance.
        <br> The singleton is instantiated when first used
        @return FacTRADEMGENServiceContext& */
    static FacTRADEMGENServiceContext& instance();

    /** Destructor.
        <br> The Destruction put the _instance to NULL
        in order to be clean for the next
        FacTRADEMGENServiceContext::instance(). */
    ~FacTRADEMGENServiceContext();

    /** Create a new TRADEMGEN_ServiceContext object.
        <br>This new object is added to the list of instantiated objects.
        @return TRADEMGEN_ServiceContext& The newly created object. */
    TRADEMGEN_ServiceContext& create ();

    
  protected:
    /** Default Constructor.
        <br>This constructor is protected in order to ensure the singleton
        pattern.*/
    FacTRADEMGENServiceContext () {}

  private:
    /** The unique instance.*/
    static FacTRADEMGENServiceContext* _instance;
  };

}
#endif // __TRADEMGEN_FAC_FACTRADEMGENSERVICECONTEXT_HPP
