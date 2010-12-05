// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
#include <fstream>
// Boost (Extended STL)
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
// Boost Spirit (Parsing)
#define BOOST_SPIRIT_DEBUG
#include <boost/spirit/home/classic/core.hpp>
#include <boost/spirit/home/classic/attribute.hpp>
#include <boost/spirit/home/classic/utility/functor_parser.hpp>
#include <boost/spirit/home/classic/utility/loops.hpp>
#include <boost/spirit/home/classic/utility/chset.hpp>
#include <boost/spirit/home/classic/utility/confix.hpp>
#include <boost/spirit/home/classic/iterator/file_iterator.hpp>
#include <boost/spirit/home/classic/actor/push_back_actor.hpp>
#include <boost/spirit/home/classic/actor/assign_actor.hpp>
// StdAir
#include <stdair/service/Logger.hpp>
// AirSched
#include <airsched/batches/BookingRequestParser.hpp>

// Type definitions
typedef char char_t;
typedef char const* iterator_t;
//typedef boost::spirit::classic::file_iterator<char_t> iterator_t;
typedef boost::spirit::classic::scanner<iterator_t> scanner_t;
typedef boost::spirit::classic::rule<scanner_t> rule_t;

namespace airsched {
  
  /** Store the parsed place element. */
  struct store_place_element {
    /** Constructor. */
    store_place_element (SearchString_T& ioSearchString)
      : _searchString (ioSearchString) {}

    /** Parse the place. */
    void operator() (iterator_t iStr, iterator_t iStrEnd) const {
      std::string lPlace (iStr, iStrEnd);
      // std::cout << "Place: " << lPlace << std::endl;
      
      // Set the place
      _searchString._tmpPlace._name += " " + lPlace;
      
      // Add the parsed place to the list
      // _searchString._placeList.push_back (_searchString._tmpPlace);
    }
    
    SearchString_T& _searchString;
  };
  
  /** Store a parsed date. */
  struct store_date {
    /** Constructor. */
    store_date (SearchString_T& ioSearchString)
      : _searchString (ioSearchString) {}
    
    /** Parse the date. */
    void operator() (iterator_t iStr, iterator_t iStrEnd) const {
      _searchString._tmpDate._date = _searchString._tmpDate.getDate();
      // std::cout << "Board date: "
      // << _searchString._date << std::endl;
    
      // Add the parsed date to the list
      _searchString._dateList.push_back (_searchString._tmpDate);
    }
    
    SearchString_T& _searchString;
  };

  /** Store the airline sign (+/-). */
  struct store_airline_sign {
    /** Constructor. */
    store_airline_sign (SearchString_T& ioSearchString)
      : _searchString (ioSearchString) {}

    /** Parse the airline sign. */
    void operator() (bool iAirlineSign) const {
      _searchString._tmpAirline._isPreferred = !iAirlineSign;
      // std::cout << "Airline is preferred: " << iAirlineSign << std::endl;
    }
    
    SearchString_T& _searchString;
  };
  
  /** Store the parsed airline code. */
  struct store_airline_code {
    /** Constructor. */
    store_airline_code (SearchString_T& ioSearchString)
      : _searchString (ioSearchString) {}
    
    /** Parse the airline code. */
    void operator() (iterator_t iStr, iterator_t iStrEnd) const {
      std::string lAirlineCode (iStr, iStrEnd);
      _searchString._tmpAirline._code = lAirlineCode;
      // std::cout << "Airline code: " << lAirlineCode << std::endl;
      
      // Add the parsed airline to the list
      _searchString._airlineList.push_back (_searchString._tmpAirline);
    }
    
    SearchString_T& _searchString;
  };
  
  /** Store the parsed airline name. */
  struct store_airline_name {
    /** Constructor. */
    store_airline_name (SearchString_T& ioSearchString)
      : _searchString (ioSearchString) {}
    
    /** Parse the airline name. */
    void operator() (iterator_t iStr, iterator_t iStrEnd) const {
      std::string lAirlineName (iStr, iStrEnd);
      _searchString._tmpAirline._name = lAirlineName;
      // std::cout << "Airline: " << lAirlineName << std::endl;
      
      // Add the parsed airline to the list
      _searchString._airlineList.push_back (_searchString._tmpAirline);
    }
    
    SearchString_T& _searchString;
  };
  
  /** Store the parsed number of passengers. */
  struct store_passenger_number {
    /** Constructor. */
    store_passenger_number (SearchString_T& ioSearchString)
      : _searchString (ioSearchString) {}

