#ifndef __TRAVELCCM_SVC_TRAVELCCM_SERVICE_CONTEXT_HPP
#define __TRAVELCCM_SVC_TRAVELCCM_SERVICE_CONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// TravelCCM
#include <travelccm/TRAVELCCM_Types.hpp>
#include <travelccm/TravelCCMType.hpp>
#include <travelccm/bom/Request.hpp>
#include <travelccm/service/ServiceAbstract.hpp>

// Forward declarations
namespace stdair {
  struct PassengerType;
}

namespace TRAVELCCM {

  // Forward declarations.
  class Passenger;
  class TravelSolution;
  class RestrictionHolder;
  class TravelSolutionHolder;

  /** Inner class holding the context for the TRAVELCCM Service object. */
  class TRAVELCCM_ServiceContext : public ServiceAbstract {
    /** The TRAVELCCM_Service class should be the sole class to get access to
        ServiceContext content: general users do not want to bother
        with a context interface. */
    friend class TRAVELCCM_Service;
    friend class FacTRAVELCCMServiceContext;

  public:
    // ///////// Getters //////////
    /** Get the list of travel solutions. */
    TravelSolutionHolder& getTravelSolutionHolder() const;

    /** Get the passenger. */
    Passenger& getPassenger () const;
    
    /** Get the restriction holder in the passenger class. */
    RestrictionHolder& getRestrictionHolder() const;

    /** Get the customer-choice model type. */
    const TravelCCMType& getTravelCCMType () const {
      return _ccmType;
    }

    
    // ///////// Setters //////////
    /** Set the customer-choice model type. */
    void setTravelCCMType (const TravelCCMType& iCCMType) {
      _ccmType = iCCMType;
    }


    // ///////// Display Methods //////////
    /** Display the short AIRINV_ServiceContext content. */
    const std::string shortDisplay() const;
    
    /** Display the full AIRINV_ServiceContext content. */
    const std::string display() const;

    
  private:
    /** To create the passenger */
    void createPassenger (const stdair::PassengerType&);

    /** To intialise the different field of the passenger, that is the
        departure time preference pattern and the restriction holder.
        To be called after the creation of the passenger inevitably.  */
    void intializePassenger();

    /** Set the TravelSolutionHolder */
    void setTravelSolutionHolder (TravelSolutionHolder* ioTravelSolutionHolderPtr) {
      _travelSolutionHolder = ioTravelSolutionHolderPtr;
    }

    /** returns the cheapest travel solution which best match the passenger's
        request among the travel solution holder field of this class */
    const TravelSolution* getBestAndCheapestTravelSolutionByMatchingIndicator();

    /** Add a travel solution to the TravelSolutionHolder */
    void addTravelSolution (const std::string& iDepartureAirport,
                            const std::string& iArrivalAirport,
                            const stdair::Date_T& iDepartureDate,
                            const stdair::Duration_T& iDepartureTime,
                            const stdair::Duration_T& iArrivalTime,
                            const stdair::Duration_T& iDuration,
                            const bool iRefubdability,
                            const std::string& iAirlineCode,
                            const std::string& iCabinCode,
                            const int FlightNumber, double iFare,
                            int iStopsNumber,  bool iSNS, bool iChangeability,
                            const std::string& id);
    
    /** Add a restriction to the restriction holder in the passenger field */
    void addRestriction (const std::string& iRestrictionType);
    void addRestriction (const std::string& iRestrictionType,
                         const std::string& iNamePreference);
    void addRestriction (const std::string& iRestrictionType,
                         const stdair::DateTime_T iDepartureTime);

    /** Add a request and link it to the passenger. */
    void addAndLinkRequest (bool refundability, bool changeability,
                            bool saturdayNightStay,
                            std::string preferredAirline,
                            std::string preferredCabin,
                            stdair::DateTime_T departureTime);
    
    /** Read the input data from a file */
    void readFromInputFile (const std::string& iInputFileName);

    /** Add in the right order the restrictions regarding the characteristics
        of the fares desired by the passenger, found in the request field */
    void addAndOrderRestrictionsFromRequest();

    
  private:
    // /////// Construction / initialisation ////////
    /** Constructors. */
    TRAVELCCM_ServiceContext ();
    TRAVELCCM_ServiceContext (const TravelCCMType&);
    TRAVELCCM_ServiceContext (const TRAVELCCM_ServiceContext&);

    /** Destructor. */
    ~TRAVELCCM_ServiceContext();

    /** Initialise the StudyStatManager. */
    void initStudyStatManager ();
    
    
  private:
    // //////////// Attributes //////////////////
    /** Reference on the passenger. */
    Passenger* _passenger;
    
    /** Reference on the list of travel solutions. */
    TravelSolutionHolder* _travelSolutionHolder;

    /** Airline code. */
    TravelCCMType _ccmType;
  };

}
#endif // __TRAVELCCM_SVC_TRAVELCCM_SERVICE_CONTEXT_HPP
