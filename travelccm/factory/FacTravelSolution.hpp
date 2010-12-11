#ifndef __TRAVELCCM_FAC_FACTRAVELSOLUTION_HPP
#define __TRAVELCCM_FAC_FACTRAVELSOLUTION_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// Boost (Extended STL)
#include <boost/date_time/gregorian/gregorian.hpp>
// TRAVELCCM 
#include <travelccm/factory/FacBomAbstract.hpp>
#include <travelccm/TRAVELCCM_Types.hpp>
#include <travelccm/bom/TravelSolutionHolder.hpp>

namespace TRAVELCCM {
  // Forward declarations.
  class TravelSolution;
  
  /** Factory for TravelSolution objects. */
  class FacTravelSolution : public FacBomAbstract {
  public:
    /** Provide the unique instance.
        <br>The singleton is instantiated when first used
        @return FacTravelSolution& */
    static FacTravelSolution& instance();

    /** Destructor.
        <br>The Destruction put the _instance to NULL
        in order to be clean for the next FacTravelSolution::instance() */
    virtual ~FacTravelSolution();

    /** Create a new TravelSolution object.
        <br>This new object is added to the list of instantiated objects.
        @return TravelSolution& The newly created object. */
    TravelSolution& create ();
    TravelSolution& create (std::string dAirport, std::string aAirport,
                            Date_T aDepDate,
                            Duration_T depTime, Duration_T arTime,
                            Duration_T dur, bool Ref, std::string airline,
                            std::string cabin, int flightNum, double fare,
                            int lagsNum, bool SNS, bool change, std::string id);

    /** Add a travel solution in the list of TravelSolutionHolder */
    void addTravelSolution(TravelSolutionHolder& ioTravelSolutionHolder,
                           TravelSolution& ioTravelSolution);

  protected:
    /** Default Constructor.
        <br>This constructor is protected in order to ensure the singleton
        pattern.*/
    FacTravelSolution () {}

  private:
    /** The unique instance.*/
    static FacTravelSolution* _instance;
  };

}
#endif // __TRAVELCCM_FAC_FACTRAVELSOLUTION_HPP

