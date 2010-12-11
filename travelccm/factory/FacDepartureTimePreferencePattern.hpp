#ifndef __TRAVELCCM_FAC_FACDEPARTURETIMEPREFERENCEPATTERN_HPP
#define __TRAVELCCM_FAC_FACDEPARTURETIMEPREFERENCEPATTERN_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// TravelCCM
#include <travelccm/TRAVELCCM_Types.hpp>
#include <travelccm/factory/FacBomAbstract.hpp>

namespace TRAVELCCM {

  // Forward declarations.
  class DepartureTimePreferencePattern;
  
  /** Factory for DepartureTimePreferencePattern objects. */
  class FacDepartureTimePreferencePattern : public FacBomAbstract {
  public:
    /** Provide the unique instance.
        <br>The singleton is instantiated when first used
        @return FacDepartureTimePreferencePattern& */
    static FacDepartureTimePreferencePattern& instance();

    /** Destructor.
        <br>The Destruction put the _instance to NULL
        in order to be clean for the next FacDepartureTimePreferencePattern::instance() */
    virtual ~FacDepartureTimePreferencePattern();

    /** Create a new DepartureTimePreferencePattern object.
        <br>This new object is added to the list of instantiated objects.
        @return DepartureTimePreferencePattern& The newly created object. */
    DepartureTimePreferencePattern& create (const PatternId_T&);

  protected:
    /** Default Constructor.
        <br>This constructor is protected in order to ensure the singleton
        pattern.*/
    FacDepartureTimePreferencePattern () {}

  private:
    /** The unique instance.*/
    static FacDepartureTimePreferencePattern* _instance;
  };

}
#endif // __TRAVELCCM_FAC_FACDEPARTURETIMEPREFERENCEPATTERN_HPP
