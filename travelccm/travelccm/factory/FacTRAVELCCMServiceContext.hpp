#ifndef __TRAVELCCM_FAC_FACTRAVELCCMSERVICECONTEXT_HPP
#define __TRAVELCCM_FAC_FACTRAVELCCMSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// TRAVELCCM
#include <travelccm/TRAVELCCM_Types.hpp>
#include <travelccm/factory/FacServiceAbstract.hpp>

namespace TRAVELCCM {

  /** Forward declarations. */
  class TRAVELCCM_ServiceContext;
  struct TravelCCMType;

  /** Factory for Bucket. */
  class FacTRAVELCCMServiceContext : public FacServiceAbstract {
  public:

    /** Provide the unique instance.
        <br> The singleton is instantiated when first used
        @return FacServiceContext& */
    static FacTRAVELCCMServiceContext& instance();

    /** Destructor.
        <br> The Destruction put the _instance to NULL
        in order to be clean for the next FacServiceContext::instance() */
    virtual ~FacTRAVELCCMServiceContext();

    /** Create a new ServiceContext object.
        <br>This new object is added to the list of instantiated objects.
        @return ServiceContext& The newly created object. */
    TRAVELCCM_ServiceContext& create (const TravelCCMType&);

    
  protected:
    /** Default Constructor.
        <br>This constructor is protected in order to ensure the singleton
        pattern.*/
    FacTRAVELCCMServiceContext () {}

  private:
    /** The unique instance.*/
    static FacTRAVELCCMServiceContext* _instance;

  };
}
#endif // __TRAVELCCM_FAC_FACTRAVELCCMSERVICECONTEXT_HPP