    /** Parse number of passengers. */
    void operator() (unsigned int iNumber) const {
      _searchString._tmpPassenger._number = iNumber;
      // std::cout << "Number of passengers: " << iNumber << std::endl;
    }
    
    SearchString_T& _searchString;
  };
  
  /** Store the parsed passenger type. */
  struct store_adult_passenger_type {
    /** Constructor. */
    store_adult_passenger_type (SearchString_T& ioSearchString)
      : _searchString (ioSearchString) {}
    
    /** Parse adult passenger type. */
    void operator() (iterator_t iStr, iterator_t iStrEnd) const {
      std::string lPassengerType (iStr, iStrEnd);
      _searchString._tmpPassenger._type = Passenger_T::ADULT;
      // std::cout << "Passenger type: " << lPassengerType << std::endl;
      
      // Add the parsed passenger to the list
      _searchString._passengerList.push_back (_searchString._tmpPassenger);
    }
    
    SearchString_T& _searchString;
  };
  
  /** Store the parsed passenger type. */
  struct store_child_passenger_type {
    /** Constructor. */
    store_child_passenger_type (SearchString_T& ioSearchString)
      : _searchString (ioSearchString) {}
    
    /** Parse child passenger type. */
    void operator() (iterator_t iStr, iterator_t iStrEnd) const {
      std::string lPassengerType (iStr, iStrEnd);
      _searchString._tmpPassenger._type = Passenger_T::CHILD;
      // std::cout << "Passenger type: " << lPassengerType << std::endl;
      
      // Add the parsed passenger to the list
      _searchString._passengerList.push_back (_searchString._tmpPassenger);
    }
    
    SearchString_T& _searchString;
  };
  
  /** Store the parsed passenger type. */
  struct store_pet_passenger_type {
    /** Constructor. */
    store_pet_passenger_type (SearchString_T& ioSearchString)
      : _searchString (ioSearchString) {}

    /** Parse pet passenger type. */
    void operator() (iterator_t iStr, iterator_t iStrEnd) const {
      std::string lPassengerType (iStr, iStrEnd);
      _searchString._tmpPassenger._type = Passenger_T::PET;
      // std::cout << "Passenger type: " << lPassengerType << std::endl;
      
      // Add the parsed passenger to the list
      _searchString._passengerList.push_back (_searchString._tmpPassenger);
    }
    
    SearchString_T& _searchString;
  };

  // /////////// Utilities /////////////
  /** 1-digit-integer parser */
  boost::spirit::classic::int_parser<unsigned int, 10, 1, 1> int1_p;
  /** 1-digit-integer parser */
  boost::spirit::classic::uint_parser<unsigned int, 10, 1, 1> uint1_p;
  /** Up-to-2-digit-integer parser */
  boost::spirit::classic::uint_parser<unsigned int, 10, 1, 2> uint1_2_p;
  /** 2-digit-integer parser */
  boost::spirit::classic::uint_parser<int, 10, 2, 2> uint2_p;
  /** Up-to-4-digit-integer parser */
  boost::spirit::classic::uint_parser<int, 10, 2, 4> uint2_4_p;
  /** 4-digit-integer parser */
  boost::spirit::classic::uint_parser<int, 10, 4, 4> uint4_p;
  /** Up-to-4-digit-integer parser */
  boost::spirit::classic::uint_parser<int, 10, 1, 4> uint1_4_p;

