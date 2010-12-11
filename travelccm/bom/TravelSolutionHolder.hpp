#ifndef __TRAVELCCM_BOM_TRAVELSOLUTIONHOLDER_HPP
#define __TRAVELCCM_BOM_TRAVELSOLUTIONHOLDER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// TRAVELCCM 
#include <travelccm/bom/BomAbstract.hpp>
#include <travelccm/bom/TravelSolutionList.hpp>
#include <travelccm/bom/Restriction.hpp>
#include <travelccm/bom/TravelSolution.hpp>

namespace TRAVELCCM {

  /**  */
  class TravelSolutionHolder : public BomAbstract {
    friend class FacTravelSolutionHolder;

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

    /** Get the number of travel solutions in the list */
    int numberOfTravelSolutions();

    /** return true if the list is void */
    bool isVoid();
    
    /** Get a string describing the whole key (differentiating two objects
        at any level). */
    const std::string describeKey() const;

    /** Get a string describing the short key (differentiating two objects
        at the same level). */
    const std::string describeShortKey() const;

  private:
    /** the list of (N) travel solutions for a given request */
    TravelSolutionList_T _travelSolutionList;

    /** Iterator for the current restriction/class */
    TravelSolutionList_T::iterator _itCurrentTravelSolution;


  private:
    /** Constructors are private so as to force the usage of the Factory
        layer. */
    /** Default constructors. */
    TravelSolutionHolder ();
    TravelSolutionHolder (const TravelSolutionHolder&);

    /** Destructor. */
    virtual ~TravelSolutionHolder();

  public:
    const TravelSolution& getCurrentTravelSolution () const;

    void begin ();
    bool hasNotReachedEnd () const;
    void iterate ();
    void eraseCurrentTravelSolution ();
    void addTravelSolution (const TravelSolution&) ;
    void addTravelSolutionList (TravelSolutionList_T);

    /** given a list of travel solutions, the function modifies the current
        holder of travel solutions to keep only the ones that match the
        restriction, and updates the list of travel solutions put in argument
        so that it holds the travel solutions erased due to that restriction.
    */
    void restrictionMeetsTSList (const Restriction&, TravelSolutionList_T&,
                                 const Passenger&);

    /** return the cheapest travel solution of the holder. If there are more than
        one, one among the cheapest travel solution is returned randomly */
    const TravelSolution* getCheapestTravelSolution();
  };

}
#endif // __TRAVELCCM_BOM_TRAVELSOLUTIONHOLDER_HPP

