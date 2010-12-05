#ifndef __SIMFQT_FAC_FACSIMFQTSERVICECONTEXT_HPP
#define __SIMFQT_FAC_FACSIMFQTSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// Simfqt
#include <simfqt/factory/FacServiceAbstract.hpp>

namespace SIMFQT {

  /** Forward declarations. */
  class SIMFQT_ServiceContext;

  /** Factory for Bucket. */
  class FacSimfqtServiceContext : public FacServiceAbstract {
  public:

    /** Provide the unique instance.
        <br> The singleton is instantiated when first used
        @return FacSimfqtServiceContext& */
    static FacSimfqtServiceContext& instance();

    /** Destructor.
        <br> The Destruction put the _instance to NULL
        in order to be clean for the next
        FacSimfqtServiceContext::instance() */
    ~FacSimfqtServiceContext();

    /** Create a new SIMFQT_ServiceContext object.
        <br>This new object is added to the list of instantiated objects.
        @return SIMFQT_ServiceContext& The newly created object. */
    SIMFQT_ServiceContext& create ();

    
  protected:
    /** Default Constructor.
        <br>This constructor is protected in order to ensure the singleton
        pattern.*/
    FacSimfqtServiceContext () {}

  private:
    /** The unique instance.*/
    static FacSimfqtServiceContext* _instance;
  };

}
#endif // __SIMFQT_FAC_FACSIMFQTSERVICECONTEXT_HPP