  //////////////////////////////////////////////////////////////////////////////
  //
  //  Our calculator grammar (using subrules)
  //
  //////////////////////////////////////////////////////////////////////////////
  /**
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

  using namespace boost::spirit::classic;

  /** Grammar for the search string parser. */
  struct SearchStringParser : 
    public boost::spirit::classic::grammar<SearchStringParser> {

    SearchStringParser (SearchString_T& ioSearchString) 
      : _searchString (ioSearchString) {
    }

    template <typename ScannerT>
    struct definition {
      definition (SearchStringParser const& self) {

        search_string = places
          >> !( dates )
          >> *( preferred_airlines )
          >> *( passengers )
          ;

        places =
          +( place_element )
          ;

        place_element =
          lexeme_d[ (repeat_p(1,20)[chset_p("a-z")])[store_place_element(self._searchString)] ]
          ;
      
        dates =
          date[store_date(self._searchString)]
          >> !date[store_date(self._searchString)]
          ;
      
        date =
          ( month | day )
          >> boost::spirit::classic::chset_p("/-")
          >> ( day | month )
          >> ! ( boost::spirit::classic::chset_p("/-")
                 >> year )
          ;

        day =
          lexeme_d[ limit_d(1u,31u)[uint1_2_p][assign_a(self._searchString._tmpDate._day)] ]
          ;
        
        month =
          lexeme_d[ limit_d(1u,12u)[uint1_2_p][assign_a(self._searchString._tmpDate._month)] ]
          ;

        year =
          lexeme_d[ limit_d(2000u,2099u)[uint4_p][assign_a(self._searchString._tmpDate._year)] ]
          | lexeme_d[ limit_d(0u,99u)[uint2_p][assign_a(self._searchString._tmpDate._year)] ]
          ;

        preferred_airlines =
          !(boost::spirit::classic::sign_p)[store_airline_sign(self._searchString)]
          >> airline_code | airline_name
          ;

        airline_code =
          lexeme_d[ (repeat_p(2,3)[chset_p("0-9a-z")])[store_airline_code(self._searchString)] ]
          ;
      
        airline_name =
          lexeme_d[ (repeat_p(4,20)[chset_p("0-9a-z")])[store_airline_name(self._searchString)] ]
          ;

        passengers =
          passenger_number >> passenger_type
          ;
      
        passenger_number =
          lexeme_d[ limit_d(1u, 9u)[uint1_p][store_passenger_number(self._searchString)] ]
          ;

        passenger_type =
          passenger_adult_type[store_adult_passenger_type(self._searchString)]
          | passenger_child_type[store_child_passenger_type(self._searchString)]
          | passenger_pet_type[store_pet_passenger_type(self._searchString)]
          ;

        passenger_adult_type =
          lexeme_d[ as_lower_d [ str_p("adult") >> !ch_p('s') ] ]
          ;
      
        passenger_child_type =
          lexeme_d[ as_lower_d [ str_p("child") >> !str_p("ren") ] ]
          ;
      
        passenger_pet_type =
          lexeme_d[ as_lower_d [ str_p("dog") | str_p("cat") >> !ch_p('s') ] ]
          ;
      
        BOOST_SPIRIT_DEBUG_NODE (search_string);
        BOOST_SPIRIT_DEBUG_NODE (places);
        BOOST_SPIRIT_DEBUG_NODE (place_element);
        BOOST_SPIRIT_DEBUG_NODE (dates);
        BOOST_SPIRIT_DEBUG_NODE (date);
        BOOST_SPIRIT_DEBUG_NODE (day);
        BOOST_SPIRIT_DEBUG_NODE (month);
        BOOST_SPIRIT_DEBUG_NODE (year);
        BOOST_SPIRIT_DEBUG_NODE (preferred_airlines);
        BOOST_SPIRIT_DEBUG_NODE (airline_code);
        BOOST_SPIRIT_DEBUG_NODE (airline_name);
        BOOST_SPIRIT_DEBUG_NODE (passengers);
        BOOST_SPIRIT_DEBUG_NODE (passenger_number);
        BOOST_SPIRIT_DEBUG_NODE (passenger_type);
        BOOST_SPIRIT_DEBUG_NODE (passenger_adult_type);
        BOOST_SPIRIT_DEBUG_NODE (passenger_child_type);
        BOOST_SPIRIT_DEBUG_NODE (passenger_pet_type);
      }
    
      boost::spirit::classic::rule<ScannerT> search_string, places, place_element,
        dates, date, month, day, year,
        preferred_airlines, airline_code, airline_name,
        passengers, passenger_number, passenger_type, passenger_adult_type,
        passenger_child_type, passenger_pet_type;

      boost::spirit::classic::rule<ScannerT> const& start() const { return search_string; }
    };

    SearchString_T& _searchString;
  };

  // //////////////////////////////////////////////////////////
  SearchString_T parseBookingRequest (const std::string& iSearchString) {
    SearchString_T oSearchStringStruct;
  
    // Read the search string
    iterator_t lStringIterator = iSearchString.c_str();

    // Instantiate the structure that will hold the result of the parsing.
    SearchStringParser lSearchStringParser (oSearchStringStruct);
    boost::spirit::classic::parse_info<iterator_t> info =
      boost::spirit::classic::parse (lStringIterator, lSearchStringParser, 
                                     boost::spirit::classic::space_p);
  
    STDAIR_LOG_DEBUG ("-------------------------");
  
    bool hasBeenParsingSuccessful = info.full;
    if (hasBeenParsingSuccessful == true) {
      STDAIR_LOG_DEBUG ("Parsing succeeded");
    
    } else {
      STDAIR_LOG_DEBUG ("Parsing failed");
    }
    STDAIR_LOG_DEBUG ("-------------------------");
  
    return oSearchStringStruct;
  }

}
