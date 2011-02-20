// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <vector>

namespace airsched {
  
  /** Place. */
  struct Place_T {
    // Attributes
    std::string _name;
    std::string _code;
    /** Constructor. */
    Place_T () : _name (""), _code ("") {}
    /* Display. */  
    void display() const {
      std::cout << "Place: " << _name << " (" << _code << ")" << std::endl;
    }
  };

  /** List of Place strucutres. */
  typedef std::vector<Place_T> PlaceList_T;

  /** Date. */
  struct Date_T {
    // Attributes
    boost::gregorian::date _date;
    unsigned int _reldays;
    unsigned int _day;
    unsigned int _month;
    unsigned int _year;
    /** Constructor. */
    Date_T () : _reldays (14), _day(1), _month(1), _year(1970) {}
    /* Display. */  
    void display() const {
      std::cout << "Date: " << _date << " (" << _day << "/" << _month
                << "/" << _year << "), i.e. in " << _reldays << " days"
                << std::endl;
    }
    /** Set the date from the staging details. */
    boost::gregorian::date getDate() const {
      return boost::gregorian::date (_year, _month, _day);
    }
  };

  /** List of Date strucutres. */
  typedef std::vector<Date_T> DateList_T;

  /** Airline. */
  struct Airline_T {
    // Attributes
    bool _isPreferred;
    std::string _name;
    std::string _code;
    /** Constructor. */
    Airline_T () : _isPreferred (true), _name(""), _code("") {}
    /* Display. */  
    void display() const {
      const std::string isPreferredStr = (_isPreferred)?"+":"-";
      std::cout << "Airline: " << isPreferredStr << _name << " (" << _code << ")"
                << std::endl;
    }
  };

  /** List of Airline strucutres. */
  typedef std::vector<Airline_T> AirlineList_T;

  /** Passenger. */
  struct Passenger_T {
    // Attributes
    typedef enum { ADULT = 0, CHILD, PET, LAST_VALUE } PassengerType_T;
    static const std::string _labels[LAST_VALUE];
    PassengerType_T _type;
    unsigned short _number;
    /** Constructor. */
    Passenger_T () : _type(ADULT), _number(1) {}
    /* Display. */  
    void display() const {
      std::cout << "Passenger: " << _number << " (" << _labels[_type] << ")"
                << std::endl;
    }
  };

  /** Passenger type labels. */
  const std::string Passenger_T::_labels[Passenger_T::LAST_VALUE] =
    { "Adult", "Child", "Pet" };

  /** List of Passenger strucutres. */
  typedef std::vector<Passenger_T> PassengerList_T;

  /** Search string. */
  struct SearchString_T {
    // Attributes
    PlaceList_T _placeList;
    DateList_T _dateList;
    AirlineList_T _airlineList;
    PassengerList_T _passengerList;
  
    /** Constructor. */
    SearchString_T () {}

    /* Display. */  
    void display() const {
      std::cout << std::endl;
    
      for (PlaceList_T::const_iterator itPlace = _placeList.begin();
           itPlace != _placeList.end(); ++itPlace) {
        const Place_T& lPlace = *itPlace;
        lPlace.display();
      }

      for (DateList_T::const_iterator itDate = _dateList.begin();
           itDate != _dateList.end(); ++itDate) {
        const Date_T& lDate = *itDate;
        lDate.display();
      }

      for (AirlineList_T::const_iterator itAirline = _airlineList.begin();
           itAirline != _airlineList.end(); ++itAirline) {
        const Airline_T& lAirline = *itAirline;
        lAirline.display();
      }

      for (PassengerList_T::const_iterator itPassenger = _passengerList.begin();
           itPassenger != _passengerList.end(); ++itPassenger) {
        const Passenger_T& lPassenger = *itPassenger;
        lPassenger.display();
      }

      std::cout << "-- Staging --" << std::endl;
      _tmpPlace.display();
    }

    // //// Staging ////
    Place_T _tmpPlace;
    Date_T _tmpDate;
    Airline_T _tmpAirline;
    Passenger_T _tmpPassenger;
  };

  //////////////////////////////////////////////////////////////////////////////
  //
  //  The booking request grammar (using subrules)
  //
  //////////////////////////////////////////////////////////////////////////////
  /** Parse the booking request.
    
      Sample
      guadeloupe rio de janeiro 07/22/2009 +aa -ua 2 adults 1 dog

      Grammar:
      search_string      ::= places
      [dates]
      *(preferred_airlines)
      *(passengers)
      dates              ::= board_date [off_date]
      places             ::= [board_place] off_place
      board_place        ::= place_elements
      off_place          ::= place_elements
      place_elements     ::= country | city | airport
      country            ::= country_code | country_name
      city               ::= city_code | city_name
      airport            ::= airport_code | airport_name
      preferred_airlines ::= [+|-] airline_code | airline_name
      passengers         ::= adult_number adult_description
      [child_number child_description]
      [pet_number pet_description]
      adult_description  ::= 'adult' | 'adults' | 'pax' | 'passengers'
      child_description  ::= 'child' | 'children' | 'kid' | 'kids'
      pet_description    ::= 'dog' | 'dogs' | 'cat' | 'cats'
  */
  SearchString_T parseBookingRequest (const std::string& iSearchString);

}
