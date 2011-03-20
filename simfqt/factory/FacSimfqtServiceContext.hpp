#ifndef __SIMFQT_FAC_FACSIMFQTSERVICECONTEXT_HPP
#define __SIMFQT_FAC_FACSIMFQTSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// StdAir
#include <stdair/stdair_basic_types.hpp>
#include <stdair/service/FacServiceAbstract.hpp>

namespace SIMFQT {

  /// Forward declarations
  class SIMFQT_ServiceContext;


  /**
   * @brief Factory for the service context.
   */
  class FacSimfqtServiceContext : public stdair::FacServiceAbstract {
  public:

    /**
     * Provide the unique instance.
     *
     * The singleton is instantiated when first used.
     * @return FacServiceContext&
     */
    static FacSimfqtServiceContext& instance();

    /**
     * Destructor.
     *
     * The Destruction put the _instance to NULL in order to be clean
     * for the next FacSimfqtServiceContext::instance().
     */
    ~FacSimfqtServiceContext();

    /**
     * Create a new ServiceContext object.
     *
     * This new object is added to the list of instantiated objects.
     *
     * @return ServiceContext& The newly created object.
     */
    SIMFQT_ServiceContext& create();

    
  protected:
    /**
     * Default Constructor.
     *
     * This constructor is protected in order to ensure the singleton pattern.
     */
    FacSimfqtServiceContext() {}


  private:
    /**
     * The unique instance.
     */
    static FacSimfqtServiceContext* _instance;
  };

}
#endif // __SIMFQT_FAC_FACSIMFQTSERVICECONTEXT_HPP
