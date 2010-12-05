#ifndef __SIMCRS_FAC_FACSIMCRSSERVICECONTEXT_HPP
#define __SIMCRS_FAC_FACSIMCRSSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// Simcrs
#include <simcrs/factory/FacServiceAbstract.hpp>

namespace SIMCRS {

  /** Forward declarations. */
  class SIMCRS_ServiceContext;

  /** Factory for Bucket. */
  class FacSimcrsServiceContext : public FacServiceAbstract {
  public:

    /** Provide the unique instance.
        <br> The singleton is instantiated when first used
        @return FacSimcrsServiceContext& */
    static FacSimcrsServiceContext& instance();

    /** Destructor.
        <br> The Destruction put the _instance to NULL
        in order to be clean for the next
        FacSimcrsServiceContext::instance() */
    ~FacSimcrsServiceContext();

    /** Create a new SIMCRS_ServiceContext object.
        <br>This new object is added to the list of instantiated objects.
        @return SIMCRS_ServiceContext& The newly created object. */
    SIMCRS_ServiceContext& create (const std::string& iTravelDatabaseName);

    
  protected:
    /** Default Constructor.
        <br>This constructor is protected in order to ensure the singleton
        pattern.*/
    FacSimcrsServiceContext () {}

  private:
    /** The unique instance.*/
    static FacSimcrsServiceContext* _instance;
  };

}
#endif // __SIMCRS_FAC_FACSIMCRSSERVICECONTEXT_HPP
