#ifndef __AVLCAL_FAC_FACAVLCALSERVICECONTEXT_HPP
#define __AVLCAL_FAC_FACAVLCALSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/service/FacServiceAbstract.hpp>

namespace AVLCAL {

  /** Forward declarations. */
  class AVLCAL_ServiceContext;

  /** Factory for Bucket. */
  class FacAvlcalServiceContext : public stdair::FacServiceAbstract {
  public:

    /** Provide the unique instance.
        <br> The singleton is instantiated when first used
        @return FacAvlcalServiceContext& */
    static FacAvlcalServiceContext& instance();

    /** Destructor.
        <br> The Destruction put the _instance to NULL
        in order to be clean for the next
        FacAvlcalServiceContext::instance() */
    ~FacAvlcalServiceContext();

    /** Create a new AVLCAL_ServiceContext object.
        <br>This new object is added to the list of instantiated objects.
        @return AVLCAL_ServiceContext& The newly created object. */
    AVLCAL_ServiceContext& create (const std::string& iTravelDatabaseName);

    
  protected:
    /** Default Constructor.
        <br>This constructor is protected in order to ensure the singleton
        pattern.*/
    FacAvlcalServiceContext () {}

  private:
    /** The unique instance.*/
    static FacAvlcalServiceContext* _instance;
  };

}
#endif // __AVLCAL_FAC_FACAVLCALSERVICECONTEXT_HPP
