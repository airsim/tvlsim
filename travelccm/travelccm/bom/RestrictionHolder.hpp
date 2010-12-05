#ifndef __TRAVELCCM_BOM_RESTRICTIONHOLDER_HPP
#define __TRAVELCCM_BOM_RESTRICTIONHOLDER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// TRAVELCCM 
#include <travelccm/bom/BomAbstract.hpp>
#include <travelccm/bom/RestrictionList.hpp>

namespace TRAVELCCM {

  // Forward declarations
  class Restriction;

  /** Respresents a holder of restriction, using a list for the moment
      This holder's aim is to represent, for a given passenger, the ordered
      list of restrictions that will determine his choice among different fares.
   */
  class RestrictionHolder : public BomAbstract {
    friend class FacRestrictionHolder;
  public:
    
    // /////////// Display support methods /////////
    /** Dump a Business Object into an output stream.
        @param ostream& the output stream. */
    void toStream (std::ostream& ioOut) const;

    /** Read a Business Object from an input stream.
        @param istream& the input stream. */
    void fromStream (std::istream& ioIn);

   /** Get the serialised version of the Business Object. */
    std::string toString() const;
    
    /** Get a string describing the whole key (differentiating two objects
        at any level). */
    const std::string describeKey() const;

    /** Get a string describing the short key (differentiating two objects
        at the same level). */
    const std::string describeShortKey() const;

    //////////////////// Setters ///////////////////
    /** Add a restriction to the restriction holder. */
    //void addRestriction (Restriction&); 

  private:
    /** the list of (N) restrictions for a given passenger */
    RestrictionList_T _restrictionList;

    /** Iterator for the current restriction/class */
    RestrictionList_T::iterator _itCurrentRestriction;
    
  private:
    /** Constructors are private so as to force the usage of the Factory
        layer. */
    /** Default constructors. */
    RestrictionHolder ();
    RestrictionHolder (const RestrictionHolder&);

    /** Destructor. */
    virtual ~RestrictionHolder();

  public:
    /** Get the current restriction. */
    const Restriction& getCurrentRestriction () const;

    /** */
    void begin ();
    /** */
    bool hasNotReachedEnd () const;
    /** */
    void iterate ();
    /** */
    void addRestriction (const Restriction&);
    /** */
    void eraseCurrentRestriction ();

    /** given a list of restriction, the function returns if the
        travel solution in argument meets the different restrictions
    */
    // Obsolete (deprecated) method
    // bool travelSolutionMeetRestrictionList (TravelSolution& TS);

  };

}
#endif // __TRAVELCCM_BOM_RESTRICTIONHOLDER_HPP

