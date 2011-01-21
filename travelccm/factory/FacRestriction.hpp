#ifndef __TRAVELCCM_FAC_FACRESTRICTION_HPP
#define __TRAVELCCM_FAC_FACRESTRICTION_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// TRAVELCCM 
#include <travelccm/TRAVELCCM_Types.hpp>
#include <travelccm/factory/FacBomAbstract.hpp>
#include <travelccm/bom/RestrictionHolder.hpp>

namespace TRAVELCCM {
  // Forward declarations.
  class Restriction;
  
  /** Factory for Restriction objects. */
  class FacRestriction : public FacBomAbstract {
  public:
    /** Provide the unique instance.
        <br>The singleton is instantiated when first used
        @return FacRestriction& */
    static FacRestriction& instance();

    /** Destructor.
        <br>The Destruction put the _instance to NULL
        in order to be clean for the next FacRestriction::instance() */
    virtual ~FacRestriction();

    /** Create a new Restriction object.
        <br>This new object is added to the list of instantiated objects.
        @return Restriction& The newly created object. */
    Restriction& create ();
    Restriction& create (std::string);
    Restriction& create (std::string, std::string);
    Restriction& create (std::string, stdair::DateTime_T);

    /* add a restriction in the list of restrictionHolder */
    void  addRestriction (RestrictionHolder& ioRestrictionHolder,
                          Restriction& ioRestriction);

  protected:
    /** Default Constructor.
        <br>This constructor is protected in order to ensure the singleton
        pattern.*/
    FacRestriction () {}

  private:
    /** The unique instance.*/
    static FacRestriction* _instance;
  };

}
#endif // __TRAVELCCM_FAC_FACRESTRICTION_HPP

